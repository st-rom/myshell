#include <unistd.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <sstream>


std::vector<std::string> splitter(std::string strline, char strdiv){
	std::string word;
	std::vector<std::string> words;
	for(int s = 0;s < strline.length();s++){
		if(strline[s] != strdiv){
			word += strline[s];
		}
		else if(word != "" && strline[s] == strdiv){
			words.push_back(word);
			word = "";
		}
	}
	if(word != ""){
		words.push_back(word);
	}
	return words;
}


int main(int argc, char* argv[]) {
	char cwd[256];
	getcwd(cwd, sizeof(cwd));
	std::cout << cwd << std::endl;
	std::cout << sizeof(cwd) << std::endl;
	//std::string mypath = new string(cwd)
	//std::cout << mypath << std::endl;
	std::string mypath(cwd);
	std::cout << mypath << std::endl;
	//mypath += " $";
	std::cout << mypath << std::endl;
	std::vector<std::string> folds = splitter(mypath, '/');
	/*
	std::string word;
	
	for(int s = 0;s < mypath.length();s++){
	//	std::cout << " dfn " << folds << std::endl;
		if(mypath[s] != '/'){
			word += mypath[s];
		}
		else if(word != "" && mypath[s] == '/'){
			folds.push_back(word);
			word = "";
		}
	}
	
	if(word != ""){
		folds.push_back(word);
	}
	
	for(int s = 0;s < folds.size();s++){
		std::cout << folds[s] << std::endl;
	}
	*/
	std::vector<string> main_cmds = {"mcd", "merror"}
	while (true){
		std::string myinput;
		std::cout << mypath << " $ ";
		getline (std::cin, myinput);
		std::vector<std::string> cmds = splitter(myinput, ' ');
		for (int i = 0; i < main_cmds.size(); i++){
			if(cmds[0] == main_cmds[i]){
				
			}
		}
		//std::cout << myinput << std::endl;
	}
	
}
