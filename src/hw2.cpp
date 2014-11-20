#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <fstream>
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
  split(tokenList, s, is_any_of(" "), token_compress_on);
    
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

void executeCmd(vector<string> list){

  //cout << "Input list" << endl;
  for(unsigned int i = 0; i < list.size(); i++){
    //cout << "list[" << i << "]: " << list[i] << endl;
  }

    bool listFlag;
    if(list.size() > 1){
        listFlag = true;
    }

  //TOKENIZES WHITE SPACE AND Executes command

  /////////////////////////////////////////////////////
  //Tokenizing
  /////////////////////////////////////////////////////
  vector<string> commandList = tokenSpace(list[0]);
    
  //cout << "commandList initial size: " << commandList.size() << endl;

  //if commandList.size() == 0, Should just exit function.
  if(commandList.size() == 0){
    return;
  }


  //cout << "commandList size: " << commandList.size() << endl;  
  for(unsigned int i = 0; i < commandList.size(); i++){
    //cout << "commandList[" << i << "]: " << commandList[i] << endl;
    if(commandList[i] == ""){
      //cout << "WHAT THE" << endl;
      commandList.erase(commandList.begin()+i);
    }
  }


  //cout << endl << "new commandList" << endl;
  for(unsigned int i = 0; i < commandList.size(); i++){
    //cout << "commandList[" << i << "]: " << commandList[i] << endl;
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

  //Array for pipe
  const int PIPE_READ = 0;
  const int PIPE_WRITE = 1;
  int fd[2];
    if(list.size() > 1){ 
        if(pipe(fd) == -1){
        perror("pipe");
        exit(1);
        }
        //cout << "Created pipe" << endl;
    }

  int pid = fork();
  if(pid == -1){
    //Checks for error with fork function
    perror("fork");
  }
  else if(pid == 0){ //When pid is 0 you are in the child process
    //cout << "In first child" << endl;
    //Input Redirection
    
    string inputFile;
    string outputFile;
        
    //Error checking for input and output redirection

    //Check if input redirection

    unsigned int i;
    int inCnt = 0;
    int outCnt = 0;
    int appCnt = 0;
    int inSCnt = 0;
    for(i = 1; i < commandList.size() - 1; i++){
        if(commandList[i] == "<"){
	        //cout << "Input Redirection" << endl;
	        inCnt++;
	        if(inCnt > 1){
	            cerr << "Error: Multiple input redirection calls" << endl;
	        }
	//cout << "Input redirection called" << endl;
	        string command = commandList[0];
	        inputFile = commandList[i+1];
	//cout << "command: " << command << endl;
	//cout << "inputFile: " << inputFile << endl;

	        commandList.erase(commandList.begin()+i,commandList.end());

	        for(unsigned int i = 0; i < commandList.size(); i++){
	            //cout << "commandList[" << i << "]: " << commandList[i] << endl;
	        }
 
	        //Getting input from inputfile to standard in        
	        int fdi = open(inputFile.c_str(),O_RDONLY);
	        if(fdi == -1){
	            perror("open");
	            exit(1);
	        }
                

	        if(-1 == close(0)){
	            perror("close");
	            exit(1);
	        }

	        if(dup(fdi) == -1){
	            perror("dup");
	            exit(1);
	        }
        }
        if(commandList[i] == ">"){
	        outCnt++;
	        if(outCnt > 1){
                cerr << "Error: Multiple output redirection calls" << endl;
	        }

	        //Output redirection
	        string command = commandList[0];
	        outputFile =  commandList[i+1];
	        commandList.erase(commandList.begin()+i,commandList.end());

	        int fdo = open(outputFile.c_str(),O_WRONLY|O_TRUNC); 
	        if(fdo == -1){
	            //cout << "Creating file" << endl;
	            fdo = creat(outputFile.c_str(), S_IRUSR|S_IWUSR);
	            if(fdo == -1){
	                perror("creat");
	            }
	        }

 
	        //cout << "fdo: " << fdo << endl;
	        if(-1 == close(1)){
	            perror("close");
	            exit(1);
	        }
	        if(dup(fdo) == -1){
	            perror("dup");
                exit(1);
            }
        }
        else if(commandList[i] == ">>"){
	        appCnt++;
	        if(appCnt > 1){
	            cerr << "Error: Multiple append redirection calls" << endl;
                exit(1);
	        }
	        //cout << "Append" << endl;
                
	        //Append Output redirection
	        string command = commandList[0];
	        outputFile = commandList[i+1];
	        commandList.erase(commandList.begin()+i,commandList.end());

	        int fdo = open(outputFile.c_str(),O_WRONLY|O_APPEND);
	        if(fdo == -1){
	            //cout << "Creating file" << endl;
	            fdo = creat(outputFile.c_str(), S_IRUSR|S_IWUSR);
	            if(fdo == -1){
	                perror("creat");
	            }
	        }

	        cout << "fdo: " << endl;
	        if(-1 == close(1)){
	            perror("close");
	            exit(1);
	        }
	        if(dup(fdo) == -1){
	            perror("dup");
	            exit(1);
	        }
        }
        else if(commandList[i] == "<<<"){
	        cout << "String input" << endl;
	        inSCnt++;
	        if(inSCnt > 1){
	            cerr << "Error: Multiple input redirection calls" << endl;
	            exit(1);
	        }

	        string command = commandList[0];
	        outputFile = commandList[i+1];
	        commandList.erase(commandList.begin()+i,commandList.end());

	        //Get string and put it in standard in
	        int fdi = 0;             
 
	        //cout << "fdi1: " << fdi << endl;
	        if(fdi == -1){
	            perror("open");
	            exit(1);
	        }

	        if(-1 == close(0)){
	            perror("close");
	            exit(1);
	        }

	        if(dup(fdi) == -1){
	            perror("dup");
	            exit(1);
	        }
        }
    }

    //cout << "List size: " << list.size() << endl;
    if(listFlag){ 
        //ONLY IF GOING TO PIPE!!!!!!      
        //cout << "Gonna pipe" << endl;
        //EXPERIMENT
        //Makes stdout the write end of the pipe
        if(-1 == dup2(fd[PIPE_WRITE],1)){
            perror("dup2(375)");
            exit(1);
        }
        //Closes the read end of the pipe
        if(-1 == close(fd[0])){
            perror("close(389)");
            exit(1);
        }
    }
    else{
        //cout << "No pipe" << endl;
    }

    //EXPERIMENT

    const int newSize = commandList.size() + 1;
    char **argv;
    argv = new char*[newSize];
     
    unsigned int j;
    for(j = 0; j < commandList.size(); ++j){
      argv[j] = new char[commandList[j].size() + 1];
      strcpy(argv[j], charCommandList[j]);
    }
        
    argv[j] = 0;
    //cout << "charCommandList[0]: " << charCommandList[0] << endl;
    //for(int i = 0; i < sizeof(argv); i++){
    //cout << "argv[" << i << "]: " << argv[i] << endl;
    //}
    int r = execvp(charCommandList[0], argv);
    if(r == -1){
      //Checks for error with execvp
      perror("execvp");
    }
    //Prevents zombie process.
    exit(1);
  }
  else if(pid > 0){ //When pid is greater than 0 you are in the parent process
    wait(NULL);
    //cout << "In first parent" << endl;
  }

    int savestdin;
    if(listFlag){
        //ONLY IF PIPING
        if(-1 == (savestdin = dup(0))){
        perror("dup(428)");
        }
        if(-1 == (dup2(fd[PIPE_READ],0)))
            perror("dup2(fd[PIPE_READ])");
        if(-1 == close(fd[PIPE_WRITE]))
            perror("close(fd[PIPE_WRITE])");

        list.erase(list.begin());
        cout << endl;
        for(unsigned int i = 0; i < list.size(); i++){
            //cout << "list[" << i << "]: " << list[i] << endl;
        }
        //cout << "Calling executeCmd again" << endl;
         executeCmd(list);
    }

     

//Recursive call(tr A-Z a-z, ...., .... , ...)


/*
  int pid2 = fork();
  if(pid2 == -1){
    perror("fork");
  }
  else if(pid2 == 0){ //When pid is 0 you are in the child process


    cout << "In the second child" << endl;


    char *argv[4];
    argv[0] = "tr";
    argv[1] = "A-Z";
    argv[2] = "a-z";
    argv[3] = 0;

/
    char *argv[2];
    argv[0] = "wc";
    argv[1] = 0;
/
    //////
    //Repeat Experiment if not end of pipe
    //////


    if(execvp(argv[0], argv) == -1){
      perror("execvp");
    }
    exit(1);
  }
 
 else{
   wait(NULL);
   cout << "In second parent" << endl;
 }
*/


if(listFlag){
  if(-1 == (dup2(fd[PIPE_READ],0)))
    perror("dup2(fd[PIPE_READ](480))");
}
//Used only when going to another pipe
/*
    if(-1 == close(fd[PIPE_WRITE]))
        perror("close(fd[PIPE_WRITE])");
*/

  if(-1 == dup2(savestdin,0)){
    perror("dup2(460)");
    exit(1);
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
        else{ 
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
            vector<string> commandList = tokenPipe(preCommandList);

            for(unsigned int i = 0; i < commandList.size(); i++){
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


	
    /*
    //Executes commands.
    for(int i = 0; i < mainList.size(); ++i){
    cout << endl;
    //cout << "mainList[" << i << "]: " <<  mainList[i] << endl;
    executeCmd(mainList[i]);
    }
    */

            executeCmd(mainList);

        }
    }

  return 0;
}
