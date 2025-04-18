#include <algorithm>
#include <arpa/inet.h>
#include <array>
#include <boost/regex.hpp>
#include <queue>
#include <string>
#include <set>
#include <map>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include "fs_server.h"

using std::cout, std::string, std::pair;

class rw_lock{
public:
    std::shared_ptr<boost::shared_mutex> mutex;
    int reader;

    rw_lock() : reader(-1) {}

    void reader_lock(){
        mutex->lock_shared();
    }
    void writer_lock(){
        mutex->lock();
    }

    void set_rw_lock(std::shared_ptr<boost::shared_mutex> mtx, int reader_in){
        mutex = mtx;
        reader = reader_in;
        if(reader == 1){
            reader_lock();
        }else if(reader == 0){
            writer_lock();
        }
    }

    void reader_lock(std::shared_ptr<boost::shared_mutex> mtx, int reader_in){
        set_rw_lock(mtx, reader_in);
    }
    void writer_lock(std::shared_ptr<boost::shared_mutex> mtx, int reader_in){
        set_rw_lock(mtx, reader_in);
    }

    void unlock_mutex(){
        if(reader == 1){
            mutex->unlock_shared();
        }else if (reader == 0){
            mutex->unlock();
        }
    }

    void hand_over_hand(std::shared_ptr<boost::shared_mutex> new_mtx, int reader_in){
        if(reader_in == 1){
            new_mtx->lock_shared();
        }else if(reader_in == 0){
            new_mtx->lock();
        }
        unlock_mutex();
        mutex = new_mtx;
        reader = reader_in;
    }

    ~rw_lock(){
        if(reader != -1){
            unlock_mutex();
        }
    }
};

class free_db {
    boost::mutex free_db_lock;
    std::set <int> free_disk_blocks;
public:
    void init_fdb(){
        boost::lock_guard<boost::mutex> lock(free_db_lock);
        for (size_t i=0;i<FS_DISKSIZE;i++){
            free_disk_blocks.insert(i);
        }
    }

    void insert_fdb(int block){
        boost::lock_guard<boost::mutex> lock(free_db_lock);
        free_disk_blocks.insert(block);
    }

    int next_fdb_non_empty(){
        boost::lock_guard<boost::mutex> lock(free_db_lock);
        // check if space on disk
        if (free_disk_blocks.empty()) {
            perror("No space on disk for new block in fs_write");
            return -1;
        }
        // allocate new block
        int new_file_block = *free_disk_blocks.begin();
        free_disk_blocks.erase(new_file_block);
        return new_file_block;
    }

    void add_file_dir_fdb(fs_inode &current_inode, fs_direntry &entry){
        boost::lock_guard<boost::mutex> lock(free_db_lock);
        // if a file that is using blocks
        if (current_inode.type == 'f') {
            for (size_t i = 0; i < current_inode.size; ++i) {
                free_disk_blocks.insert(current_inode.blocks[i]);
            }
        }
        free_disk_blocks.insert(entry.inode_block);
    }

    void check_for_db_fdb(int disk_block, fs_inode &inode){
        boost::lock_guard<boost::mutex> lock(free_db_lock);
        // delete this inode's block
        auto current_inode_block = free_disk_blocks.find(disk_block);
        free_disk_blocks.erase(current_inode_block);

        // iterate through this data blocks and delete
        for (size_t i = 0; i < inode.size; i++){
            auto cur_file_block = free_disk_blocks.find(inode.blocks[i]);
            free_disk_blocks.erase(cur_file_block);
        }
    }
};

free_db fdb;

// hard maybe right here
int MAX_MESSAGE_SIZE = 13 + 1 + FS_MAXUSERNAME + 1 + FS_MAXPATHNAME + 1 + 3 + 1 + FS_BLOCKSIZE;

// inode block to rw lock
std::vector<std::shared_ptr<boost::shared_mutex>> master_locks;

std::queue<string> splice_string(string pathname){
    std::istringstream iss(pathname);
    std::queue<string> result;
    string ppart;
    // helps with if there's a double slash somewhere
    bool first_one = true;
    // get each part of the string (seperated by '/')
    while (std::getline(iss, ppart, '/')) {
        if (!ppart.empty()) {
            if (ppart.size() > FS_MAXFILENAME) {
                return {};
            }
            result.push(ppart);
        }
        else if(first_one){
            first_one = false;
        }
        else{
            return {};
        }
    }
    return result;
}

