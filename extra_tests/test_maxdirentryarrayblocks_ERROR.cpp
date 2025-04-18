#include <iostream>
#include <cassert>
#include <cstdlib>
#include "fs_client.h"

// fill up root direntry array blocks to 1 over max
int main(int argc, char *argv[]) {
    char *server;
    int server_port;

    int status;
    static constexpr unsigned int FS_DIRENTRIES = 8;

    if (argc != 3) {
        std::cout << "error: usage: " << argv[0] << " <server> <serverPort>\n";
        exit(1);
    }
    server = argv[1];
    server_port = atoi(argv[2]);

    fs_clientinit(server, server_port);

    for (size_t i = 0; i < FS_MAXFILEBLOCKS; i++){
        for (size_t j = 0; j < FS_DIRENTRIES; j++){
            std::string string_path = "/" + std::to_string(i) + "file" + std::to_string(j);
            const char * pathname = string_path.c_str();
            status = fs_create("user1", pathname, 'f');
        }
    }

    status = fs_create("user1", "overflow", 'f');
    status++;
}
