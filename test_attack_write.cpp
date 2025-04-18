#include <iostream>
#include <cassert>
#include <cstdlib>
#include "fs_client.h"

int main(int argc, char *argv[]) {
    char *server;
    int server_port;

    const char *writedata = "We hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";
    const char *writedata1 = "Anand David Sophie = EECS Roomies";
    const char *writedata2 = "Anand David Sophie/0 = EECS Roomies";
    char readdata[FS_BLOCKSIZE];
    int status;

    if (argc != 3) {
        std::cout << "error: usage: " << argv[0] << " <server> <serverPort>\n";
        exit(1);
    }
    server = argv[1];
    server_port = atoi(argv[2]);

    fs_clientinit(server, server_port);

    // create a sophie file
    status = fs_create("sophie", "/sophie", 'f');

    // Attacks with write
    // normal writes
    status = fs_writeblock("sophie", "/sophie", 0, writedata);
    status = fs_writeblock("sophie", "/sophie", 1, writedata1);
    status = fs_writeblock("sophie", "/sophie", 2, writedata2);

    // try to read data with a null character in it
    status = fs_readblock("sophie", "/sophie", 2, readdata);

    // write root
    status = fs_writeblock("user1",  "/", 0, writedata);

    // write without root
    status = fs_writeblock("user1",  "sophie", 0, writedata);
    
    // write directory
    status = fs_create("user1",  "/dir", 'd');
    status = fs_create("user1",  "/dir/anand", 'd');
    status = fs_create("user1",  "/dir/anand/sophie", 'f');
    status = fs_writeblock("user1",  "/dir", 0, writedata);

    // double slash in pathname
    status = fs_writeblock("user1",  "/dir/anand//sophie", 0, writedata);

    // write file not owned by user
    status = fs_writeblock("user1",  "/sophie", 0, writedata);

    // write end of a file blocks
    status = fs_writeblock("sophie",  "/sophie", 3, writedata);

    // write past end of file blocks
    status = fs_writeblock("sophie",  "/sophie", 5, writedata);

    // write entire file
    for (size_t i = 0; i < 124; ++i) {
        status = fs_writeblock("sophie",  "/sophie", i, writedata);
    }

    // write past end of file
    status = fs_writeblock("sophie",  "/sophie", 124, writedata);

    // write inputs
    // write at max file block
    status = fs_writeblock("sophie",  "/sophie", 124, writedata);

    // write at negative
    status = fs_writeblock("sophie",  "/sophie", -1, writedata);

    // max path name
    status = fs_writeblock("sophie",  "/sophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavid", 0, writedata);
    
    // max directory in path
    status = fs_writeblock("sophie",  "/sophieanandsophieanandsophieanandsophieanandsophieanandsophieanand", 0, writedata);

    // max username
    status = fs_writeblock("sophiesophie",  "/sophie", 0, writedata);

    // delete the file and try to write from it
    status = fs_delete("sophie", "/sophie");
    status = fs_writeblock("sophie",  "/sophie", 0, writedata);

    // read file that doesn't exist
    status = fs_writeblock("sophie",  "/anand", 0, writedata);
}