// traverses file system
// set parent_inode to be the parent inode of name
// returns parent inode's block or -1 if error
// takes in an RAII reader writer lock class
int traverse_helper(std::queue<string> &path, fs_inode *parent_inode, std::vector<string> request, rw_lock &parent_lock) {
    // access root inode
    string req_type = request[1];
    const char * user = (request[3]).c_str();
    fs_inode current_inode;
    int prev_block = 0;

    // if root is parent and its a read or write
    if(path.size() == 1 && (req_type != "FS_READBLOCK" && req_type != "FS_WRITEBLOCK")){ // and not read
        parent_lock.writer_lock(master_locks[prev_block], 0);
    }else{
        parent_lock.reader_lock(master_locks[prev_block], 1);
    }
    disk_readblock(prev_block, &current_inode);
    // root is the immediate parent
    if(path.size() == 1){
        // return our of function with reader or writer lock on root
        *parent_inode = current_inode;
        return 0;
    }

    fs_direntry parent_entries[FS_DIRENTRIES];
    // loop until only one item left (file/dir to create)
    // or file/dir to read or write to (?)
    while (path.size() > 0) {
        bool found = false;
        if (current_inode.type != 'd'){
            // ERROR (not directory type)
            perror("found a file in the middle of the path in traverse_helper");
            return -1;
        }

        // current block we are on in inode.blocks array
        uint32_t current_dir_index = 0;

        // for each block in the fs_inode
        while (current_dir_index < current_inode.size && !found) {
            // read the next array of direntries
            disk_readblock(current_inode.blocks[current_dir_index], parent_entries);

            // for each entry, if valid, check name
            for (size_t i = 0; i < FS_DIRENTRIES; ++i) {
                // if we find the next directory, set new current inode
                if (parent_entries[i].inode_block != 0 && parent_entries[i].name == path.front()) { // double check c string stuff
                    path.pop();
                    // hand over hand locking for each inode
                    if(path.size() <= 1 && (req_type != "FS_READBLOCK" && req_type != "FS_WRITEBLOCK")){ // and not read
                        parent_lock.hand_over_hand(master_locks[parent_entries[i].inode_block], 0);
                    }else{
                        parent_lock.hand_over_hand(master_locks[parent_entries[i].inode_block], 1);
                    }
                    prev_block = parent_entries[i].inode_block;
                    disk_readblock(parent_entries[i].inode_block, &current_inode);
                    found = true;

                    // check to make sure this is owned by the user
                    if (strcmp(current_inode.owner, user) != 0) {
                        perror("tried to access a directory not owned");
                        // prev_block has already been updated at this point
                        return -1;
                    }

                    if(path.size() <= 1){
                        // would want to return holding the reader or writer lock
                        if (current_inode.type != 'd') {
                            perror("parent inode is a file");
                            return -1;
                        }
                        *parent_inode = current_inode;
                        return parent_entries[i].inode_block;
                    }
                    break;
                }
            }
            if (!found) { current_dir_index++; }
            else { break; }
        }
        
        if(!found){
            // ERROR dir not found for all cases
            perror("didn't find the correct dir in traverse_helper");
            return -1;
        }
    }
    return -1;
}

// only used in fs_create
// searches through all direntries
// first value is free_dir, second is entries_dir_index
// free_dir the first empty direntry found, -2 if the name exists, -1 if there is no space
// entries_dir_index is how to get to entries from parent_inode.blocks
std::pair<int, int> find_empty_direntry_create(fs_inode *parent_inode, std::string name, fs_direntry ret_entries[FS_DIRENTRIES], fs_direntry *ret_direntry) {
    bool first = true;
    int free_dir = -1;
    int entries_dir_index = -1;
    fs_direntry entries[FS_DIRENTRIES];
    for(size_t i=0;i<parent_inode->size;i++){
        disk_readblock(parent_inode->blocks[i], entries);
        for (size_t j = 0; j < FS_DIRENTRIES; j++) {
            // if we find the direntry we are looking for, set new current inode
            if (entries[j].inode_block != 0 && entries[j].name == name) {
                return std::make_pair(-2, -2);
            }
            // find first empty direntry to create i
            if(first && entries[j].inode_block == 0){
                first = false;
                free_dir = j;
                entries_dir_index = i;
                memcpy(ret_entries, entries, FS_BLOCKSIZE);
            }
        }
    }
    return std::make_pair(free_dir, entries_dir_index);
}

