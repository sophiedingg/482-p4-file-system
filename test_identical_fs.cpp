#include <iostream>
#include <cassert>
#include <cstdlib>
#include "fs_client.h"

int main(int argc, char *argv[]) {
    char *server;
    int server_port;
    const char *writedata1 = "In this project, you will implement a multi-threaded network file server. Clients that use your file server will interact with it via network messages. This project will help you understand hierarchical file systems, socket programming, client-server systems, and network protocols, and it will give you experience building a substantial multi-threaded program with fine-grained locking. Your file server will provide a hierarchical file system. Files and directories stored on the file server are referred to by a full pathname, with / being the delimiting character. For example, the pathname /class/482/notes refers to a file notes that is stored in the directory /class/482. Pathnames must start with /, and they must not end with /. Directories store files and/or sub-directories; files store data. Each file and directory is owned by a particular user, except for the root directory /, which is owned by all users. Users may only access files and directories they own.";
    const char *writedata = "(Anand David Sophie) EECS Roomies most famous quote: We hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";
    char readdata[FS_BLOCKSIZE];
    int status;

    if (argc != 3) {
        std::cout << "error: usage: " << argv[0] << " <server> <serverPort>\n";
        exit(1);
    }
    server = argv[1];
    server_port = atoi(argv[2]);

    fs_clientinit(server, server_port);
    // 2 directories and 1 file under root
    status = fs_create("user1", "/dir1", 'd');

    status = fs_create("user2", "/dir1", 'd');

    status = fs_create("user1", "/file", 'f');

    status = fs_create("user2", "/file", 'f');

    status = fs_create("user1", "/dir2", 'd');

    status = fs_create("user2", "/dir2", 'd');


    // add 2 directories and 6 files under dir2
    status = fs_create("user1", "/dir2/dir1", 'd');

    status = fs_create("user2", "/dir2/dir1", 'd');

    status = fs_create("user1", "/dir2/file6", 'f');

    status = fs_create("user2", "/dir2/file6", 'f');

    status = fs_create("user1", "/dir2/file1", 'f');

    status = fs_create("user2", "/dir2/file1", 'f');

    status = fs_create("user1", "/dir2/file2", 'f');

    status = fs_create("user2", "/dir2/file2", 'f');

    status = fs_create("user1", "/dir2/file3", 'f');

    status = fs_create("user2", "/dir2/file3", 'f');

    status = fs_create("user1", "/dir2/dir2", 'd');

    status = fs_create("user2", "/dir2/dir2", 'd');

    status = fs_create("user1", "/dir2/file4", 'f');

    status = fs_create("user2", "/dir2/file4", 'f');

    status = fs_create("user1", "/dir2/file5", 'f');

    status = fs_create("user2", "/dir2/file5", 'f');


    // writedata to /dir2/file6 (diff data to diff users)
    status = fs_writeblock("user1", "/dir2/file6", 0, writedata);

    status = fs_writeblock("user2", "/dir2/file6", 0, writedata1);


    // read both users, overwrite user2 and re-read
    status = fs_readblock("user2",  "/dir2/file6", 0, readdata);

    status = fs_readblock("user1",  "/dir2/file6", 0, readdata);

    status = fs_writeblock("user2", "/dir2/file6", 0, writedata);

    status = fs_readblock("user2",  "/dir2/file6", 0, readdata);


    // delete /dir1 and /file
    status = fs_delete("user2", "/dir1");

    status = fs_delete("user1", "/dir1");

    status = fs_delete("user1", "/file");

    status = fs_delete("user2", "/file");


    // delete /dir2/file6 (non empty)
    status = fs_delete("user1", "/dir2/file6");

    status = fs_delete("user2", "/dir2/file6");
    status++;
}
