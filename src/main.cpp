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

//Function called tokenQuote
//Input: string
//Output: vector of strings, deliminator: \"
vector<string> tokenQuote(string s){
    vector<string> tokenList;
    split(tokenList, s, is_any_of("\""), token_compress_on);

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
    split(tokenList, s, is_any_of("&"), token_compress_on);
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
   
        //char *s1;
        //s1 = new char[s[i].size() + 1];
        //memcpy(s1,s[i].c_str(), s[i].size() + 1);
        
        c.push_back(&s[i][0]);
    }
    return c;
}

//Function: executeNow
//Input: String that only needs white spaces to be eliminated
//Output: Void. Executes command.
/////////////////////////////////////////////////////

void executeCmd(string s){

    //TOKENIZES WHITE SPACE AND Executes command

    /////////////////////////////////////////////////////
    //Tokenizing
    /////////////////////////////////////////////////////
    vector<string> commandList = tokenSpace(s);
    
    for(int i = 0; i < commandList.size(); i++){
        //cout << "PRINTING COMMANDLIST: " << endl << commandList[i] << endl;
        //cout << "Printing ASCII: " << endl << (int) commandList[i][0] << endl;
        if((int) commandList[i][0] == 0){
            //cout << "ERASING" << endl; 
            commandList.erase(commandList.begin()+i);
        }
    }


    cout << endl;

    vector<char *> charCommandList = stringToChar(commandList);
 

    /////////////////////////////////////////////////////
    //Executing
   /////////////////////////////////////////////////////
    int pid = fork();
    if(pid == 0){
        //cout << "I'm a child" << endl << endl;
        //cout << "CommandList size: " << commandList.size() << endl;
        char *argv[commandList.size() + 1];
     
        int j;
        for(j = 0; j < commandList.size(); ++j){
            argv[j] = new char[commandList[j].size() + 1];
            strcpy(argv[j], charCommandList[j]);
        }
        
        argv[j] = 0;
        
        //cout << "Right before execvp" << endl;
        int r = execvp(charCommandList[0], argv);
        //cout << "Right after execvp" << endl;
        if(r ==-1){
            perror("execvp");
        }
    }
    else{
        wait(NULL);
        //cout << endl << "$ ";
    }
    //cout << "end of executeCmd function" << endl << endl;
}
/////////////////////////////////////////////////////

int main(){

    bool exit = false;

    string s;    
    while(!exit){

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
            break;
        }
       
        ////////////////////////////////////////////////////
        //Remove semicolons
        ///////////////////////////////////////////////////
        vector<string> commandList = tokenSemicolon(s);

        for(int i = 0; i < commandList.size(); ++i){
            cout << endl;
            //cout << commandList[i] << endl << endl;
            //cout << "Right before executing command" << endl;
            executeCmd(commandList[i]);
            //cout << "After executing command" << endl;
        }


        //executeCmd(s);

        //Now have to Tokenizer String.
        //For now, let's just try to tokenize ';'

//TOKENIZES WHITE SPACE AND Executes command
        /////////////////////////////////////////////////////
        //Tokenizing
        /////////////////////////////////////////////////////

/*
        vector<string> commandList = tokenSpace(s);

        vector<char *> charCommandList = stringToChar(commandList);

        //cout << charCommandList[0] << endl;

        /////////////////////////////////////////////////////
        //Executing
        /////////////////////////////////////////////////////
        int pid = fork();
        if(pid == 0){
            // cout << "I'm a child" << endl << endl;
            //cout << "CommandList size: " << commandList.size() << endl;
            char *argv[commandList.size() + 1];
            
            int j;
            for(j = 0; j < commandList.size(); ++j){
                argv[j] = new char[commandList[j].size() + 1];
                strcpy(argv[j], charCommandList[j]);
            }
            argv[j] = 0;

            execvp(charCommandList[0], argv);
        }
        else{
            wait(NULL);
            //cout << endl << "$ ";
        }
    */
    }

    return 0;
}
