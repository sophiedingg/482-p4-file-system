#include <iostream>
#include <cassert>
#include <cstdlib>
#include "fs_client.h"

// fill up root direntry array blocks to max
int main(int argc, char *argv[]) {
    char *server;
    int server_port;

    const char *writedata = "We hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";
    char readdata[FS_BLOCKSIZE];
    int status;
    static constexpr unsigned int FS_DIRENTRIES = 8;

    if (argc != 3) {
        std::cout << "error: usage: " << argv[0] << " <server> <serverPort>\n";
        exit(1);
    }
    server = argv[1];
    server_port = atoi(argv[2]);

    fs_clientinit(server, server_port);

    // for (size_t i = 0; i < FS_MAXFILEBLOCKS; i++){
    //     for (size_t j = 0; j < FS_DIRENTRIES; j++){
    //         std::string string_path = "/" + std::to_string(i) + "file" + std::to_string(j);
    //         const char * pathname = string_path.c_str();
    //         status = fs_create("user1", pathname, 'f');
    //         assert(!status);
    //     }
    // }
    status = fs_create("user1", "/a", 'f');
    status = fs_delete("user1", "/1file7");
    status = fs_create("user1", "/a", 'f');
}
