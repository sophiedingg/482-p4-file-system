#include <iostream>
#include <cassert>
#include <cstdlib>
#include "fs_client.h"

// testing all operations
int main(int argc, char *argv[]) {
    char *server;
    int server_port;

    // data to be written to david and molly files
    const char *writedata_david = "This is David truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";

    int status = 1;

    if (argc != 3) {
        std::cout << "error: usage: " << argv[0] << " <server> <serverPort>\n";
        exit(1);
    }
    server = argv[1];
    server_port = atoi(argv[2]);

    fs_clientinit(server, server_port);

    // testing create on files and directories (basic)
    status = fs_create("sophie", "/david", 'f');

    status = fs_create("sophie", "/anand", 'd');

    status = fs_create("sophie", "/anand/molly", 'f');

    status = fs_create("sophie", "/anand/pritika", 'd');

    // error checking: write to directory
    status = fs_writeblock("sophie", "/anand", 0, writedata_david);
    status++;
}