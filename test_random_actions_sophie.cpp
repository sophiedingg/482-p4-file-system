#include <iostream>
#include <cassert>
#include <cstdlib>
#include <set>
#include "fs_client.h"

int main(int argc, char *argv[]) {
    srand(0);
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

    std::set<std::string> names_set;
    names_set.insert("/david");
    names_set.insert("/anand");
    names_set.insert("/peter");
    names_set.insert("/chen");
    names_set.insert("/larry");
    names_set.insert("/justin");
    names_set.insert("/neil");
    names_set.insert("/molly");
    names_set.insert("/pritika");
    names_set.insert("/sophieanandsophieanandsophieanandsophieanandsophieanandsophieanand");
    names_set.insert("/sophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavid");

    while (true) {
        std::cout << "1\n";
        // chooses length of path
        int path_length = rand() % 4;
        std::cout << "2\n";
        // build path name
        std::string path = "";
        for (int i = 0; i < path_length; ++i) {
            // chooses file/directory name in names set
            int set_val = rand() % names_set.size();
            auto it = names_set.begin();
            for (int i = 0; i < set_val; i++){
                it++;
            }
            path += *it;
        }
        std::cout << "3\n";
        const char * pathname = path.c_str();

        // chooses type
        int type_int = rand() % 3;    
        std::cout << "4\n";
        char type;
        if (type_int == 0) {
            type = 'f';
        }
        else if (type_int == 1) {
            type = 'd';
        }
        else {
            type = 's';
        }

        // chooses username
        int username_int = rand() % 3;
        std::cout << "5\n";
        std::string username_string;
        if (username_int == 0) {
            username_string = "sophie";
        }
        else if (username_int == 1) {
            username_string = "jonathan";
        }
        else {
            username_string = "longusername";
        }
        const char *username = username_string.c_str();

        // choose block_index
        int block_index_int = rand() % 2000;
        std::cout << "6\n";
        int block_index = block_index_int - 1;

        // choose operation
        int random = rand() % 4;
        std::cout << "7\n";
        
        if (random == 0) {
            std::cout << "a\n";
            status = fs_create(username, pathname, type);
            std::cout << "b\n";
        }
        else if (random == 1) {
            std::cout << "c\n";
            status = fs_readblock(username,  pathname, block_index, readdata);
            std::cout << "d\n";
        }
        else if (random == 2) {
            std::cout << "e\n";
            status = fs_writeblock(username, pathname, block_index, writedata);
            std::cout << "f\n";
        }
        else {
            std::cout << "g\n";
            status = fs_delete(username, pathname);
            std::cout << "h\n";
        }
        std::cout << "8\n";
    }
    std::cout << "9\n";
}
