#include <string.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
int myer = 0;


int dir_check(const char *path)
{
    struct stat info;
    if(stat( path, &info ) != 0)
        return 0;
    else if(info.st_mode & S_IFDIR)
        return 1;
    else
        return 0;
}


int main(int argc, char *argv[]){
    bool p = false;
    std::vector<std::string> myargs;
    if(argc == 1){
        std::cout << "No name for directory. For more info use agument [-h|--help]" << std::endl;
        myer = 7;
        return 1;
    }
    for(int i = 1; i < argc; ++i){
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0){
            std::cout << "Creates directory\n-p create all dirs necessary even if dir exists" << std::endl;
            myer = 0;
            return 0;
        }
        else if (strcmp(argv[i], "-p") == 0 ){
            p = true;
        }
        else {
            myargs.push_back(std::string(argv[i]));
        }
    }
    if(p){
        mkdir(myargs[myargs.size() - 1].c_str(), ACCESSPERMS);
        myer = 0;
        return 0;
    }
    else{
        if (dir_check(myargs[myargs.size() - 1].c_str()) == 0){
            mkdir(myargs[myargs.size() - 1].c_str(), ACCESSPERMS);
            myer = 0;
            return 0;
        }
        else{
            std::cout << "Directory already exist" << std::endl;
            myer = 2;
            return 1;
        }
    }
}
