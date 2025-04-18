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

    const char *writedata = "We hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";

    // start create thread
    // should sleep
    std::thread create_thread(fs_create, "user1", "/read_dir/slow", 'f');

    // Increase likelihood that file server receives fs_create first
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // start write thread
    std::thread write_thread(fs_writeblock, "user1", "/file", 0, writedata);

    create_thread.join();   
    write_thread.join(); 
}