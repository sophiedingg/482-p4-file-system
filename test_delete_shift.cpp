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

    // fill first block array of direntries in root (A)
    // block A holding the array should be block 2
    status = fs_create("user1", "/1", 'd');
    status = fs_create("user1", "/2", 'd');
    status = fs_create("user1", "/3", 'd');
    status = fs_create("user1", "/4", 'd');
    status = fs_create("user1", "/5", 'd');
    status = fs_create("user1", "/6", 'd');
    status = fs_create("user1", "/7", 'd');
    status = fs_create("user1", "/8", 'd');

    // fill second block B
    // should be block 11 holding array of direntries
    status = fs_create("user1", "/9", 'd');
    status = fs_create("user1", "/10", 'd');
    status = fs_create("user1", "/11", 'f');
    status = fs_create("user1", "/12", 'f');
    status = fs_create("user1", "/13", 'd');
    status = fs_create("user1", "/14", 'd');
    status = fs_create("user1", "/15", 'f');
    status = fs_create("user1", "/16", 'd');

    status = fs_writeblock("user1", "/11", 0, writedata);
    status = fs_writeblock("user1", "/12", 0, writedata1);

    // fill third block C
    status = fs_create("user1", "/17", 'd');
    
    // delete first block A
    // should free up block 2
    status = fs_delete("user1", "/1");
    status = fs_delete("user1", "/2");
    status = fs_delete("user1", "/3");
    status = fs_delete("user1", "/4");
    status = fs_delete("user1", "/5");
    status = fs_delete("user1", "/6");
    status = fs_delete("user1", "/7");
    status = fs_delete("user1", "/8");

    // create a new file in the block C
    status = fs_create("user1", "/18", 'd');

    // delete a file in the second block B
    status = fs_delete("user1", "/15");

    // try to read and write to files in block B
    status = fs_writeblock("user1", "/11", 0, writedata);
    status = fs_readblock("user1", "/12", 0, readdata);

    // create a file in block 2
    // should fill up block 2
    status = fs_create("user1", "/15again", 'd');

    // create a file in block 3
    status = fs_create("user1", "/19", 'd');
}
