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

    // get really long traversal
    fs_create("user1", "/dir/dir", 'd');
    fs_create("user1", "/dir/dir/dir", 'd');
    fs_create("user1", "/dir/dir/dir/dir", 'd');
    fs_create("user1", "/dir/dir/dir/dir/dir", 'd');
    fs_create("user1", "/dir/dir/dir/dir/dir/dir", 'd');
    fs_create("user1", "/dir/dir/dir/dir/dir/dir/dir", 'd');
    fs_create("user1", "/dir/dir/dir/dir/dir/dir/dir/dir", 'd');
    fs_create("user1", "/dir/dir/dir/dir/dir/dir/dir/dir/dir", 'd');
    fs_create("user1", "/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir", 'd');
    fs_create("user1", "/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir", 'd');
    fs_create("user1", "/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir", 'd');
    fs_create("user1", "/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir", 'd');
    fs_create("user1", "/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir", 'd');
    // create two files
    fs_create("user1", "/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/file", 'f');
    fs_create("user1", "/dir/file", 'f');

    // write to far away one, should be able to write to close one concurrently
    fs_writeblock("user1", "/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/file", 0, writedata);
    fs_writeblock("user1", "/dir/file", 0, writedata);
    // same thing but reads
    fs_readblock("user1", "/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/file", 0, readdata);
    fs_readblock("user1", "/dir/file", 0, readdata);
    // creates
    fs_create("user1", "/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/file2", 'f');
    fs_create("user1", "/dir/file2", 'f');
    // deletes
    fs_delete("user1", "/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/file2");
    fs_delete("user1", "/dir/file2");

    // write and read at the same time
    fs_writeblock("user1", "/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/file", 1, writedata);
    fs_readblock("user1", "/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/file", 1, readdata);

    // read then write (error maybe)
    fs_readblock("user1", "/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/file", 2, readdata);
    fs_writeblock("user1", "/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/file", 2, writedata);

    // this better not work
    fs_delete("user1", "/dir/dir/dir/dir/dir/dir/dir");

    // create delete
    fs_create("user1", "/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/file2", 'f');
    fs_delete("user1", "/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/file2");

    // delete create
    fs_delete("user1", "/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/file2");
    fs_create("user1", "/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/dir/file2", 'f');
}