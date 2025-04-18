#include <iostream>
#include <cassert>
#include <cstdlib>
#include "fs_client.h"

int main(int argc, char *argv[]) {
    char *server;
    int server_port;

    const char *writedata = "We hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";
    char readdata[FS_BLOCKSIZE];

    if (argc != 3) {
        std::cout << "error: usage: " << argv[0] << " <server> <serverPort>\n";
        exit(1);
    }
    server = argv[1];
    server_port = atoi(argv[2]);

    fs_clientinit(server, server_port);

    fs_writeblock("user1", "/", 0, writedata);
    fs_create("user1", "/", 'd');
    fs_readblock("user1", "/", 0, readdata);
    fs_create("user1", "/dir/", 'd');
    fs_create("user1", "/er rr", 'f');
    // /f
    fs_create("user1", "/f", 'f');
    fs_create("user1", "/f/d", 'd');
    fs_create("user1", "/d/d", 'd');
    // /d
    fs_create("user1", "/d", 'd');
    fs_writeblock("user1", "/d", 0, writedata);
    fs_readblock("user1", "/d", 0, readdata);
    // /d/f
    fs_create("user1", "/d/f", 'f');
    fs_readblock("user1", "/d/f", 0, readdata);
    fs_writeblock("user1", "/d/f", 0, writedata);
    fs_create("user1", "/d/f/d", 'd');
    fs_create("user1", "/d/f/f", 'f');
    fs_delete("user1", "/d/f/f");
    // /d/f -> /d/d
    fs_delete("user1", "/d/f");
    fs_create("user1", "/d/d", 'd');
    fs_delete("user1", "/d");
    fs_delete("user2", "/d/d");
    fs_writeblock("user1", "/d/d", 0, writedata);
    fs_readblock("user1", "/d/d", 0, readdata);
    fs_create("user2", "/d/d/f", 'f');
    // /d/d/f
    fs_create("user1", "/d/d/f", 'f');
    // /d/f  x /d/d/f
    fs_create("user1", "/d/f", 'f');
    fs_delete("user3", "/d");
    fs_delete("user3", "/");
    fs_delete("user1", "/d/d/d");
    fs_delete("user1", "/d/d/f");
    // /d/f1 to f20
    fs_create("user1", "/d/f1", 'f');
    fs_create("user1", "/d/f2", 'f');
    fs_create("user1", "/d/f3", 'f');
    fs_create("user1", "/d/f4", 'f');
    fs_create("user1", "/d/f5", 'f');
    fs_create("user1", "/d/f6", 'f');
    fs_create("user1", "/d/f7", 'f');
    fs_create("user1", "/d/f8", 'f');
    fs_create("user1", "/d/f9", 'f');
    fs_create("user1", "/d/f10", 'f');
    fs_create("user1", "/d/f11", 'f');
    fs_create("user1", "/d/f12", 'f');
    fs_create("user1", "/d/f13", 'f');
    fs_create("user1", "/d/f14", 'f');
    fs_create("user1", "/d/f15", 'f');
    fs_create("user1", "/d/f16", 'f');
    fs_create("user1", "/d/f17", 'f');
    fs_create("user1", "/d/f18", 'f');
    fs_create("user1", "/d/f19", 'f');
    fs_create("user1", "/d/f20", 'f');

    fs_writeblock("user1", "/d/f13", 0, writedata);
    fs_readblock("user1", "/d/f13", 0, readdata);
    fs_readblock("user1", "/d/f12", 0, readdata);

    fs_create("user2", "/d", 'd');
    fs_create("user2", "/d/f12", 'd');
    fs_writeblock("user2", "/d/f12", 0, writedata);
    fs_readblock("user1", "/d/f12", 0, readdata);
    fs_readblock("user2", "/d/f12", 0, readdata);
    fs_writeblock("user1", "/d/f12", 0, writedata);    
    fs_readblock("user1", "/d/f12", 0, readdata);
    fs_readblock("user2", "/d/f12", 0, readdata);
    fs_delete("user2", "/d/f12");
    fs_delete("user1\0", "/d/f12");
    fs_readblock("user1", "/d/f12", 0, readdata);
    fs_readblock("user2", "/d/f12", 0, readdata);
}
