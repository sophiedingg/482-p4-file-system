#include <iostream>
#include <cassert>
#include <cstdlib>
#include "fs_client.h"

// use max disk blocks
int main(int argc, char *argv[]) {
    char *server;
    int server_port;

    const char *writedata = "We hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";
    const char *writedata1 = "Anand David Sophie = EECS Roomies";
    char readdata[FS_BLOCKSIZE];
    int status;

    if (argc != 3) {
        std::cout << "error: usage: " << argv[0] << " <server> <serverPort>\n";
        exit(1);
    }
    server = argv[1];
    server_port = atoi(argv[2]);

    fs_clientinit(server, server_port);

    static constexpr unsigned int FS_DIRENTRIES = 8;

    // make the first entry a direntry
    status = fs_create("user1", "/dir1", 'd');
    status = fs_create("user1", "/dir2", 'd');

    // for the next six files
    for (size_t i = 2; i < 8; i++){
        std::string string_path = "/file" + std::to_string(i);
        const char * pathname = string_path.c_str();
        status = fs_create("user1", pathname, 'f');

        // write 4 blocks
        status = fs_writeblock("user1", pathname, 0, writedata);
        status = fs_writeblock("user1", pathname, 1, writedata);
        status = fs_writeblock("user1", pathname, 2, writedata);
        status = fs_writeblock("user1", pathname, 3, writedata);
    }

    // for the remaining space in root, create a file
    // use 3 disk blocks within the file
    for (size_t i = 1; i < FS_MAXFILEBLOCKS; i++){
        for (size_t j = 0; j < FS_DIRENTRIES; j++){
            std::string string_path = "/" + std::to_string(i) + "file" + std::to_string(j);
            const char * pathname = string_path.c_str();
            status = fs_create("user1", pathname, 'f');

            // write 3 blocks
            status = fs_writeblock("user1", pathname, 0, writedata);
            status = fs_writeblock("user1", pathname, 1, writedata);
            status = fs_writeblock("user1", pathname, 2, writedata);
        }
    }

    // now should only have 3 disk blocks left
    // 1 disk block for root
    // 124 disk blocks for all the blocks in root
    // 124 * 8 = 992 for all direntry inodes in root.blocks
    // 4 * 6 = 24 for the blocks used in the first direntry array
    // 123 * 8 * 3 = 2952 for all the file blocks in direntry arrays 1-123    

    // create a file on the first dir 
    // should use 2 disk blocks
    // 1 disk block left
    status = fs_create("user1", "/dir1/file", 'f');

    // create on second dir
    // should fail
    status = fs_create("user1", "/dir2/file", 'f');

    // delete 1 file
    // should be 2 disk blocks left
    status = fs_delete("user1", "/1file1");

    // create on second dir
    // should work
    status = fs_create("user1", "/dir2/file", 'f');
}
