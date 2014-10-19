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

//Function called MyTokenizer
//Input: string
//Output: list of commands
vector<string> myTokenizer(string s){
    vector<string> tokenList;
    split(tokenList, s, is_any_of(" "), token_compress_on);
    
    //Optional print of list
    
    for(std::vector<string>::iterator it=tokenList.begin(); it != tokenList.end(); ++it){
        cout << *it << endl;
    }
    
    return tokenList;
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
    vector<string> commandList = myTokenizer(s);


   /////////////////////////////////////////////////////
   //Executing
   /////////////////////////////////////////////////////
    int pid = fork();
    if(pid == 0){
        cout << "I'm a child" << endl << endl;

        char *argv[4];
        argv[0] = new char[3];
        strcpy(argv[0],"ls");

        argv[1] = new char[3];
        strcpy(argv[1],"-a");

        argv[2] = new char[3];
        strcpy(argv[2], "-l");

        argv[3] = 0;

        execvp("ls", argv);
    }
    else{
        wait(NULL);
        cout << "I'm a parent" << endl << endl;
    }

    return 0;
}
