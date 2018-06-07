#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>
#include <errno.h>

const char INSTRUCTION[] = R"(Usage:
mycat [-h|--help] [-A] <file1> <file2> ... <fileN>
-A -- replace non-printable characters by their code:\xAB,;
-h | --help -- print instructions and exit
)";
const char COPYRIGHT[] = R"(mycat.cpp:
This program shows the contents of files.
Arguments:
<name of file> files to work with
-A invisible symbols will be printed in hexadecimal code.
-h or --help for more help.
)";
const char ERRORMESSAGE[] = R"(Unable to open file.)
Make sure you typed correct file name and path to it(if needed)
For more info look in help(-h or --help).
)";


const char *to_hex(char c) {
    std::string result;
    std::stringstream str;
    str << std::hex << (int)c;
    str>>result;
    result = "\\x" + result;

    return result.c_str();
}

void read_buffer(char *buffer, bool toHexadecimal) {
    int fd = open(buffer, O_RDONLY);
    char buff[256];
    ssize_t written_bytes = 0;

    if (fd == -1) { //check if open
        std::cerr << "Something went wrong. Error code: " << errno << std::endl;
        exit(errno);
    }

    while ((written_bytes = read(fd, &buff, sizeof(buff))) > 0) { //write buffer

        for (int i = 0; i < written_bytes; ++i) {
            if (toHexadecimal && (!(isspace(buff[i])) && !isprint(buff[i]))) {
                write(STDOUT_FILENO, to_hex(buff[i]), sizeof(buff[i]));
            } else {
                write(STDOUT_FILENO, &buff[i], sizeof(buff[i]));
            }
        }
    }

}


int main(int argc, char *argv[]){
    std::vector<char *> files;
    bool toHexadecimal = 0;

    if (argc == 1){
        std::cout << COPYRIGHT << std::endl;
        exit(EXIT_FAILURE);
    }

    for(int i = 1; i < argc; ++i){
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0){
            std::cout << INSTRUCTION << std::endl;
            exit(EXIT_SUCCESS);
        }
        else if (strcmp(argv[i], "-A") == 0){
            toHexadecimal = 1;
        }
        else{
            files.push_back(argv[i]);
        }
    }

    for (int i; i<files.size(); ++i) {
        read_buffer(files[i], toHexadecimal);
    }

    return 0;
}