// EFFECTS: Returns bool saying if name is found in direntries of parent_inode
// Loops through array of direntries to find the direntry specified
// will edit ret_direntry and entries
bool find_direntry(fs_inode *parent_inode, std::string name, fs_direntry entries[FS_DIRENTRIES], fs_direntry *ret_direntry) {
    for(size_t i=0;i<parent_inode->size;i++){
        disk_readblock(parent_inode->blocks[i], entries);
        for (size_t j = 0; j < FS_DIRENTRIES; j++) {
            // if we find the direntry we are looking for, set new current inode
            if (entries[j].inode_block != 0 && entries[j].name == name) {
                *ret_direntry = entries[j];
                return true;
            }
        }
    }
    return false;
}

void send_message_same(int connectionfd, std::vector<string> request){
    // create string from entire request
    string temp_str = request[0];
    // define response to be one char longer
    char response[temp_str.length() + 1];
    // copy temp_str to response
    strcpy(response, temp_str.c_str());
    // set final char in response to be null character
    response[strlen(response)] = '\0';
    // get length of entire message, strlen plus null character
    const ssize_t response_len = strlen(response) + 1;
    ssize_t sent = 0;
    do {
        const ssize_t n = send(connectionfd, response + sent, response_len - sent, MSG_NOSIGNAL);
        if (n == -1) {
            perror("Error sending on stream socket");
            return;
        }
        sent += n;
    } while (sent < response_len);
}

std::vector<string> fs_create(int connectionfd, std::vector<string> request){
    // split string
    std::queue<std::string> path = splice_string(request[4]);
    if (path.empty()){
        perror("Path returned empty in fs_create");
        return {};
    }

    // the disk block for the new file or directory
    // already checked that we have space on the disk
    int new_inode_block = fdb.next_fdb_non_empty();
    if(new_inode_block == -1){
        return {};
    }

    // check filename length
    if (path.back().length() > FS_MAXFILENAME){
        perror("Filename too long");
        fdb.insert_fdb(new_inode_block);
        return {};
    }

    // only two options, ' f' or ' d' (filetypes)
    string extra_str = request[5];
    if(extra_str != " f" && extra_str != " d"){
        perror("fs_create type input invalid\n");
        fdb.insert_fdb(new_inode_block);
        return {};
    }

    // save name of the new file or directory
    char name[FS_MAXFILENAME + 1];
    strcpy(name, path.back().c_str());

    // get parent inode of the one we want to create
    fs_inode parent_inode;
    rw_lock parent_lock;
    int parent_inode_block = traverse_helper(path, &parent_inode, request, parent_lock);
    if (parent_inode_block == -1){
        perror("traverse_helper did not work for fs_create");
        fdb.insert_fdb(new_inode_block);
        return {};
    }

    // not used, just to avoid error
    fs_direntry temp_direntry;
    int free_dir = -1;
    // this is the direntries that point to what we are creating
    fs_direntry entries[FS_DIRENTRIES];
    // check if the file / directory to be created is already there
    std::pair<int, int> empty_dir_pair = find_empty_direntry_create(&parent_inode, name, entries, &temp_direntry);
    free_dir = empty_dir_pair.first;
    int entries_dir_index = empty_dir_pair.second;
    // if it already exists
    if (free_dir == -2){
        perror("File already exists when running create");
        fdb.insert_fdb(new_inode_block);
        return {};
    }

    // if we need to create a new block, check we have space
    if(free_dir == -1){
        if (parent_inode.size >= FS_MAXFILEBLOCKS) {
            fdb.insert_fdb(new_inode_block);
            perror("Need to create new block for direntry in fs_create, and not enough space");
            return {};
        }
    }

    // direntries in the parent's inode are full
    if(free_dir == -1){

        // get the next free disk block for the array of new direntries
        int new_array_block = fdb.next_fdb_non_empty();
        if(new_array_block == -1){
            return {};
        }

        // creating the actual inode and writing to disk
        fs_inode temp_inode;
        strcpy(temp_inode.owner, request[3].c_str());
        temp_inode.type = extra_str[1];
        temp_inode.size = 0;
        disk_writeblock(new_inode_block, &temp_inode);

        fs_direntry temp;
        temp.inode_block = 0;
        fs_direntry new_entries[FS_DIRENTRIES];
        std::fill(std::begin(new_entries), std::end(new_entries), temp);

        // set the direntry with name and inode block
        strcpy(new_entries[0].name, name);
        new_entries[0].inode_block = new_inode_block;

        // write back to disk
        disk_writeblock(new_array_block, new_entries);

        // connect this new array of direntries to parent inode and increment size
        // figure out how to get parent's writer lock
        parent_inode.blocks[parent_inode.size] = new_array_block;
        parent_inode.size++;

        // write parent inode back to disk
        disk_writeblock(parent_inode_block, &parent_inode);
    }
    else {
        // creating the actual inode and writing to disk
        fs_inode temp_inode;
        strcpy(temp_inode.owner, request[3].c_str());
        temp_inode.type = extra_str[1];
        temp_inode.size = 0;
        disk_writeblock(new_inode_block, &temp_inode);

        // set the direntry with name and inode block
        // get parent inode writer lock
        strcpy(entries[free_dir].name, name);
        entries[free_dir].inode_block = new_inode_block;

        // write back to disk
        disk_writeblock(parent_inode.blocks[entries_dir_index], entries);
    }
    return request;
}

