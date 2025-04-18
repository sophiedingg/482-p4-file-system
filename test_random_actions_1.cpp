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

    status = fs_create("jonathan", "/sophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavid/neil/david", 's');
    
    status = fs_delete("jonathan", "/chen");
    
    status = fs_create("jonathan", "/larry/sophieanandsophieanandsophieanandsophieanandsophieanandsophieanand/sophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavid", 'd');
    
    status = fs_delete("jonathan", "/anand/larry/neil");

    status = fs_delete("sophie", "/peter");

    status = fs_readblock("jonathan",  "/pritika", 3, readdata);

    status = fs_create("jonathan", "/chen", 's');

    status = fs_readblock("longusername",  "/neil/pritika/neil", 0, readdata);

    status = fs_create("sophie", "/molly/molly", 'd');

    status = fs_create("jonathan", "/neil/sophieanandsophieanandsophieanandsophieanandsophieanandsophieanand/sophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavid", 'f');

    status = fs_delete("longusername", "/sophieanandsophieanandsophieanandsophieanandsophieanandsophieanand/larry/anand");

    status = fs_create("longusername", "/peter/chen", 'f');

    status = fs_create("sophie", "/sophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavid/justin/peter", 'f');
    
    status = fs_writeblock("longusername", "/larry/molly/sophieanandsophieanandsophieanandsophieanandsophieanandsophieanand", 4294967295, writedata);

    status = fs_readblock("longusername", "/peter/molly", 1, readdata);

    status = fs_create("longusername", "", 'd');

    status = fs_create("jonathan", "/anand/neil", 's');

    status = fs_readblock("sophie", "/larry", 0, readdata);

    status = fs_writeblock("longusername", "/sophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavid/pritika/sophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavid", 2, writedata1);
    
    status = fs_create("longusername", "/sophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavid", 's');

    status = fs_create("sophie", "/neil/sophieanandsophieanandsophieanandsophieanandsophieanandsophieanand/sophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavid", 's');

    status = fs_writeblock("longusername", "/david/pritika/peter", 4294967295, writedata);

    status = fs_readblock("longusername", "/peter/larry/sophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavid", 2, readdata);

    status = fs_writeblock("jonathan", "/justin/sophieanandsophieanandsophieanandsophieanandsophieanandsophieanand", 3, writedata);
 
    status = fs_create("jonathan", "/anand/neil/larry", 's');

    status = fs_delete("longusername", "");

    status = fs_delete("longusername", "/pritika/pritika/sophieanandsophieanandsophieanandsophieanandsophieanandsophieanand");

    status = fs_create("sophie", "/larry", 's');

    status = fs_readblock("jonathan", "/sophieanandsophieanandsophieanandsophieanandsophieanandsophieanand", 2, readdata);

    status = fs_create("jonathan", "/larry/pritika", 'd');

    status = fs_delete("sophie", "/anand");

    status = fs_create("jonathan", "/peter/chen", 's');

    status = fs_writeblock("jonathan", "/david/larry", 3, writedata1);

    status = fs_create("sophie", "/larry/pritika", 'f');

    status = fs_create("jonathan", "/justin/anand/sophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavid", 'f');

    status = fs_writeblock("jonathan", "/anand/pritika", 0, writedata);
    
    status = fs_writeblock("jonathan", "/larry/sophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavid", 0, writedata1);
}
