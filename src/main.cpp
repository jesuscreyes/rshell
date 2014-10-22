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

//Function called tokenSpace
//Input: string
//Output: vector of strings, deliminator "white space"
vector<string> tokenSpace(string s){
    vector<string> tokenList;
    split(tokenList, s, is_any_of("  "), token_compress_on);
    
    //Optional print of list
    //cout << "Printing list" << endl;  
    //for(std::vector<string>::iterator it=tokenList.begin(); it != tokenList.end(); ++it){
    //    cout << *it << endl;
    //}
  
    return tokenList;
}

//Function called tokenHash
//Input: string
//Output: vector of strings, deliminator: #
vector<string> tokenHash(string s){
    vector<string> tokenList;
    split(tokenList, s, is_any_of("#"), token_compress_on);

    //Optional print of list
/*
    for(std::vector<string>::iterator it=tokenList.begin(); it != tokenList.end(); ++it){
        cout << *it << endl;
    }
*/
    return tokenList;
}

//Function: tokenSemicolon
//Input: string
//Output: vector of strings, deliminator: ';'
vector<string> tokenSemicolon(string s){
    vector<string> tokenList;
    split(tokenList, s, is_any_of(";"), token_compress_on);
/*
    //Optional print of list
    for(std::vector<string>::iterator it=tokenList.begin(); it != tokenList.end(); ++it){
        cout << *it << endl;
    }
*/
    return tokenList;
}

//Function: tokenAND
//Input: string
//Output: vector of strings, deliminator: '&'
vector<string> tokenAnd(string s){
    vector<string> tokenList;

	cout << "In tokenAND" << endl;
	string replaceAnd = " ";

	for(unsigned int i = 0; i < s.size(); i++){
		cout << "In for loop" << endl;
		if(s[i] == '&'){
			cout << "Current char is '&'" << endl;
			cout << "pos: " << i << endl;
			if(s[i+1] != '\0'){
				cout << "next char not null" << endl;
				cout << "next char is: " << s[i+1] << endl;
				if(s[i+1] == '&'){
				cout << "next char IS &" << endl;
					s.replace(i,2,replaceAnd);
				}
			}
		}
	}

	cout << "PRINTING NEW S: " << s << endl;


	for(std::vector<string>::iterator it=tokenList.begin(); it!= tokenList.end(); ++it){
		cout << *it << endl;
	}

    return tokenList;
}

//Function: tokenOR
//Input: string
//Output: vector of strings, deliminator: '|'
vector<string> tokenOr(string s){
    vector<string> tokenList;
    split(tokenList, s, is_any_of("|"), token_compress_on);

    //Optional print of list
    //for(std::vector<string>::iterator it = tokenList.begin(); it != tokenList.end(); ++it){
    //    cout << *it << endl;
    //}

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
	perror("fork");
	}
    else if(pid == 0){
    	const int newSize = commandList.size() + 1;
	char **argv;
 	argv = new char*[newSize];
     
        unsigned int j;
        for(j = 0; j < commandList.size(); ++j){
		//int newSize = commandList[j].size() + 1;
		argv[j] = new char[commandList[j].size() + 1];
            	strcpy(argv[j], charCommandList[j]);
        }
        
        argv[j] = 0;
        
        int r = execvp(charCommandList[0], argv);
        if(r ==-1){
            perror("execvp");
        }
        exit(1);
    }
    else{
        wait(NULL);
    }
}

//Function: checkAnd
//Input: string
//Output: boolean


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
        
        //Removes |'s from input line
        vector<string> orList = tokenOr(preCommandList);

        ////////////////////////////////////////////////////
        //Remove semicolons
        ///////////////////////////////////////////////////
        vector<string> commandList = tokenSemicolon(preCommandList);
        
        vector<string> mainList;
        if(andList.size() > 1){
            mainList = andList;
        }
        else if(orList.size() > 1){
            mainList = orList;
        }
        else{
            mainList = commandList;
        }

        for(unsigned int i = 0; i < mainList.size(); ++i){
            cout << endl;
            executeCmd(mainList[i]);
        }
    }

    return 0;
}
