#include <ctype.h>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <regex>


std::string makelower(std::string str){
	std::string toret;
	for(int i = 0; i < str.length(); i++){
		toret += tolower(str[i]);
	}
	return toret;
}


int main(int argc, char *argv[]){
	bool ign = false;
	bool v = false;
	bool locked = false;
	std::string restr;
	std::string read_file;
	std::string str;
	if(argc < 2){
		std::cout << "No regexp|string or file name. For more info use agument [-h|--help]" << std::endl;
		return 7;
	} 
	for(int i = 1; i < argc; ++i){
		if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0){
			std::cout << "grep\n-i ignore case\n-v invert match" << std::endl;
			return 0;
		}
		if(strcmp(argv[i], "-i") == 0){
			ign = true;
		}
		else if(strcmp(argv[i], "-v") == 0){
			v = true;
		}
		else if(std::string(argv[i]).find("--file=") == 0){
			read_file = std::string(argv[i]).substr(7);
		}
		else if(std::string(argv[i]).find("--regexp=") == 0){
			restr = std::string(argv[i]).substr(9);//, std::string(argv[i]).length() - 1);
			locked = true;
		}
		else if(!locked){
			str = argv[i];
			locked = true;
		}
		else{
			std::cout << "Wrong arguments" << std::endl;
			return -1;
		}
	}
	if(!locked){
		std::cout << "Wrong arguments" << std::endl;
		return -1;
	}
	std::cout << restr << std::endl;
	std::ifstream infile(read_file);
	std::string line;
	std::vector<std::string> result;
	while (std::getline(infile, line)){
		std::istringstream iss(line);
		std::regex re(restr);
		if(ign){
			if(makelower(line).find(makelower(str)) != -1 && !v){
				result.push_back(line);
			}
			else if(v && makelower(line).find(makelower(str)) == -1){
				result.push_back(line);
			}
		}
		else{
			if(line.find(str) != -1 && !v){
				result.push_back(line);
			}
			else if(v && line.find(str) == -1){
				result.push_back(line);
			}
		}
	}
	for (int i = 0; i < result.size(); i++){
		std::cout << result[i] << std::endl;
	}
	return 0;
}
