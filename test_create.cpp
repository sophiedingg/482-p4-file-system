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

    status = fs_create("user1", "/dir", 'd');

    status = fs_create("user1", "/dir/file", 'f');

    status = fs_create("user1", "/dir/anand", 'd');

    status = fs_create("user1", "/dir/anand/david", 'd');

    status = fs_create("user1", "/dir/anand/david/sophie", 'd');

    status = fs_create("user1", "/dir/anand/david/david", 'f');

    status = fs_create("user1", "/dir/anand/sophie", 'f');

    status = fs_writeblock("user1", "/dir/anand/david/david", 0, writedata);

    status = fs_writeblock("user1", "/dir/anand/sophie", 0, writedata1);

    status = fs_readblock("user1",  "/dir/anand/david/david", 0, readdata);

    status = fs_readblock("user1",  "/dir/anand/sophie", 0, readdata);

    status = fs_delete("user1", "/dir/anand/sophie");
}
