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
list<string> myTokenizer(string s){
    list<string> tokenList;
    split(tokenList, s, is_any_of(" "), token_compress_on);
    
    //Optional print of list
    for(std::list<string>::iterator it=tokenList.begin(); it != tokenList.end(); ++it){
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

    //Acquiring String
    string s;
    getline(cin,s);
    cout << s << endl;

    //Now have to Tokenizer String.
    //For now, let's just try to tokenize ';'

    //////////
    //Tokenizing
    /////////
    list<string> commandList = myTokenizer(s);

    return 0;
}
