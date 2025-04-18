#include <iostream>
#include <cassert>
#include <cstdlib>
#include "fs_client.h"

// fills three blocks with direntries
// deletes the middle block direntry array
// adds one on to the end, creating a new block
int main(int argc, char *argv[]) {
    char *server;
    int server_port;

    const char *writedata = "190823759812y4659o94u3tqwadjsivnsdjufhg9udafehognaojfgbjisadbgfzcxbenq  ofl,1np9rfscpol mpofg3q8owan l314075t78qerwnvkomw[e0 tb42   9j #@%&$%&#$%^^@!@*#^$%WY@#WQ$^@%&436@#548(#4%*@&34lksi9dh0tq3krwgmsvoxdch9bndsfokmbihx9noqkrmeav9 hxcvnbovm z9xcf9mjq0jb9uheu9jfg0asdlzpvmjxcu9fq0wiavmkpsad9vbaugnokasnsf81209458691275901827509872390587129083579asidgoadfkmasod8ucvgsdn wevhr08uqfosdou hsa8gfasdfcoszjflmfojposfg;a,pscz,[lvzspidlas,dvpasdpfomasdpofmaspl,[;c,plvmskdmfvpa,pvl,z[lbmpkczmbpdkfm;,QP9IALKDVNBIOWGPO12K4MF;,QWMC9UHQKo9uwergokmsef9uheoignq3pfjea-0bsdprm1ioprhg1]]";
    char readdata[FS_BLOCKSIZE];
    int status;

    if (argc != 3) {
        std::cout << "error: usage: " << argv[0] << " <server> <serverPort>\n";
        exit(1);
    }
    server = argv[1];
    server_port = atoi(argv[2]);

    fs_clientinit(server, server_port);

    status = fs_writeblock("jonathan", "/larry/sophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavidsophiedavid", 0, writedata);
}