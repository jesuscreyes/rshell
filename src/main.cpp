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
    split(tokenList, s, is_any_of("&&"), token_compress_on);
/*
    //Optional print of list
    for(std::vector<string>::iterator it=tokenList.begin(); it != tokenList.end(); ++it){
        cout << *it << endl;
    }
*/
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

    for(int i = 0; i < s.size(); ++i){
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
    for(int i = 0; i < commandList.size(); i++){
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
    if(pid == 0){
        char *argv[commandList.size() + 1];
     
        int j;
        for(j = 0; j < commandList.size(); ++j){
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
        /*
        vector<string> andList = tokenAnd(preCommandList);
        cout << "Removing &&'s: " << endl; 
        cout << "size: " << andList.size() << endl;
        for(int i = 0; i < andList.size(); i++){
            cout << endl << andList[i] << endl;
        }
        cout << endl;
        */

        ////////////////////////////////////////////////////
        //Remove semicolons
        ///////////////////////////////////////////////////
        vector<string> commandList = tokenSemicolon(preCommandList);
        

        for(int i = 0; i < commandList.size(); ++i){
            cout << endl;
            //cout << endl << "Executing this command: " << commandList[i] << endl;
            executeCmd(commandList[i]);
        }
    }

    return 0;
}
