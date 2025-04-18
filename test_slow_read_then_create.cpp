#include <iostream>
#include <cassert>
#include <cstdlib>
#include <boost/regex.hpp>
#include "fs_client.h"
#include <algorithm>
#include <arpa/inet.h>
#include <array>
#include <queue>
#include <string>
#include <set>
#include <map>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <thread>
#include <chrono>
#include <unistd.h>

#include "fs_server.h"

using std::cout, std::string, std::pair;

// slow read
// then create
// create should happen first
// add sleep code after traverse_helper in fs_read in server.cpp

//    if (path.back() == "slow") {
//        // simulate slow disk IO with a sleep!
//        std::cout << "fs_read sleeping..." << std::endl;
//        boost::this_thread::sleep_for(boost::chrono::seconds(10));
//        std::cout << "fs_read woke up" << std::endl;
//    }

int main(int argc, char *argv[]) {
    char *server;
    int server_port;

    server = argv[1];
    server_port = atoi(argv[2]);

    fs_clientinit(server, server_port);
    char readdata[FS_BLOCKSIZE];

    // start reading thread
    std::thread read_thread(fs_readblock, "user1", "/read_dir/slow", 0, readdata);

    // Increase likelihood that file server receives fs_create first
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // start create thread
    std::thread create_thread(fs_create, "user1", "/file2", 'f');

    read_thread.join();
    create_thread.join();    
}