#include <iostream>
#include <cassert>
#include <cstdlib>
#include "fs_client.h"

// fills three blocks with direntries
// deletes the middle block direntry array
// adds one on to the end, creating a new block
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

    // fill first block
    status = fs_create("user1", "/1", 'd');
    status = fs_create("user1", "/2", 'd');
    status = fs_create("user1", "/3", 'd');
    status = fs_create("user1", "/4", 'd');
    status = fs_create("user1", "/5", 'd');
    status = fs_create("user1", "/6", 'd');
    status = fs_create("user1", "/7", 'd');
    status = fs_create("user1", "/8", 'd');

    // fill second block
    status = fs_create("user1", "/9", 'd');
    status = fs_create("user1", "/10", 'd');
    status = fs_create("user1", "/11", 'd');
    status = fs_create("user1", "/12", 'd');
    status = fs_create("user1", "/13", 'd');
    status = fs_create("user1", "/14", 'd');
    status = fs_create("user1", "/15", 'd');
    status = fs_create("user1", "/16", 'd');

    // fill third block
    status = fs_create("user1", "/17", 'd');
    status = fs_create("user1", "/18", 'd');
    status = fs_create("user1", "/19", 'd');
    status = fs_create("user1", "/20", 'd');
    status = fs_create("user1", "/21", 'd');
    status = fs_create("user1", "/22", 'd');
    status = fs_create("user1", "/23", 'd');
    status = fs_create("user1", "/24", 'd');

    // delete second block
    status = fs_delete("user1", "/9");
    status = fs_delete("user1", "/10");
    status = fs_delete("user1", "/11");
    status = fs_delete("user1", "/12");
    status = fs_delete("user1", "/13");
    status = fs_delete("user1", "/14");
    status = fs_delete("user1", "/15");
    status = fs_delete("user1", "/16");

    // add one more onto new block
    status = fs_create("user1", "/25", 'd');
}
