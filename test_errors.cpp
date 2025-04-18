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

    // bad delete root
    fs_delete("user1", "/");

    // good
    fs_create("user1", "/dir", 'd');

    // good
    fs_create("user2", "/dir2", 'd');

    // good
    fs_create("user2", "/dir2/file", 'f');

    // bad diff owner deleting
    fs_delete("user2", "/dir");

    // bad creating already existing dir
    fs_create("user2", "/dir", 'd');

    // bad creating already existing dir
    fs_create("user1", "/dir", 'd');

    // bad creating already existing dir but type of file
    fs_create("user1", "/dir", 'f');

    // bad creating off of non owned dir
    fs_create("user2", "/dir/file", 'f');

    // good
    fs_create("user1", "/dir/file", 'f');

    // bad diff owner deleting
    fs_delete("user2", "/dir/file");

    // bad non owner write
    fs_writeblock("user2", "/dir/file", 0, writedata);

    // good
    fs_writeblock("user1", "/dir/file", 0, writedata);

    // bad writing to negative
    fs_writeblock("user1", "/dir/file", -1, writedata);

    // bad diff owner deleting
    fs_delete("user2", "/dir/file");

    // good
    fs_delete("user1", "/dir/file");

    // good
    fs_create("user1", "/dir/file", 'f');

    // bad reading from non write
    fs_readblock("user1", "/dir/file", 0, readdata);

    // good
    fs_writeblock("user1", "/dir/file", 0, writedata);

    // bad non owner read
    fs_readblock("user2", "/dir/file", 0, readdata);

    // good
    fs_readblock("user1", "/dir/file", 0, readdata);

    // bad
    fs_readblock("user1", "/dir/file", 1, readdata);

    // bad
    fs_writeblock("user1", "/dir/file", 4, writedata);

    // bad
    fs_writeblock("user2", "/dir/file", 1, writedata);

    // good
    fs_writeblock("user1", "/dir/file", 0, writedata);

    // bad
    fs_writeblock("user1", "/dir/file", FS_MAXFILEBLOCKS, writedata);

    // good
    fs_writeblock("user1", "/dir/file", FS_MAXFILEBLOCKS-1, writedata);

    // bad
    fs_delete("user2", "/dir/file");

    // bad
    fs_delete("user1", "/dir");

    // good
    fs_delete("user1", "/dir/file");
}