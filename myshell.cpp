#include <unistd.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/param.h>
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
        std::cout << myer << std::endl;
        myer = 0;
		return 0;
	}
   
	else if(myargs.size() == 1 && (myargs[0] == "-h" || myargs[0] == "--help")){
		std::cout << "merrno v1.0\nCatches errors" << std::endl;
        myer = 0;
		return 0;
	}
	std::cout << "Too many arguments were entered. No arguments needed\nFor more info use help [-h|--help]" << std::endl;
    myer = 7;
	return 1;
}


int mpwd(std::vector<std::string> myargs, std::string mypath){
	if(myargs.size() == 0){
		char buffer[MAXPATHLEN];
		char *path = getcwd(buffer, MAXPATHLEN);
		std::cout << path << std::endl;
		myer = 0;
		return 0;
	}
	else if(myargs.size() == 1 && (myargs[0] == "-h" || myargs[0] == "--help")){
		std::cout << "mpwd v1.0\nShows current path" << std::endl;
		myer = 0;
		return 0;
	}
	std::cout << "Too many arguments were entered. No arguments needed\nFor more info use help [-h|--help]" << std::endl;
	myer = 7;
	return 1;
}


int moutput(std::vector<std::string> myargs) {
	std::string named(myargs[0]);
	auto path_ptr = getenv("PATH");
    std::string path_var;
    if(path_ptr != nullptr)
        path_var = path_ptr;
    path_var += ":.";
    setenv("PATH", path_var.c_str(), 1);
	pid_t pid = fork();
	std::vector<const char*> arg_for_c;
       for(auto s: myargs)
           arg_for_c.push_back(s.c_str());
       arg_for_c.push_back(nullptr);
	if (pid == 0){
		execvp(named.c_str(), const_cast<char* const*>(arg_for_c.data()));
		std::cerr << "Failed to execute " + myargs[0] << std::endl;
		myer = 16;
		return 1;
	}
	myer = 0;
	return 0;
}

int mycat(std::vector<std::string> myargs) {
	if(myargs.size() > 1 && (myargs[1] == "-h" || myargs[1] == "--help")){
		std::cout << "mycat v1.0\nShows the containment of the file" << std::endl;
		std::cout << "Arguments are the name of the files and path to them if needed. Different files are divided by space.\n";
		std::cout << "You can enter as many as you want" << std::endl;
		myer = 0;
		return 0;
	}
	else if (myargs.size() > 1){
		std::cout << "mycat v1.0\nShows the containment of the file" << std::endl;
		std::string named("./mycat");
		
		auto path_ptr = getenv("PATH");
        std::string path_var;
        if(path_ptr != nullptr)
            path_var = path_ptr;
        path_var += ":.";
        setenv("PATH", path_var.c_str(), 1);
		
		pid_t pid1 = fork();
		if (pid1 == 0){
			std::vector<const char*> cmd = {"gcc", "mycat.cpp", "-std=c++11", "-o", "mycat", nullptr};
			execvp("g++", const_cast<char* const*>(cmd.data()));
			std::cerr << "Failed to create mycat.exe" << std::endl;
			myer = 15;
			return 1;
		}
		int status;
		waitpid(pid1, &status, 0);
		pid_t pid2 = fork();
		std::vector<const char*> arg_for_c;
        for(auto s: myargs)
            arg_for_c.push_back(s.c_str());
        arg_for_c.push_back(nullptr);
		if (pid2 == 0){
			execvp(named.c_str(), const_cast<char* const*>(arg_for_c.data()));
			std::cerr << "Failed to execute mycat.exe" << std::endl;
			myer = 16;
			return 1;
		}
		myer = 0;
		return 0;
	}
	std::cout << "Arguments are necessary. No arguments were entered\nFor more info use help [-h|--help]" << std::endl;
	myer = 22;
	return 1;
}


int mcd(std::vector<std::string> myargs, std::string mypath) {
	if(myargs.size() == 0 || myargs[0] == "."){
        return 0;
    }
	else if(myargs.size() == 1 && myargs[0] == ".."){
		std::vector<std::string> p = splitter(mypath, '/');
		std::string one_higher = p[0];
		for(int i = 1;i < p.size() - 1; i++){
			one_higher += '/' + p[i];
		}
		chdir(one_higher.c_str());
		myargs[0] = one_higher;
		myer = 0;
		return 0;
		
	}
	else if(myargs.size() != 1){
		std::cout << "Arguments are necessary. No arguments were entered\nFor more info use help [-h|--help]" << std::endl;
		myer = 7;
		return 1;
	}
	else if(myargs.size() == 1 && (myargs[0] == "-h" || myargs[0] == "--help")){
		std::cout << "mcd v1.0\nChanges current directory path\n<..> will move you one directory higher\n<'path'> enter where you want to move to do it" << std::endl;
		myer = 0;
		return 0;
	}
	
	const char *path = myargs[0].c_str();
	int acc = dir_check(path);
	if (acc == 1){
		chdir(myargs[0].c_str());
		myer = 0;
		return 0;
	}
	std::cout << "Wrong path. Such path doesn't exist\nFor more info use help [-h|--help]" << std::endl;
	myer = 22;
	return 1;
	
}

int mexit(std::vector<std::string> myargs) {
	if(myargs.size() == 1){
		if(myargs[0] == "-h" || myargs[0] == "--help"){
			std::cout << "mexit v1.0\nExits program with inputed code\nArgument code(integer) is necessary\n" << std::endl;
            myer = 0;
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
            myer = 22;
			return 1;
		}
	}
	else if(myargs.size() > 1){
		std::cout << "Too many were entered.\nPlease read help using [-h] or [--help] to get more info" << std::endl;
        myer = 7;
		return 1;
	}
	else{
		exit(0);
		myer = 0;
		return 0;
	}
}

int main(int argc, char* argv[]) {
	char cwd[256];
	getcwd(cwd, sizeof(cwd));
	std::cout << "Welcome to myshell" << std::endl;
	std::string mypath(cwd);
	std::vector<std::string> folds = splitter(mypath, '/');
	char buffer[MAXPATHLEN];

	while (true){
		char *path = getcwd(buffer, MAXPATHLEN);
		mypath = path;
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
			mycat(cmds);
		}
		else if(!(cmds.size() == 0) && cmds[0][0] == '.' && cmds[0][1] == '/'){
			moutput(cmds);
		}
		else if (!(cmds.size() == 0) && !(cmds[0] == "")){
			myer = 13;
			std::cout << cmds[0] + ": command not found" << std::endl;
		}
	}
}
