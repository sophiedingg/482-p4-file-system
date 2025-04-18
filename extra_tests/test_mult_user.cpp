#include <iostream>
#include <cassert>
#include <cstdlib>
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

    status = fs_create("user1", "/dir1", 'd');


    status = fs_create("user2", "/dir2", 'd');


    status = fs_create("user1", "/dir1/file", 'f');


    status = fs_create("user2", "/dir2/dir", 'd');


    status = fs_create("user3", "/dir3", 'd');


    status = fs_create("user2", "/dir2/file", 'f');


    status = fs_create("user3", "/dir3/dir3", 'd');


    status = fs_writeblock("user2", "/dir2/file", 0, writedata1);


    status = fs_writeblock("user1", "/dir1/file", 0, writedata);


    status = fs_create("user2", "/dir2/dir2", 'd');


    status = fs_create("user2", "/dir2/dir2/dir2", 'd');


    status = fs_create("user2", "/dir2/dir2/dir2/file", 'f');


    status = fs_writeblock("user2", "/dir2/dir2/dir2/file", 0, writedata);


    status = fs_readblock("user2",  "/dir2/dir2/dir2/file", 0, readdata);


    status = fs_readblock("user2",  "/dir2/file", 0, readdata);


    status = fs_writeblock("user2", "/dir2/dir2/dir2/file", 0, readdata);


    status = fs_writeblock("user1", "/dir1/file", 0, readdata);


    status = fs_create("user2", "/dir2/dir2/dir2/delete", 'f');


    status = fs_delete("user2", "/dir2/dir2/dir2/delete");


    status = fs_delete("user2", "/dir2/dir2/dir2/file");


    status = fs_delete("user2", "/dir2/dir2/dir2");


    status = fs_delete("user2", "/dir2/dir2");


    status = fs_delete("user2", "/dir2/dir");


    status = fs_delete("user2", "/dir2");
    status++;

}