std::vector<string> fs_read(int connectionfd, std::vector<string> request, char *data){
    // block should be between 0 and (current_inode.size - 1)
    // space then 1, 2, or 3 digit number
    const boost::regex read_expr{ " [\\d]{1,3}" };
    string extra_str = request[5];
    boost::match_results<string::const_iterator> results;
    if (boost::regex_match(extra_str, results, read_expr)){
        // get rid of the space at the beginning
        string temp_str = extra_str.substr(1, extra_str.size()-1);

        // check for leading zeros
        if (temp_str.size() > 1 && temp_str[0] == '0'){
            perror("found leading zero");
            return {};
        }

        // get block number from string
        uint32_t block_index = stoi(temp_str);

        // early error checking
        if (block_index < 0 || block_index >= FS_MAXFILEBLOCKS){
            perror("block index out of range in fs_read");
            return {};
        }
        // error checking for directory in path name too long
        std::queue<std::string> path = splice_string(request[4]);
        if (path.empty()){
            perror("Path returned empty in fs_read");
            return {};
        }

        // check if is root (path empty)
        if (path.empty()) {
            perror("Trying to delete root");
            return {};
        }

        // save name of the new file or directory
        char name[FS_MAXFILENAME + 1];
        strcpy(name, path.back().c_str());
 
        fs_inode parent_inode;
        rw_lock parent_lock;
        int parent_inode_block = traverse_helper(path, &parent_inode, request, parent_lock);
        // should have parent reader lock now
        if (parent_inode_block == -1){
            // fail  (we have already unlocked within traverse_helper)
            perror("traverse_helper did not work for fs_read");
            return {};
        }

        // check if the file / directory to be created is already there
        fs_direntry current_direntry;
        fs_direntry entries[FS_DIRENTRIES];
        if(!find_direntry(&parent_inode, name, entries, &current_direntry)){
            // ERROR not able to find last part of the path
            perror("find_direntry did not work for fs_read");
            return {};
        }

        fs_inode current_inode;

        // hand over hand locking from parent to child
        parent_lock.hand_over_hand(master_locks[current_direntry.inode_block], 1);
        
        disk_readblock(current_direntry.inode_block, &current_inode);

        // need to make sure they are reading a valid block
        // added in current_inode.size == 0 because the -1 was messing up the unsigned int
        if (current_inode.size == 0 || block_index > current_inode.size-1){
            perror("block_index out of bounds for fs_read");
            return {};
        }

        // check that direntry is a file
        if (current_inode.type != 'f'){
            perror("reading from a directory");
            return {};
        }

        // check that the username entered == owner in the inode
        if(current_inode.owner != request[3]) {
            perror("non-owner reading");
            return {};
        }

        // access the information in this block
        disk_readblock(current_inode.blocks[block_index], data);
        return request;
    }
    else {
        perror("read input invalid\n");
        return {};
    }
}

