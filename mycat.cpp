#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <ctype.h>
#include <stdlib.h>


int main(int argc, char *argv[]){
	std::vector<std::string> files;
	//files.push_back("qw\n");
	//std::cout<<files[0];
	int num_f;
	bool ahex = 0;
	for(int i = 1; i < argc; ++i){
		//std::std::cout << argv[1] << std::std::endl;
		if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0){
			std::cout << "Arguments are the name of the files and path to them if needed. Different files are divided by space.\n";
		std::cout << "You can enter as many as you want.\n-A 	invisible symbols will be printed in hexadecimal code." << std::endl;
			exit(EXIT_SUCCESS);
		}
		else if (strcmp(argv[i], "-A") == 0){
			ahex = 1;
		}
		else{
			files.push_back(argv[i]);
			num_f++;
		}
	}
	if (argc == 1){
		std::cout << "mycat.cpp\nMade by Roman Stepaniuk\nThis program shows the contents of files.\nArguments:\n<name of file>    files to work with\n-A 	invisible symbols will be printed in hexadecimal code.\n-h or --help 	more help." << std::endl;
	}
	//std::cout << "asd " << num_f;
	for(int i = 0; i < num_f; ++i){
		std::cout << files[i] << ":\n" << std::endl;
		std::string line;
		std::stringstream ss;
		std::ifstream myfile (files[i]);
		//std::cout << "DARRRR";
		if (myfile.is_open())
		{
			while ( getline (myfile,line) )
			{
				//std::cout << line << '\n';
				ss << line << '\n';
			}
			myfile.close();
		}
		
		else{
			std::cout << "Unable to open file.\nMake sure you typed correct file name and path to it(if needed)\nFor more info look in help(-h or --help)." << std::endl;
			exit(EXIT_FAILURE);
		}
		std::string s = ss.str();
		//std::cout << s;
		int n = s.length();
		//char char_array[n+1]; 
		//strcpy(char_array, s.c_str());
		
		//convertToHexadecimal(char_array, n);
		for (int i = 0; i < n; i++ ){
			if(!isprint(s[i]) && ahex == 1){
				std::string hex;
				hex.resize(4);
				sprintf((char *) hex.c_str(), "\\x%02X", s[i]);
				std::cout << hex;
			}
			else{
				/*string hex;
				hex.resize(4);
				sprintf((char *) hex.c_str(), "\\x%02X", s[i]);
				std::cout << sat;*/
				std::cout << s[i];
				//writetofile(s.c_str());
				//printf("%s", s.c_str());
				//std::cout << hex << (unsigned int)(s[i]);
			}
		}
		std::cout << "\n\n";
	}
		
	return 0;
}
