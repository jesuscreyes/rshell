#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <string>
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

//Global Variables
	int andFlag = 0;
	int orFlag = 0;

//Function called tokenSpace
//Input: string
//Output: vector of strings, deliminator "white space"
vector<string> tokenSpace(string s){
    vector<string> tokenList;
    split(tokenList, s, is_any_of("  "), token_compress_on);
    
    return tokenList;
}

//Function called tokenHash
//Input: string
//Output: vector of strings, deliminator: #
vector<string> tokenHash(string s){
    vector<string> tokenList;
    split(tokenList, s, is_any_of("#"), token_compress_on);

    return tokenList;
}

//Function: tokenSemicolon
//Input: string
//Output: vector of strings, deliminator: ';'
vector<string> tokenSemicolon(string s){
    vector<string> tokenList;
    split(tokenList, s, is_any_of(";"), token_compress_on);
    
	return tokenList;
}

//Function: tokenPipe
//Input: string
//Output: vector of strings, deliminator: '|'
vector<string> tokenPipe(string s){
    vector<string> tokenList;
    split(tokenList, s, is_any_of("|"), token_compress_on);
    
	return tokenList;
}

//Function: tokenInputR
//Input: string
//Output: vector of strings, deliminator: '<'
vector<string> tokenInputR(string s){
    vector<string> tokenList;
    split(tokenList, s, is_any_of("<"), token_compress_on);
    
	return tokenList;
}

//Function: tokenAND
//Input: string
//Output: vector of strings, deliminator: '&'
//Replaces '&&' with '!', tokenizes with '!' as deliminator.
vector<string> tokenAnd(string s){
    vector<string> tokenList;

	string replaceAnd = "!";

	for(unsigned int i = 0; i < s.size(); i++){
		if(s[i] == '&'){
			if(s[i+1] != '\0'){
				if(s[i+1] == '&'){
					s.replace(i,2,replaceAnd);
				}
			}
		}
	}


	split(tokenList, s, is_any_of("!"), token_compress_on);

    return tokenList;
}

//Function: tokenOR
//Input: string
//Output: vector of strings, deliminator: '|'
//Replaces '||' with '!', tokenizes '!' as deliminator.
vector<string> tokenOr(string s){
    vector<string> tokenList;

        string replaceOr = "!";

        for(unsigned int i = 0; i < s.size(); i++){
                if(s[i] == '|'){
                        if(s[i+1] != '\0'){
                                if(s[i+1] == '|'){
				s.replace(i,2,replaceOr);
                                }
                        }
                }
        }


	split(tokenList, s, is_any_of("!"), token_compress_on);

	orFlag = 1;

    return tokenList;

}

//Function called stringToChar
//Input: vector of strings
//Output: vector of char *

vector<char *>  stringToChar(vector<string> &s){
   
   vector<char *>  c;

    for(unsigned int i = 0; i < s.size(); ++i){
        c.push_back(&s[i][0]);
    }
    return c;
}

//Function: executeCmd
//Input: String that only needs white spaces to be eliminated
//Output: Void. Executes command.
/////////////////////////////////////////////////////