std::vector<string> fs_write(int connectionfd, std::vector<string> request, char* writeblock){
    // block should be between 0 and 123
    // space then 1, 2, or 3 digit number
    const boost::regex write_expr{ " [\\d]{1,3}" };
    string extra_str = request[5];
    boost::match_results<string::const_iterator> results;
    if (boost::regex_match(extra_str, results, write_expr)){
        // get rid of the space at the beginning
        string temp_str = extra_str.substr(1, extra_str.size()-1);
    
        // check for leading zeros
        if (temp_str.size() > 1 && temp_str[0] == '0'){
            perror("found leading zero");
            return {};
        }

        // get block number from string
        uint32_t block_index = stoi(temp_str);

        // early error checking
        if (block_index < 0 || block_index >= FS_MAXFILEBLOCKS){
            perror("block index out of range in fs_write");
            return {};
        }

        // split string
        // error checking for directory in path name too long
        std::queue<std::string> path = splice_string(request[4]);
        if (path.empty()){
            perror("Path returned empty in fs_write");
            return {};
        }

        // check if is root
        if (path.empty()) {
            perror("Trying to write to root");
            return {};
        }

        // save name of the new file or directory
        char name[FS_MAXFILENAME + 1];
        strcpy(name, path.back().c_str());

        fs_inode parent_inode;
        rw_lock parent_lock;
        int parent_inode_block = traverse_helper(path, &parent_inode, request, parent_lock);
        if (parent_inode_block == -1){
            perror("traverse_helper did not work for fs_write");
            return {};
        }
        // should have reader lock for parent inode

        // check if the file / directory to be created is already there
        fs_direntry current_direntry;
        fs_direntry entries[FS_DIRENTRIES];
        if(!find_direntry(&parent_inode, name, entries, &current_direntry)){
            // ERROR not able to find last part of the path
            perror("find_direntry did not work for fs_write");
            return {};
        }

        fs_inode current_inode;
        parent_lock.hand_over_hand(master_locks[current_direntry.inode_block], 0);

        disk_readblock(current_direntry.inode_block, &current_inode);

        // need to make sure they are reading a valid block
        if (block_index > current_inode.size){
            // fail
            perror("block_index out of bounds for fs_write");
            return {};
        }

        // check that direntry is a file
        if (current_inode.type != 'f') {
            perror("Writing to a directory");
            return {};
        }

        // check that the username entered == owner in the inode
        if (current_inode.owner != request[3]){
            perror("Non-owner writing");
            return {};
        }

        if (block_index < current_inode.size) {
            disk_writeblock(current_inode.blocks[block_index], writeblock);
        }
        else if (block_index == current_inode.size) { // new block to be written to
            // check if space in file
            if (current_inode.size == FS_MAXFILEBLOCKS) {
                // ERROR not enough space in file
                perror("Inode is full in fs_write");
                return {};
            }
            int new_file_block = fdb.next_fdb_non_empty();
            if(new_file_block == -1){
                return {};
            }
            disk_writeblock(new_file_block, writeblock);

            current_inode.blocks[current_inode.size] = new_file_block;
            current_inode.size++;
            // COMMIT
            disk_writeblock(current_direntry.inode_block, &current_inode);
        }        
    }
    else {
        perror("write input invalid\n");
        // fail
        return {};
    }
    return request;
}

