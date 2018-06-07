#include <unistd.h>
#include <string.h>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/param.h>
#include <dirent.h>
#include <typeinfo>
extern char **environ;
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
			if (cont){
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


int mshbyline(std::string fl){
    char file[fl.length() + 1];
    strcpy(file, fl.c_str());
    std::ifstream infile(file);
    std::string line;

    while (std::getline(infile, line))
    {
        std::vector<const char*> arg_for_c;
        std::vector<std::string> fwa = splitter(line, ' ');

        char **args = static_cast<char**>(malloc( (fwa.size() + 1) * sizeof(char*)));

        for(int i = 0; i < fwa.size();i++){
            args[i] = (char *) fwa[i].c_str();
        }
        args[fwa.size()] = NULL;
        if(line[0] != '#') {
            pid_t pid = fork();
            if (pid == -1)
            {
                std::cerr << "Failed to fork()" << std::endl;
                exit(EXIT_FAILURE);
            }
            else if(pid == 0)
            {
                std::cout << args[1] << std::endl;
                execvpe(fwa[0].c_str(), args, environ);
                std::cerr << "Failed to execute " << std::endl;
                return 1;
            }
            else if (pid > 0) {
                int status;
                waitpid(pid, &status, 0);
            }
        }


    }
    return 0;
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


int myminies(std::vector<std::string> myargs) {
    std::string named;
    if(myargs[0][0] == '.' && myargs[0][1] == '/'){
        named = myargs[0].substr(2);
    }
    else {
        named = myargs[0];
    }
    auto path_ptr = getenv("PATH");
    std::string path_var;
    if(path_ptr != nullptr)
        path_var = path_ptr;
    path_var += ":.";
    setenv("PATH", path_var.c_str(), 1);
    pid_t pid = fork();
    char **args = static_cast<char**>(malloc( (myargs.size() + 1) * sizeof(char*)));
    args[0] = (char *) named.c_str();
    for(int i = 1; i < myargs.size();i++){
        args[i] = (char *) myargs[i].c_str();
    }
    args[myargs.size()] = NULL;
    if (pid == 0){
        execvpe(named.c_str(), args, environ);
        std::cerr << "Failed to execute " + named << std::endl;
        myer = 16;
        return 1;
    }
    int status;
    waitpid(pid, &status, 0);
    myer = 0;
    return 0;
}


int mcd(std::vector<std::string> myargs, std::string mypath) {
	if(myargs.empty() || myargs[0] == "."){
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
	}
}


int main(int argc, char* argv[]) {
	char cwd[256];
	getcwd(cwd, sizeof(cwd));
	std::cout << "Welcome to myshell" << std::endl;
	std::string mypath(cwd);
	std::vector<std::string> folds = splitter(mypath, '/');
	char buffer[MAXPATHLEN];
	for(int i = 1; i < argc; ++i){
	    mshbyline(argv[i]);
	}

	while (true){
		char *path = getcwd(buffer, MAXPATHLEN);
		mypath = path;
		std::string myinput;
		//std::cout << "\033[1;34m" + mypath +  "\033[0m" + "\033[1;31m" + " $ " +  "\033[0m";
        std::cout << mypath + " $ ";
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
		else if(!(cmds.size() == 0) && (cmds[0] == "mymkdir" | cmds[0] == "mycat" | cmds[0] == "myrm" |
                                        cmds[0] == "mycp" | cmds[0] == "mymv" | cmds[0] == "mygrep" |
                                        (cmds[0][0] == '.' && cmds[0][1] == '/'))){
			myminies(cmds);
		}
        else if(!(cmds.size() == 0) && cmds[0][0] == '.' && cmds[0][1] != '/'){
            mshbyline(cmds[0].substr(1));
        }
		else if (!(cmds.size() == 0) && !(cmds[0] == "")){
			myer = 13;
			std::cout << cmds[0] + ": command not found" << std::endl;
		}
	}
}
