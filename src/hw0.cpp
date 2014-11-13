#include <stdio.h>
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
 

    /////////////////////////////////////////////////////
    //Executing
   /////////////////////////////////////////////////////

    int pid = fork();
    if(pid == -1){
	//Checks for error with fork function
	perror("fork");
	}
    else if(pid == 0){
    	const int newSize = commandList.size() + 1;
	char **argv;
 	argv = new char*[newSize];
     
        unsigned int j;
        for(j = 0; j < commandList.size(); ++j){
		argv[j] = new char[commandList[j].size() + 1];
            	strcpy(argv[j], charCommandList[j]);
        }
        
        argv[j] = 0;
        
        int r = execvp(charCommandList[0], argv);
	if(r == -1){
	    //Checks for error with execvp
            perror("execvp");
        }
	//Prevents zombie process.
        exit(1);
    }
    else{
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
            executeCmd(mainList[i]);
        }
    }

    return 0;
}
