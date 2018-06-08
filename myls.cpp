#include <fstream>
#include <sys/param.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <iterator>
#include <map>
#include <fstream>
#include <time.h>
#ifndef BOOST_ALL_DYN_LINK
#   define BOOST_ALL_DYN_LINK
#endif
#include <boost/filesystem.hpp>

using boost::filesystem::path;
using boost::filesystem::last_write_time;


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


inline bool exists_test (const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}


int get_file_size(path filename) {
    struct stat _info;
    int fd = stat(filename.c_str(), &_info);
    return _info.st_size;
}


//bool by_ex(path& a, path& b){
//    std::string a_str = a.string();
//    size_t a_index = a_str.rfind('.');
//    std::string b_str = b.string();
//    size_t b_index = b_str.rfind('.');
//    if(std::string(a_str.begin() + index + 1, a_str.end()) > std::string(b_str.begin() + index + 1, b_str.end())){
//        return -1;
//    }
//    if(std::string(a_str.begin() + index + 1, a_str.end()) <std::string(b_str.begin() + index + 1, b_str.end())){
//        return 1;
//    }
//    return 0;
//}


bool by_dir(path a, path b){

    if(dir_check(a.c_str()) && !(dir_check(b.c_str()))){
        return 1;
    }
    else if(dir_check(b.c_str()) && !(dir_check(a.c_str()))){
        return 0;
    }
    std::cout << dir_check(a.c_str()) << ' ' << a.string().empty() << ' ' << a.string() << ' ' << b.string() << std::endl;
    return 1;
}


bool is_exefile(path &file)
{
    struct stat st;

    if (stat(file.c_str(), &st) < 0) {
        return false;
    }
    if ((st.st_mode & S_IEXEC) != 0) {
        return true;
    }
    return false;
}

bool is_link(path &file)
{
    struct stat st;

    if (stat(file.c_str(), &st) < 0) {
        return false;
    }
    if (S_ISLNK (st.st_mode)){
        return true;
    }
    return false;
}





bool is_chanel(path &file)
{
    struct stat st;

    if (stat(file.c_str(), &st) < 0) {
        return false;
    }
    if (S_ISFIFO (st.st_mode)){
        return true;
    }
    return false;
}

bool is_sock(path &file)
{
    struct stat st;

    if (stat(file.c_str(), &st) < 0) {
        return false;
    }
    if (S_ISSOCK (st.st_mode)){
        return true;
    }
    return false;
}

bool by_special(path& a, path& b){
    if(is_chanel(b)||is_exefile(b)||is_link(b)||is_sock(b)){
        return 0;
    }
    if(is_chanel(a)||is_exefile(a)||is_link(a)||is_sock(a)){
        return 1;
    }
    return 1;
}


std::vector<path> rec_through(std::string inp){
    char complete_filename[512];
    struct dirent **namelist;
    struct stat buf;
    int n = scandir(inp.c_str(), &namelist, 0, alphasort);
    int i;
    std::vector<path> toret;
    for ( i = 0; i < n; i++ )
    {
        char *file_name = namelist[i]->d_name;
        strcpy(complete_filename, "./");
        strcat(complete_filename, "/");
        strcat(complete_filename, file_name);
        if(exists_test(file_name) && (file_name[0] != '.')){
            std::cout << "/" << file_name << std::endl;
            toret.push_back(path{file_name});
        }
    }
    return toret;
}


int main(int argc, char** argv){
    bool flag_special = false;
    bool flag_reverse = false;
    char cwd[256];
    getcwd(cwd, sizeof(cwd));
    std::string mypath(cwd);
    char buffer[MAXPATHLEN];
    char *mpath = getcwd(buffer, MAXPATHLEN);
    mypath = mpath;
    for(int i = 1; i < argc; ++i){
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0){
            std::cout << "mycp\nAt least 2 args files/dirs are required\n-f do not ask questions\n-R copy directories" << std::endl;
            exit(EXIT_SUCCESS);
        }
        else if (std::string(argv[i])[0] != '-'){
            if(exists_test(std::string(argv[i])) && dir_check(argv[i])) {
                mypath = std::string(argv[i]);
            }
            else if (exists_test(std::string(argv[i]))){
                std::cout << argv[i] << std::endl;
                return 0;
            }
            else{
                std::cerr << "Path doesnt exist" << std::endl;
            }

        }
        //else if (strcmp(argv[i], "-f") == 0 ){
        //    f = true;
        //}
    }
    //rec_through(std::string(argv[1]), true);
    std::vector<path> toret = rec_through(mypath);
    std::cout << toret.size() << std::endl;
    const std::vector<path> vec = toret;
    sort(toret.begin(), toret.end(), [](path& a, path& b){return a.filename() < b.filename();});
    for(int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-U") == 0 ){
            toret = vec;
        }
        else if (strcmp(argv[i], "-S") == 0 ){
            sort(toret.begin(), toret.end(), [](path &a, path &b) { return get_file_size(a) > get_file_size(b); });
        }
        else if (strcmp(argv[i], "-t") == 0 ){
            sort(toret.begin(), toret.end(), [](path &a, path &b) { return last_write_time(a) > last_write_time(b); });
        }
        else if (strcmp(argv[i], "-N") == 0 ){
            sort(toret.begin(), toret.end(), [](path& a, path& b){return a.string() < b.string();});
        }
        else if (strcmp(argv[i], "-F") == 0 ) {
            flag_special = true;
        }
        else if (strcmp(argv[i], "-D") == 0 ) {
            std::cout << toret.size() << std::endl;
            sort(toret.begin(), toret.end(), by_dir);
        }
        else if (strcmp(argv[i], "-r") == 0 ){
            flag_reverse = true;
        }

//        else if(argv[i] == "-X"){
//            sort(toret.begin(), toret.end(), by_ex);
//        }

    }
    if(flag_reverse){
        for(int i =toret.size(); i>0 ; --i){
            if (flag_special) {
                if(dir_check(toret[i].c_str())){
                    std::cout<<"/"+toret[i].string() << std::endl;
                }
                else if(is_exefile(toret[i])){
                    std::cout<<"*"+toret[i].string() << std::endl;
                }
                else if(is_link(toret[i])){
                    std::cout<<"@"+toret[i].string() << std::endl;
                }
                else if(is_exefile(toret[i])){
                    std::cout<<"|"+toret[i].string() << std::endl;
                }
                else if(is_sock(toret[i])){
                    std::cout<<"="+toret[i].string() << std::endl;
                }
                else{
                    std::cout<<"?"+toret[i].string() << std::endl;
                }
            }
            else{
                std::cout<<toret[i].string() << std::endl;
            }
        }
    }
    else {
        for (int i = 0; i < toret.size(); ++i) {
            if (flag_special) {
                if(dir_check(toret[i].c_str())){
                    std::cout<<"/"+toret[i].string() << std::endl;
                }
                else if(is_exefile(toret[i])){
                    std::cout<<"*"+vec[i].string() << std::endl;
                }
                else if(is_link(toret[i])){
                    std::cout<<"@"+toret[i].string() << std::endl;
                }
                else if(is_exefile(toret[i])){
                    std::cout<<"|"+toret[i].string() << std::endl;
                }
                else if(is_sock(toret[i])){
                    std::cout<<"="+toret[i].string() << std::endl;
                }
                else{
                    std::cout<<"?"+toret[i].string() << std::endl;
                }
            }
            else{
                std::cout<<toret[i].string() << std::endl;
            }
        }
    }
    return 0;
}
