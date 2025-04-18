#include <iostream>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <string.h>
#include "fs_client.h"

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

    // create directory
    fs_create("user1", "/dir", 'd');

    // create file
    fs_create("user1", "/file", 'f');

    // FS_MAXFILEBLOCKS = 124
    // make dir and file completely full
    int counter = 0;
    for(size_t i=0;i<FS_MAXFILEBLOCKS;i++){
        fs_writeblock("user1", "/file", i, writedata);
        for(size_t j=0;j<8;j++){
            std::string dir_name = "/dir/dir" + std::to_string(counter);
            counter++;
            char dir_c_str[dir_name.length() + 1] ;
            strcpy(dir_c_str, dir_name.c_str());
            char type = 'f';
            if(i % 2 == 0){
                type = 'd';
            }
            fs_create("user1", dir_c_str, type);
        }
    }
    // illegal
    fs_create("user1", "/dir/d", 'd');
    fs_writeblock("user1", "/file", FS_MAXFILEBLOCKS, writedata);

    // delete some blocks
    fs_delete("user1", "/dir/dir10");
    fs_delete("user1", "/dir/dir100");
    fs_delete("user1", "/dir/dir200");
    fs_delete("user1", "/dir/dir529");
    fs_delete("user1", "/dir/dir430");
    fs_delete("user1", "/dir/dir280");
    fs_delete("user1", "/dir/dir281");
    // delete a whole direntry block
    fs_delete("user1", "/dir/dir48");
    fs_delete("user1", "/dir/dir49");
    fs_delete("user1", "/dir/dir50");
    fs_delete("user1", "/dir/dir51");
    fs_delete("user1", "/dir/dir52");
    fs_delete("user1", "/dir/dir53");
    fs_delete("user1", "/dir/dir54");
    fs_delete("user1", "/dir/dir55");
    

    fs_create("user1", "/dir/10000", 'd');
    fs_create("user1", "/dir/10", 'd');
    fs_create("user1", "/dir/a", 'd');
    fs_create("user1", "/dir/b", 'd');
    fs_create("user1", "/dir/c", 'd');
    fs_create("user1", "/dir/e", 'd');
    fs_create("user1", "/dir/f", 'd');
    fs_create("user1", "/dir/48", 'd');
    fs_create("user1", "/dir/53", 'd');
    fs_create("user1", "/dir/g", 'd');
    fs_create("user1", "/dir/h", 'd');
    fs_create("user1", "/dir/i", 'd');
    fs_create("user1", "/dir/j", 'd');
    fs_create("user1", "/dir/k", 'd');
    fs_create("user1", "/dir/l", 'd');
    // illegal
    fs_create("user1", "/dir/m", 'd');
    
}