void executeCmd(string s){


    //TOKENIZES WHITE SPACE AND Executes command

    /////////////////////////////////////////////////////
    //Tokenizing
    /////////////////////////////////////////////////////
    vector<string> commandList = tokenSpace(s);
    
    //cout << "commandList initial size: " << commandList.size() << endl;

    //if commandList.size() == 0, Should just exit function.
    if(commandList.size() == 0){
	return;
    }
   
    //This for loop takes care of instances where the commandList array has a blank space in it's first index 
    for(unsigned int i = 0; i < commandList.size(); i++){
        if((int) commandList[i][0] == 0){
            commandList.erase(commandList.begin()+i);
        }
    }


   
 
    //Creates vector of char * so that I can use execvp
    vector<char *> charCommandList = stringToChar(commandList);

/*
    //Input Redirection
    
    string inputFile;
    //Check if input redirection
    if(commandList[1] == "<"){
        //cout << "Input redirection called" << endl;
        string command = commandList[0];
        inputFile = commandList[2];
        //cout << "command: " << command << endl;
        //cout << "inputFile: " << inputFile << endl;

        commandList.erase(commandList.begin(),commandList.begin()+2);

        for(int i = 0; i < commandList.size(); i++){
            //cout << "commandList[" << i << "]: " << commandList[i] << endl;
        }
 
        //Getting input from inputfile to standard in        
        int fdi = open(inputFile.c_str(),O_RDONLY);
        //cout << "fdi1: " << fdi << endl;
        if(fdi == -1){
            perror("open");
            exit(1);
        }

        if(-1 == close(0)){
            perror("close");
        }

        if(dup(fdi) == -1){
            perror("dup");
        }

        int pid = fork();
        if(pid == -1){
            perror("fork");
        }

        else if(pid == 0){ //When pid is 0 you are in the child process
            //Trying to execute cat now
            const int newSize = commandList.size() + 1;
            char **argv;
            argv = new char*[newSize];
            int j;
            for(j = 0; j < commandList.size(); j++){
                argv[j] = new char[commandList[j].size() + 1];
                strcpy(argv[j], charCommandList[j]);
            }
            argv[j] = 0;
            if(execvp(charCommandList[0], argv) == -1){
                perror("execvp");
            }
        }
        else{
            wait(NULL);
        }

        exit(1);
    }
*/
    /////////////////////////////////////////////////////
    //Executing
   /////////////////////////////////////////////////////

    int pid = fork();
    if(pid == -1){
	//Checks for error with fork function
	perror("fork");
	}
    else if(pid == 0){ //When pid is 0 you are in the child process

    //Input Redirection
    
        string inputFile;
        //Check if input redirection

        int i;
        for(i = 0; i < commandList.size(); i++){
        //
        if(commandList[i] == "<"){
            //cout << "Input redirection called" << endl;
            string command = commandList[0];
            inputFile = commandList[i+1];
            //cout << "command: " << command << endl;
            //cout << "inputFile: " << inputFile << endl;

            commandList.erase(commandList.begin()+i,commandList.end());

            //for(int i = 0; i < commandList.size(); i++){
                //cout << "commandList[" << i << "]: " << commandList[i] << endl;
            //}
 
            //Getting input from inputfile to standard in        
            int fdi = open(inputFile.c_str(),O_RDONLY);
            //cout << "fdi1: " << fdi << endl;
            if(fdi == -1){
                perror("open");
                exit(1);
            }

            if(-1 == close(0)){
                perror("close");
            }

            if(dup(fdi) == -1){
                perror("dup");
            }
        }
        else if(commandList[i] == ">"){
        
        }
        //
        }
       
        /*
        for(int i = 0; i < commandList.size(); i++){
            cout << "commandList[" << i << "]: " << commandList[i] << endl;
        }
        */

 	    const int newSize = commandList.size() + 1;
	    char **argv;
 	    argv = new char*[newSize];
     
        unsigned int j;
        int num = i + 1;
        //cout << "i" << i << endl;
        //cout << "commandList size: " << commandList.size() << endl;
        for(j = 0; j < commandList.size(); ++j){
		argv[j] = new char[commandList[j].size() + 1];
            	strcpy(argv[j], charCommandList[j]);
        }
        
        argv[j] = 0;
        //cout << "charCommandList[0]: " << charCommandList[0] << endl;
        //cout << "argv[0]: " << argv[0] << endl;
        //cout << "argv[1]: " << argv[1] << endl;
        int r = execvp(charCommandList[0], argv);
	if(r == -1){
	    //Checks for error with execvp
            perror("execvp");
        }
	//Prevents zombie process.
        exit(1);
    }
    else{ //When pid is greater than 0 you are in the parent process
        wait(NULL);
    }
}

/////////////////////////////////////////////////////

int main(){

    bool run = true;


    string s;    
    while(run){

        cout << endl << "$ ";
        //1. Print a command prompt(e.g. $)
        //cout << "$ ";

        //2. Read in a command on one line. Commands will have the form
        //   cmd = executable [argumentList] [connecter cmd]
        //   connecter = || or && or ;

        /////////////////////////////////////////////////////
        //Acquiring String
        /////////////////////////////////////////////////////
        getline(cin,s);
        
        if(s == "exit"){
            exit(1);
        } 
        ////////////////////////////////////////////////////
        //Remove Comments
        ////////////////////////////////////////////////////
        string preCommandList = tokenHash(s)[0];

        ////////////////////////////////////////////////////
        //Handle && cases
        ////////////////////////////////////////////////////

        //Removes &&'s from inputline
        vector<string> andList = tokenAnd(preCommandList);

        ///////////////////////////////////////////////////
        //Handle || cases
        ///////////////////////////////////////////////////
        
        //Removes ||'s from input line
        vector<string> orList = tokenOr(preCommandList);

        ////////////////////////////////////////////////////
        //Remove semicolons
        ///////////////////////////////////////////////////
        vector<string> commandList = tokenSemicolon(preCommandList);

        for(int i = 0; i < commandList.size(); i++){
            //cout << commandList.at(i) << endl;
        }       
 
        vector<string> mainList;
       
 
	//Based off connecter, determines which vector of commands to run.	
	if(andList.size() > 1){
            mainList = andList;
        }
        else if(orList.size() > 1){
            mainList = orList;
        }
        else{
            mainList = commandList;
        }


	
	//Executes commands.
        for(unsigned int i = 0; i < mainList.size(); ++i){
            cout << endl;
            //cout << "mainList[" << i << "]: " <<  mainList[i] << endl;
            executeCmd(mainList[i]);
        }
    }

    return 0;
}
