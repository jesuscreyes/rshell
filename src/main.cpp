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
    split(tokenList, s, is_any_of(" "), token_compress_on);
    
    //Optional print of list
    
    for(std::vector<string>::iterator it=tokenList.begin(); it != tokenList.end(); ++it){
        cout << *it << endl;
    }
    
    return tokenList;
}

//Function called tokenQuote
//Input: string
//Output: vector of strings, deliminator: \"
vector<string> tokenQuote(string s){
    vector<string> tokenList;
    split(tokenList, s, is_any_of("\""), token_compress_on);

    //Optional print of list

    for(std::vector<string>::iterator it=tokenList.begin(); it != tokenList.end(); ++it){
        cout << *it << endl;
    }

    return tokenList;
}

//Function: tokenSemicolon
//Input: string
//Output: vector of strings, deliminator: ';'
vector<string> tokenSemicolon(string s){
    vector<string> tokenList;
    split(tokenList, s, is_any_of(";"), token_compress_on);

    //Optional print of list
    for(std::vector<string>::iterator it=tokenList.begin(); it != tokenList.end(); ++it){
        cout << *it << endl;
    }

    return tokenList;
}

//Function: tokenAND
//Input: string
//Output: vector of strings, deliminator: '&'
vector<string> tokenAnd(string s){
    vector<string> tokenList;
    split(tokenList, s, is_any_of("&"), token_compress_on);

    //Optional print of list
    for(std::vector<string>::iterator it=tokenList.begin(); it != tokenList.end(); ++it){
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
    for(std::vector<string>::iterator it = tokenList.begin(); it != tokenList.end(); ++it){
        cout << *it << endl;
    }

    return tokenList;
}

//Function called stringToChar
//Input: vector of strings
//Output: vector of char *

vector<char *>  stringToChar(vector<string> s){
   
   vector<char *>  c;

    for(int i = 0; i < s.size(); ++i){
   
        char *s1;
        s1 = new char[s[i].size() + 1];
        memcpy(s1,s[i].c_str(), s[i].size() + 1);
        
        c.push_back(s1);

    }
    return c;
}

int main(){

    //1. Print a command prompt(e.g. $)
    cout << "$ ";

    //2. Read in a command on one line. Commands will have the form
    //   cmd = executable [argumentList] [connecter cmd]
    //   connecter = || or && or ;

    /////////////////////////////////////////////////////
    //Acquiring String
    /////////////////////////////////////////////////////
    string s;
    getline(cin,s);
    cout << s << endl;

    //Now have to Tokenizer String.
    //For now, let's just try to tokenize ';'

    /////////////////////////////////////////////////////
    //Tokenizing
    /////////////////////////////////////////////////////
    vector<string> commandList = tokenSpace(s);
/*
    char *s1;
    char *s2;
    char *s3;

    s1 = new char[commandList[0].size() + 1];
    memcpy(s1, commandList[0].c_str(), commandList[0].size() + 1);
    s2 = new char[commandList[1].size() + 1];
    memcpy(s2, commandList[1].c_str(), commandList[1].size() + 1);
    s3 = new char[commandList[2].size() + 1];
    memcpy(s3, commandList[2].c_str(), commandList[2].size() + 1);
*/

    vector<char *> charCommandList = stringToChar(commandList);

    //cout << charCommandList[0] << endl;


   /////////////////////////////////////////////////////
   //Executing
   /////////////////////////////////////////////////////
    int pid = fork();
    if(pid == 0){
        cout << "I'm a child" << endl << endl;
        cout << "CommandList size: " << commandList.size() << endl;

        char *argv[commandList.size() + 1];

        int j;
        for(j = 0; j < commandList.size(); ++j){
            argv[j] = new char[commandList[j].size() + 1];
            strcpy(argv[j], charCommandList[j]);
        }
        cout << "j outside of for loop: " << j << endl;
        argv[j] = 0;
        /*
        argv[0] = new char[commandList[0].size() + 1];
        strcpy(argv[0],charCommandList[0]);

        argv[1] = new char[commandList[1].size() + 1];
        strcpy(argv[1],charCommandList[1]);

        argv[2] = new char[commandList[2].size() + 1];
        strcpy(argv[2], charCommandList[2]);

        argv[3] = 0;
        */
        execvp(charCommandList[0], argv);
    }
    else{
        wait(NULL);
        cout << "I'm a parent" << endl << endl;
    }

    return 0;
}