std::vector<string> fs_delete(int connectionfd, std::vector<string> request){
    // split string
    // error checking for directory in path name too long
    std::queue<std::string> path = splice_string(request[4]);
    if (path.empty()){
        // fail
        perror("Path returned empty in fs_delete");
        return {};
    }

    // check if there is anything after the pathname in the request
    if(!request[5].empty()){
        // fail
        perror("Something after pathname in fs_delete");
        return {};
    }

    // check if is root
    if (path.empty()) {
        // ERROR trying to delete root
        perror("Trying to delete root");
        return {};
    }

    // save name of the new file or directory
    char name[FS_MAXFILENAME + 1];
    strcpy(name, path.back().c_str());
    fs_inode parent_inode;
    rw_lock parent_lock;
    int parent_inode_block = traverse_helper(path, &parent_inode, request, parent_lock);
    if (parent_inode_block == -1){
        // fail  (we have already unlocked within traverse_helper)
        perror("traverse_helper did not work for fs_delete");
        return {};
    }
    // should have writer lock for parent inode here

    uint32_t current_block_index = 0;
    fs_direntry entries[FS_DIRENTRIES];
    size_t entries_index = 0;

    bool found = false;
    fs_inode current_inode;

    // looking through parent inode for desired direntry
    rw_lock child_lock;
    for(size_t i=0;i<parent_inode.size;i++){
        disk_readblock(parent_inode.blocks[i], entries);
        for (size_t j = 0; j < FS_DIRENTRIES; j++) {
            // if we find the direntry we are looking for, set new current inode
            if (entries[j].inode_block != 0 && strcmp(entries[j].name, name) == 0) {
                // shouldn't do hand over hand locking in case we need to modify parent ?
                // write lock current inode
                child_lock.writer_lock(master_locks[entries[j].inode_block], 0);
                disk_readblock(entries[j].inode_block, &current_inode);
                entries_index = j;
                found = true;
                current_block_index = i;
                break;
            }
        }
        if(found){
            break;
        }
    }
    // now have parent_inode writer lock and current_inode writer lock
    int current_inode_block = entries[entries_index].inode_block;
    if (!found) {
        // ERROR couldn't find direntry to delete
        perror("Can't find direntry that we want to delete in fs_delete");
        return {};
    }

    // check that the username entered == owner in the inode
    if (current_inode.owner != request[3]){
        perror("Non-owner deleting");
        return {};
    }

    // if a directory
    if (current_inode.type == 'd') {
        // check that the directory contains no files or sub-directories
        if (current_inode.size != 0) {
            perror("Trying to delete directory with direntries in it in fs_delete");
            return {};
        }
    }

    // bool to see if this is the last direntry in the disk block
    bool empty = true;
    for (size_t i = 0; i < FS_DIRENTRIES; ++i) {
        if (i != entries_index && entries[i].inode_block != 0) {
            empty = false;
            break;
        }
    }

    // need to get rid of entire direntry block
    if (empty) {
        fdb.insert_fdb(parent_inode.blocks[current_block_index]);

        // shift block array over
        for (size_t i = current_block_index; i < parent_inode.size - 1; ++i) {
            parent_inode.blocks[i] = parent_inode.blocks[i + 1];
        }
        parent_inode.size--;

        // COMMIT
        disk_writeblock(parent_inode_block, &parent_inode);
    }

    // deleting the file / directory
    fdb.add_file_dir_fdb(current_inode, entries[entries_index]);

    if(!empty){
        entries[entries_index].inode_block = 0;
        disk_writeblock(parent_inode.blocks[current_block_index], &entries);
    }

    return request;
}

void send_read(int connectionfd, char data[FS_BLOCKSIZE]){
    // get length of entire message, strlen with no null character at end 
    const ssize_t data_len = FS_BLOCKSIZE;
    ssize_t sent = 0;
    do {
        const ssize_t n = send(connectionfd, data + sent, data_len - sent, MSG_NOSIGNAL);
        if (n == -1) {
            perror("Error sending on stream socket");
            return;
        }
        sent += n;
    } while (sent < data_len);
}

