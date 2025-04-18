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

    // delele inputs

    // max path name
    status = fs_delete("sophie",  "/sophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavid");
    
    // max directory in path
    status = fs_delete("sophie",  "/sophieanandsophieanandsophieanandsophieanandsophieanandsophieanand");

    // max username
    status = fs_delete("sophiesophie",  "/sophie");

    // delete root 
    status = fs_delete("sophie",  "/");

    // delete without root
    status = fs_delete("sophie",  "sophie");

    // delete something that doesn't exist
    status = fs_delete("sophie",  "/david");

    // create a sophie directory
    status = fs_create("sophie",  "/sophie", 'd');
    status = fs_create("sophie",  "/sophie/anand", 'f');

    // delete directory that isn't empty
    status = fs_delete("sophie",  "/sophie");

    // real delete
    status = fs_create("sophie",  "/sophie/anand", 'f');

    // double delete
    status = fs_create("sophie",  "/sophie/anand", 'f');
}
