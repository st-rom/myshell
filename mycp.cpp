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


bool file_copy(std::string inp, std::string outp, bool f){
    char ask = 'y';
    std::ifstream iiifile(outp);
    if (iiifile) {
        if(!f){
            std::cout << "Are you sure you want to rewrite " << outp << "?  Y[es]/N[o]/A[ll]/C[ancel] ";
            std::cin >> ask;
        }
    }
    if (tolower(ask) == 'y'){
        std::ifstream src(inp, std::ios::binary);
        std::ofstream dest(outp, std::ios::binary);
        dest << src.rdbuf();
    }
    else if(tolower(ask) == 'a'){
        f = true;
        std::ifstream src(inp, std::ios::binary);
        std::ofstream dest(outp, std::ios::binary);
        dest << src.rdbuf();
    }
    else if(tolower(ask) == 'c'){
        exit(EXIT_SUCCESS);
    }
    return f;
}


bool rec_copy(std::string inp, std::string outp, bool f){
    mkdir((outp).c_str(), ACCESSPERMS);
    char complete_filename[512];
    struct dirent **namelist;
    struct stat buf;
    int n = scandir(inp.c_str(), &namelist, 0, alphasort);
    int i;

    for ( i = 0; i < n; i++ )
    {
        char *file_name = namelist[i]->d_name;

        strcpy(complete_filename, "./");
        strcat(complete_filename, "/");
        strcat(complete_filename, file_name);
        if((dir_check((inp + '/' + file_name).c_str()) == 1) && (file_name[0] != '.')){
            rec_copy(inp + '/' + file_name, outp + '/' + file_name, f);
        }
        else if(file_name[0] != '.'){
            f = file_copy(inp + '/' + file_name, outp + '/' + file_name, f);
        }
    }
    return f;
}


int main(int argc, char *argv[]){
    std::vector<std::string> files;
    bool f = false;
    bool r = false;
    if(argc < 3){
        std::cout << "At least 2 arguments are necessary" << std::endl;
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < argc; ++i){
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0){
            std::cout << "mycp\nAt least 2 args files/dirs are required\n-f do not ask questions\n-R copy directories" << std::endl;
            exit(EXIT_SUCCESS);
        }
        else if (strcmp(argv[i], "-f") == 0 ){
            f = true;
        }
        else if (strcmp(argv[i], "-R") == 0 ){
            r = true;
        }
        else if (i != 0){
            files.push_back(std::string(argv[i]));
        }
    }
    char ask = 'y';
    if (files.size() < 2){
        std::cout << "At least 2 arguments files are necessary" << std::endl;
        exit(EXIT_FAILURE);
    }
    else if (files.size() == 2 && dir_check(files[0].c_str()) == 0 && dir_check(files[1].c_str()) == 0){
        std::ifstream src(files[0], std::ios::binary);
        std::ifstream ifile(files[1]);
        if (ifile) {
            if(!f){
                std::cout << "Are you sure you want to rewrite " << files[1] << "?  Y[es]/N[o]/A[ll]/C[ancel] ";
                std::cin >> ask;
            }
        }
        if (tolower(ask) == 'y'){
            std::ofstream dest(files[1], std::ios::binary);
            dest << src.rdbuf();
        }
            //CONTINUE(Done?)
        else if(tolower(ask) == 'a'){
            f = true;
            std::ofstream dest(files[1], std::ios::binary);
            dest << src.rdbuf();
        }
        else if(tolower(ask) == 'c'){
            exit(EXIT_SUCCESS);
        }
    }
    else if (dir_check(files[files.size() - 1].c_str()) == 0){
        std::cout << "Last argument isn't folder" << std::endl;
        return -1;
    }
    else{
        for(int i = 0; i < files.size() - 1; i++){
            ask = 'y';
            if (dir_check(files[i].c_str()) == 1 && !r){
                std::cout << "Cannot copy dir w/o spec arg" << std::endl;
            }
            else if (dir_check(files[i].c_str()) == 1){

                if (dir_check((files[files.size() - 1] + '/' + files[i]).c_str()) == 1) {
                    if(!f){
                        std::cout << "Are you sure you want to rewrite " << files[files.size() - 1] + '/' + files[i] << "?  Y[es]/N[o]/A[ll]/C[ancel] ";
                        std::cin >> ask;
                    }
                }
                if (tolower(ask) == 'y'){
                    f = rec_copy(files[i], files[files.size() - 1] + '/' + files[i], f);
                }
                else if(tolower(ask) == 'a'){
                    f = true;
                    f = rec_copy(files[i], files[files.size() - 1] + '/' + files[i], f);
                    //TODO done?
                }
                else if(tolower(ask) == 'c'){
                    exit(EXIT_SUCCESS);
                }
            }
            else{
                f = file_copy(files[i], files[files.size() - 1] + '/' + files[i], f);
            }
        }
    }
}