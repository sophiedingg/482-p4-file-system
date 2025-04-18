#include <iostream>
#include <cassert>
#include <cstdlib>
#include "fs_client.h"
#include "fs_server.h"

// testing all operations
int all_operations() {

    // data to be written to david and molly files
    const char *writedata_david = "This is David truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";
    const char *writedata_molly = "We hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";

    // data to be read from david and molly files
    char readdata_david[FS_BLOCKSIZE];
    char readdata_molly[FS_BLOCKSIZE];

    int status = 1;

    // testing create on files and directories (basic)
    status = fs_create("sophie", "/david", 'f');
    std::cout << "1\n";


    status = fs_create("sophie", "/anand", 'd');
    std::cout << "2\n";


    status = fs_create("sophie", "/anand/molly", 'f');
    std::cout << "3\n";


    status = fs_create("sophie", "/anand/pritika", 'd');
    std::cout << "4\n";


    // testing write
    // david file gets a full block
    // molly file gets part of a block
    status = fs_writeblock("sophie", "/david", 0, writedata_david);
    std::cout << "5\n";


    status = fs_writeblock("sophie", "/anand/molly", 0, writedata_molly);
    std::cout << "6\n";


    // testing read after writing
    status = fs_readblock("sophie", "/david", 0, readdata_david);
    std::cout << "7\n";


    status = fs_readblock("sophie", "/anand/molly", 0, readdata_molly);
    std::cout << "8\n";


    // testing write to existing block
    writedata_david = "David block 0 file change";
    writedata_molly = "Molly block 0 file change";

    status = fs_writeblock("sophie", "/david", 0, writedata_david);
    std::cout << "9\n";


    status = fs_writeblock("sophie", "/anand/molly", 0, writedata_molly);
    std::cout << "10\n";


    // testing read after writing
    status = fs_readblock("sophie", "/david", 0, readdata_david);
    std::cout << "11\n";


    status = fs_readblock("sophie", "/anand/molly", 0, readdata_molly);
    std::cout << "12\n";


    // testing write to a new block in existing file
    writedata_david = "David block 1";
    writedata_molly = "Molly block 1";

    status = fs_writeblock("sophie", "/david", 1, writedata_david);
    std::cout << "13\n";


    status = fs_writeblock("sophie", "/anand/molly", 1, writedata_molly);
    std::cout << "14\n";


    // testing read after writing
    status = fs_readblock("sophie", "/david", 1, readdata_david);
    std::cout << "15\n";


    status = fs_readblock("sophie", "/anand/molly", 1, readdata_molly);
    std::cout << "16\n";


    // fill up direntry array in root
    // Owned by sophie: avery (d), emma (f)
    // josh (d), ryan (f), claire (d), colleen (f)
    
    status = fs_create("sophie", "/avery", 'd');
    std::cout << "17\n";


    status = fs_create("sophie", "/emma", 'f');
    std::cout << "18\n";


    status = fs_create("jonathan", "/josh", 'd');
    std::cout << "19\n";


    status = fs_create("jonathan", "/ryan", 'f');
    std::cout << "20\n";


    status = fs_create("jonathan", "/claire", 'd');
    std::cout << "21\n";


    status = fs_create("jonathan", "/colleen", 'f');
    std::cout << "22\n";


    // create new direntry (f)
    // current array of direntries full
    // need to allocate a new array of direntries in root blocks array

    status = fs_create("sophie", "/klaus", 'f');


    char readdata_klaus[FS_BLOCKSIZE];
    const char *writedata_klaus = "Klaus: The Sixth Member";

    // test write
    status = fs_writeblock("sophie", "/klaus", 0, writedata_klaus);


    // test write to existing block
    writedata_klaus = "Klaus block 0 change";

    status = fs_writeblock("sophie", "/klaus", 0, writedata_klaus);


    // test read block 0
    status = fs_readblock("sophie", "/klaus", 0, readdata_klaus);


    // test write to new block in file
    writedata_klaus = "Klaus block 1";

    status = fs_writeblock("sophie", "/klaus", 1, writedata_klaus);


    // test read block 1
    status = fs_readblock("sophie", "/klaus", 1, readdata_klaus);


    // deleting only direntry in a direntry array
    // should shrink blocks array
    status = fs_delete("sophie", "/klaus");


    // same test but with a directory
    // create new direntry (d)
    // current array of direntries full
    // need to allocate a new array of direntries in root blocks array
    status = fs_create("sophie", "/chris", 'd');


    status = fs_create("sophie", "/chris/hannah", 'f');


    status = fs_create("sophie", "/chris/maya", 'd');


    // test delete file and directory (basic)
    status = fs_delete("sophie", "/chris/maya");


    status = fs_delete("sophie", "/chris/hannah");


    // delete chris
    status = fs_delete("sophie", "/chris");


    // test delete file and directory (basic)
    status = fs_delete("sophie", "/avery");


    status = fs_delete("sophie", "/emma");


    // create files in the recently freed direntries
    // refill all direntries in first block
    status = fs_create("anand", "/ishaan", 'f');


    status = fs_create("anand", "/collin", 'd');


    // create more direntries in a new array of direntries
    status = fs_create("sophie", "/riley", 'f');


    status = fs_create("sophie", "/tommy", 'd');


    // delete all direntries in first direntry array
    // will have to resize blocks array and shift
    status = fs_delete("sophie", "/anand/molly");


    status = fs_delete("sophie", "/anand/pritika");


    status = fs_delete("sophie", "/david");

    status = fs_delete("sophie", "/anand");

    status = fs_delete("anand", "/ishaan");

    status = fs_delete("anand", "/collin");

    status = fs_delete("jonathan", "/josh");

    status = fs_delete("jonathan", "/ryan");

    status = fs_delete("jonathan", "/claire");

    status = fs_delete("jonathan", "/colleen");
}

int main(int argc, char *argv[]){

    if (argc != 3) {
        std::cout << "error: usage: " << argv[0] << " <server> <serverPort>\n";
        exit(1);
    }
    char *server = argv[1];
    int server_port = atoi(argv[2]);

    fs_clientinit(server, server_port);
    all_operations();
    all_operations();
}