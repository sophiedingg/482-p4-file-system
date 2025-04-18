#include <iostream>
#include <cassert>
#include <cstdlib>
#include "fs_client.h"

// testing operations that can happen concurrently
// should be more useful after we implement threads
int main(int argc, char *argv[]) {
    char *server;
    int server_port;

    const char *writedata = "We hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";

    char readdata[FS_BLOCKSIZE];
    int status;

    if (argc != 3) {
        std::cout << "error: usage: " << argv[0] << " <server> <serverPort>\n";
        exit(1);
    }
    server = argv[1];
    server_port = atoi(argv[2]);

    fs_clientinit(server, server_port);

    status = fs_create("user1", "/file", 'f');

    status = fs_create("user1", "/read_dir", 'd');

    status = fs_create("user1", "/read_dir/file", 'f');

    status = fs_writeblock("user1", "/read_dir/file", 0, writedata);

    status = fs_create("user1", "/write_dir", 'd');

    status = fs_create("user1", "/write_dir/file", 'f');

    // // concurrent read and create
    status = fs_readblock("user1", "/read_dir/file", 0, readdata);

    status = fs_create("user1", "/file2", 'f');

    status = fs_writeblock("user1", "/write_dir/file", 0, writedata);

    // concurrent write and create
    status = fs_create("user1", "/read_dir/file2", 'f');

    status = fs_writeblock("user1", "/file", 0, writedata);

    // should be a concurrent create
    status = fs_create("user1", "/write_dir/file2", 'f');

    // concurrent read and write
    status = fs_writeblock("user1", "/write_dir/file2", 0, writedata);

    status = fs_readblock("user1", "/read_dir/file", 0, readdata);

    // should be a concurrent read
    status = fs_readblock("user1", "/write_dir/file", 0, readdata);

    // concurrent read and delete

    status = fs_delete("user1", "/read_dir/file2");
    
    status = fs_readblock("user1", "/file", 0, readdata); 

    // should be a concurrent delete
    status = fs_delete("user1", "/write_dir/file2");

    // // concurrent write and delete
    status = fs_writeblock("user1", "/write_dir/file", 0, writedata);

    // should be a concurrent write
    status = fs_writeblock("user1", "/read_dir/file", 0, writedata);

    status = fs_delete("user1", "/file2");

    // concurrent create and delete
    status = fs_create("user1", "/write_dir/file3", 'f');

    status = fs_delete("user1", "/file");
    status++;
}
