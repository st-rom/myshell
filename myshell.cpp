#include <unistd.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

int dirExists(const char *path)
{
    struct stat info;

    if(stat( path, &info ) != 0)
        return 0;
    else if(info.st_mode & S_IFDIR)
        return 1;
    else
        return 0;
}

std::vector<std::string> splitter(std::string strline, char strdiv){
	std::string word;
	std::vector<std::string> words;
	bool cont = false;
	for(int s = 0;s < strline.length();s++){
		if(strline[s] == '"' || strline[s] == '\''){
			if (cont == true){
				cont = false;
			}
			else{
				cont = true;
			}
		}
		else if(word != "" && strline[s] == strdiv && cont == false){
			words.push_back(word);
			word = "";
		}
		else{
			word += strline[s];
		}
	}
	if(word != ""){
		words.push_back(word);
	}
	return words;
}

int merrno(std::vector<std::string> myargs) {
	if(myargs.size() == 0){
		return 0;
	}
	else if(myargs.size() == 1 && (myargs[0] == "-h" || myargs[0] == "--help")){
		std::cout << "merrno v1.0\nCatches errors" << std::endl;
		return 0;
	}
	std::cout << "Too many arguments were entered. No arguments needed\nFor more info use help [-h|--help]" << std::endl;
	return 1;
}

int mpwd(std::vector<std::string> myargs, std::string mypath) {
	if(myargs.size() == 0){
		std::cout << mypath << std::endl;
		return 0;
	}
	else if(myargs.size() == 1 && (myargs[0] == "-h" || myargs[0] == "--help")){
		std::cout << "mpwd v1.0\nShows current path" << std::endl;
		return 0;
	}
	std::cout << "Too many arguments were entered. No arguments needed\nFor more info use help [-h|--help]" << std::endl;
	return 1;
}


int mycat(std::vector<std::string> myargs) {
	if(myargs.size() > 1 && (myargs[1] == "-h" || myargs[1] == "--help")){
		std::cout << "mycat v1.0\nShows the containment of the file" << std::endl;
		return 0;
	}
	else{
		std::cout << "mycat v1.0\nShows the containment of the file" << std::endl;
		std::string named("mycat.exe");
		pid_t pid = fork();
		char *cmd[] = { "mycat.exe", "mycat.cpp", (char *)0 };
		if(pid == 0){
			execv(named.c_str(), cmd);
			std::cerr << "Failed to execute mycat.exe" << std::endl;
			return 1;   // exec never returns
		}
	}
	//std::cout << "Too many arguments were entered. No arguments needed\nFor more info use help [-h|--help]" << std::endl;
	return 0;
}


int mcd(std::vector<std::string> myargs, std::string mypath) {
	if(myargs.size() != 1){
		return 1;
	}
	else if(myargs[0] == "-h" || myargs[0] == "--help"){
		std::cout << "mcd v1.0\nChanges current directory path" << std::endl;
		return 0;
	}
	else{
		/*
		DIR* dir = opendir(myargs[0]);
		if (dir)
		{
			std::cout << "worked" << std::endl;
			//mypath += '/' + myargs[0];
			closedir(dir);
		}
		else if (ENOENT == errno)
		{
			std::cout << "Directory does not exist" << std::endl;
		}
		else
		{
			std::cout << "Failed for some other reason" << std::endl;
		}
		*/
		
		/*
		std::filesystem::path path(myargs[0]);
		if (path.is_absolute()) {
			std::cout << "absolute" << std::endl;
		}
		else if (path.is_relative()) {
			std::cout << "relative" << std::endl;
		}
		else{
			const char *patho = "ind";
			printf("%d\n", dirExists(patho), patho);
		}
		*/
		return 0;
	}
	return 1;
}

int mexit(std::vector<std::string> myargs) {
	if(myargs.size() == 1){
		if(myargs[0] == "-h" || myargs[0] == "--help"){
			std::cout << "mexit v1.0\nExits program with inputed code\nArgument code(integer) is necessary\n" << std::endl;
			return 0;
		}
		bool isdig = true;
		for(int i = 0;i < myargs[0].size(); i++){
			if(!std::isdigit(myargs[0][i])){
				isdig = false;
			}
		}
		if(isdig){
			exit(atoi(myargs[0].c_str()));
		}
		else{
			std::cout << "Wrong argument was entered. For more info use help [-h|--help]" << std::endl;
			return 1;
		}
		//std::cout << std::stoi(myargs[0]) << std::endl;
		
	}
	else if(myargs.size() > 1){
		std::cout << "Too many were entered.\nPlease read help using [-h] or [--help] to get more info" << std::endl;
		return 1;
	}
	else{
		std::cout << "mexit v1.0\nNo arguments were entered. For more info use help [-h|--help]" << std::endl;
		return 1;
	}
}

int main(int argc, char* argv[]) {
	char cwd[256];
	getcwd(cwd, sizeof(cwd));
	std::cout << "Welcome to myshell" << std::endl;
	//std::cout << sizeof(cwd) << std::endl;
	//std::string mypath = new string(cwd)
	//std::cout << mypath << std::endl;
	std::string mypath(cwd);
	//std::cout << mypath << std::endl;
	//mypath += " $";
	//std::cout << mypath << std::endl;
	std::vector<std::string> folds = splitter(mypath, '/');

	while (true){
		std::string myinput;
		std::cout << "\033[1;34m" + mypath +  "\033[0m" + "\033[1;31m" + " $ " +  "\033[0m";
		getline (std::cin, myinput);
		std::vector<std::string> cmds = splitter(myinput, ' ');
		std::vector<std::string> cmd_args;
		for(int i = 1;i < cmds.size();i++){
			cmd_args.push_back(cmds[i]);
		}
		if(!(cmds.size() == 0) && cmds[0] == "merrno"){
			merrno(cmd_args);
		}
		else if(!(cmds.size() == 0) && cmds[0] == "mpwd"){
			mpwd(cmd_args, mypath);
		}
		else if(!(cmds.size() == 0) && cmds[0] == "mcd"){
			mcd(cmd_args, mypath);
		}
		else if(!(cmds.size() == 0) && cmds[0] == "mexit"){
			mexit(cmd_args);
		}
		else if(!(cmds.size() == 0) && cmds[0] == "mycat"){
			std::cout << "mycat v1.0\nShows the containe" << std::endl;
			mycat(cmds);
		}
		else if(!(cmds.size() == 0) && cmds[0][0] == '.' && cmds[0][1] == '/'){
			//mycat(cmd_args);
			std::cout << cmds[0] << std::endl;
		}
		else if (!(cmds.size() == 0) && !(cmds[0] == "")){
			std::cout << cmds[0] + ": command not found" << std::endl;
		}
		/*for(int s = 0;s < cmds.size();s++){
			std::cout << cmds[s] << std::endl;
		}*/
		//std::cout << myinput << std::endl;
	}
}
