#include <iostream>
#include <cassert>
#include <cstdlib>
#include "fs_client.h"
#include "fs_server.h"

// testing delete by non-owner
// should fail
int main(int argc, char *argv[]) {
    char *server;
    int server_port;

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

    // error checking: invalid delete access by jonathan to sophie data
    status = fs_delete("jonathan", "/david");
    status++;
}