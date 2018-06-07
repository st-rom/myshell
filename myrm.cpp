#include <string.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>


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


bool dir_rm(std::string inp, bool f){
    char complete_filename[512];
    struct dirent **namelist;
    struct stat buf;
    int n = scandir(inp.c_str(), &namelist, 0, alphasort);
    for (int i = 0; i < n; i++ )
    {
        char ask = 'y';
        char *file_name = namelist[i]->d_name;

        strcpy(complete_filename, "./");
        strcat(complete_filename, "/");
        strcat(complete_filename, file_name);
        if(file_name[0] != '.'){
            if (!f){
                std::cout << "Are you sure you want to delete " << inp + '/' + file_name << "?  Y[es]/N[o]/A[ll]/C[ancel] ";
                std::cin >> ask;
            }
            if(tolower(ask) == 'y'){
                if(dir_check((inp + '/' + file_name).c_str()) == 1){
                    f = dir_rm(inp + '/' + file_name, f);
                    if( remove((inp + '/' + file_name).c_str()) != 0 ){
                        perror( "Error deleting file" );
                    }
                }
                else if( remove((inp + '/' + file_name).c_str()) != 0 ){
                    perror( "Error deleting file" );
                }
            }
            else if(tolower(ask) == 'a'){
                f = true;
                if(dir_check((inp + '/' + file_name).c_str()) == 1){
                    f = dir_rm(inp + '/' + file_name, f);
                    if( remove((inp + '/' + file_name).c_str()) != 0 ){
                        perror( "Error deleting file" );
                    }
                }
                else if( remove((inp + '/' + file_name).c_str()) != 0 ){
                    perror( "Error deleting file" );
                }
            }
            else if(tolower(ask) == 'c'){
                exit(EXIT_SUCCESS);
            }
        }
    }
    return f;
}


int main(int argc, char *argv[]){
    std::vector<std::string> files;
    bool r = false;
    bool f = false;
    if (argc == 1){
        std::cout << "At least 1 arg is required" << std::endl;
        exit(EXIT_FAILURE);
    }
    for(int i = 1; i < argc; ++i){
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0){
            std::cout << "myrm\nRemoves directory\n-f do not ask questions\n-R removes whole directories"<< std::endl;
            exit(EXIT_SUCCESS);
        }
        else if (strcmp(argv[i], "-f") == 0 ){
            f = true;
        }
        else if (strcmp(argv[i], "-R") == 0 ){
            r = true;
        }
        else{
            files.push_back(std::string(argv[i]));
        }
    }
    int er = 0;
    char ask = 'y';
    for(int i = 0; i < files.size(); ++i){
        if (!f){
            std::cout << "Are you sure you want to delete " << files[i] << "?  Y[es]/N[o]/A[ll]/C[ancel] ";
            std::cin >> ask;
        }
        if(tolower(ask) == 'y'){
            if(dir_check(files[i].c_str()) == 1 && r){
                f = dir_rm(files[i], f);
                if( remove((files[i]).c_str()) != 0 ){
                    perror( "Error deleting file" );
                }
            }
            else if( remove(files[i].c_str()) != 0 ){
                perror( "Error deleting file" );
                er = 1;
            }
        }
        else if(tolower(ask) == 'a'){
            f = true;
            if(dir_check(files[i].c_str()) == 1 && r){
                f = dir_rm(files[i], f);
                if( remove((files[i]).c_str()) != 0 ){
                    perror( "Error deleting file" );
                }
            }
            else if( remove(files[i].c_str()) != 0 ){
                perror( "Error deleting file" );
                er = 1;
            }
        }
        else if(tolower(ask) == 'c'){
            exit(EXIT_SUCCESS);
        }
    }
    if (er == 1)
        return -1;
    return 0;
}