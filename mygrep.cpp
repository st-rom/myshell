#include <ctype.h>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <vector>


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
	std::string read_file;
	std::string str;
	if(argc < 2){
		std::cout << "No regexp|string or file name. For more info use agument [-h|--help]" << std::endl;
		return 7;
	} 
	for(int i = 1; i < argc; ++i){
		if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0){
			std::cout << "greper" << std::endl;
			return 0;
		}
		if(strcmp(argv[i], "-i") == 0){
			ign = true;
		}
		else if(strcmp(argv[i], "-v") == 0){
			v = true;
		}
		//std::cout << (std::string(argv[i]).find("--file=") == 0) << std::endl;
		else if(std::string(argv[i]).find("--file=") == 0){
			read_file = std::string(argv[i]).substr(7);
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
	std::ifstream infile(read_file);
	std::string line;
	std::vector<std::string> result;
	while (std::getline(infile, line)){
		std::istringstream iss(line);
		//std::cout << "dd" << std::endl;
		if(ign){
			//std::cout << line << std::endl;
			//std::cout << makelower(line) << ' ' << makelower(str) << std::endl;
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
