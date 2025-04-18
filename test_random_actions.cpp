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

    for(int i = 0; i < 100000; ++i){
        int random = rand() % 100;
        std::string string_path = "/" + std::to_string(i % 4) + "file" + std::to_string(i % 2);
        const char * pathname = string_path.c_str();
        std::string string_path2 = "/" + std::to_string(i % 5) + "dir" + std::to_string(i % 3);
        const char * pathname2 = string_path2.c_str();
        if(random < 25){
            if(random % 2 == 0){
                status = fs_create("user1",  pathname, 'f');
            }else{
                status = fs_create("user1",  pathname2, 'd');
            }
        }else if(random < 50){
            status = fs_readblock("user1", pathname, 1, readdata);
        }else if(random < 75){
            status = fs_writeblock("user1",  pathname, 0, writedata);
        }else{
            status = fs_delete("user1", pathname);
        }
    }
}
