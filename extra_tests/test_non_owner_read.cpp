#include <iostream>
#include <cassert>
#include <cstdlib>
#include "fs_client.h"
#include "fs_server.h"

// testing read by non-owner
// should fail
int main(int argc, char *argv[]) {
    char *server;
    int server_port;

    // data to be written to david and molly files
    const char *writedata_david = "This is David truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";

    // data to be read from david and molly files
    char readdata_david[FS_BLOCKSIZE];

    int status = 1;

    if (argc != 3) {
        std::cout << "error: usage: " << argv[0] << " <server> <serverPort>\n";
        exit(1);
    }
    server = argv[1];
    server_port = atoi(argv[2]);

    fs_clientinit(server, server_port);

    // testing create
    status = fs_create("sophie", "/david", 'f');
    std::cout << "1\n";
    assert(!status);

    // testing write
    // david file gets a full block
    status = fs_writeblock("sophie", "/david", 0, writedata_david);
    std::cout << "2\n";
    assert(!status);

    // error checking: invalid read access by jonathan to sophie data
    status = fs_readblock("jonathan", "/david", 0, readdata_david);
    assert(!status);
}