void handle_connection(int connectionfd) {
    char msg[MAX_MESSAGE_SIZE + 1];
    char writeblock[FS_BLOCKSIZE];
    char data[FS_BLOCKSIZE];
    memset(msg, 0, sizeof(msg));

    // Call recv() enough times to consume all the data the client sends.
    ssize_t recvd = 0;
    ssize_t rval;
    // every command has these
    string action;
    string pathname;

    boost::smatch request;
    std::vector<string> request_vec;
    string message_str;
    const boost::regex reg_expr{ "^(FS_(CREATE|DELETE|READBLOCK|WRITEBLOCK)) ([^\\s]{1,10}) (\\/[^\\s]{1,127})(.*)" };          
    // regex explanation - Starts with FS_(one of the functions)
    // then a non whitespace character for 1-10 characters
    // then starts with a slash, has a non whitespace character for 1-127 chars (doesn't check for not ending with a slash, will do that later)
    // then any characters for any amount of length (we'll check this later)
    // this regex should match all incoming messages

    // this is only used for writes
    char after_null_position[FS_BLOCKSIZE];
    do {
        // receive one byte at a time
        rval = recv(connectionfd, msg + recvd, 1, 0);
        // if we failed to receive
        if (rval < 1) {
            perror("Error reading stream message");
            break;
        }
        recvd += rval;
    } while (msg[recvd - 1] != '\0' || recvd > MAX_MESSAGE_SIZE); // recv() returns 0 when client closes
    message_str = msg;

    // if we don't match, close connection
    if (recvd <= MAX_MESSAGE_SIZE && rval > 0 && boost::regex_search(message_str, request, reg_expr)){
        for (size_t i=0; i<request.size(); i++){
            request_vec.push_back(request[i]);
        }
        action = request[1];
        pathname = request[4];
        if (pathname[pathname.size()-1] == '/'){
            perror("pathname ending in slash\n");
            request_vec.clear();
        }
        else if (action == "FS_CREATE"){
            request_vec = fs_create(connectionfd, request_vec);
        }
        else if (action == "FS_READBLOCK"){
            request_vec = fs_read(connectionfd, request_vec, data);
        }
        else if (action == "FS_WRITEBLOCK"){
            // recieve an entire block
            rval = recv(connectionfd, after_null_position, FS_BLOCKSIZE, MSG_WAITALL);
            if (rval != 512) {
                perror("Error reading stream message");
                request_vec.clear();
            }else{
                // should copy the block passed in into writeblock
                memcpy(writeblock, after_null_position, FS_BLOCKSIZE);
                request_vec = fs_write(connectionfd, request_vec, writeblock);
            }
        }
        else if (action == "FS_DELETE"){
            request_vec = fs_delete(connectionfd, request_vec);
        }
        else{
            perror("action is not one of the four for some reason\n");
            request_vec.clear();
        }

        // send message if returns successful
        if(!request_vec.empty()){
            send_message_same(connectionfd, request_vec);
            if(action == "FS_READBLOCK"){
                send_read(connectionfd, data);
            }
        }

    }else{
        perror("did not match initial regex check\n");
    }

    printf("FINAL MESSAGE: '%s'\n", msg);
    close(connectionfd);
    return;
}

void check_for_disk_blocks(int disk_block){
    // read in current inode
    fs_inode inode;
    disk_readblock(disk_block, &inode);
    // remove inode and its' data blocks
    fdb.check_for_db_fdb(disk_block, inode);

    // directory
    if(inode.type == 'd'){
        // iterate through all direntries
        for (size_t i=0;i<inode.size;i++){
            // get current direntries
            fs_direntry entries[FS_DIRENTRIES];
            disk_readblock(inode.blocks[i], &entries);
            // for each direntry in block
            for (size_t j=0;j<FS_DIRENTRIES;j++){
                if (entries[j].inode_block != 0){
                    // call recursively on new inode
                    check_for_disk_blocks(entries[j].inode_block);
                }
            }
        }
    }
}

int run_server(int port_number){
    // init master lock list
    master_locks.reserve(FS_DISKSIZE);
    for(int i = 0; i < FS_DISKSIZE; ++i){
        master_locks.emplace_back(std::make_shared<boost::shared_mutex>());
    }

    // add all free disk blocks
    fdb.init_fdb();

    // set free disk blocks based on current file system
    check_for_disk_blocks(0);

    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (sockfd == -1){
        perror("Failed initial socket setup");
        return 1;
    }
    int yesval = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yesval, sizeof(yesval)) == -1){
        perror("Failed socket setup");
        return 1;
    }

    sockaddr_in addr{}; // initializes with zeroes
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port_number);
    socklen_t addr_size = sizeof(addr);

    // bind, getsockname, listen
    if (bind(sockfd, reinterpret_cast<sockaddr*>(&addr), addr_size) == -1){
        perror("Failed bind call");
        return 1;
    }
    if (getsockname(sockfd, reinterpret_cast<sockaddr*>(&addr), &addr_size) == -1){
        perror("Failed getting socket name");
        return 1;
    }
    port_number = ntohs(addr.sin_port);
    printf("Listening on port number: %d\n", port_number);
    if (listen(sockfd, 30) == -1) {
        perror("Error listening");
        return 1;
    }

    print_port(port_number);

    // loop to accept requests
    while (1) {
        int connectionfd = accept(sockfd, NULL, NULL);
        if (connectionfd == -1) {
            perror("Error accepting connection");
            break;
        }
        printf("found connection at %d\n", connectionfd);
        boost::thread t(handle_connection, connectionfd);
        t.detach();
    }

    close(sockfd);
    return 0;
}

int main(int argc, const char **argv){
    // set port to 0 if not specified
    int port_number = 0;
    if (argc > 2) {
        printf("Usage: ./fs port_num\n");
        return 1;
    }
    else if(argc == 2){
        port_number = atoi(argv[1]);
    }
    run_server(port_number);
}
