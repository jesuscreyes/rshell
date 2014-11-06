#include <fstream>
#include "Timer.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

void streamMethod(const string input, const string output){

    ifstream ifs;
    ifs.open(input.c_str());
    
    if(!ifs.is_open()){
        cerr << "Input file not open" << endl;
        exit(1);
    }

    ofstream ofs;
    ofs.open(output.c_str());
    if (!ofs.is_open())
    {
        cerr << "Output file not open" << endl;
        exit(1);
    }
    

    char c;
    while(!ifs.eof()){
        ifs.get(c);
        ofs.put(c);
    }
    ifs.close();
    ofs.close();
}   

void copyMethod(const string input, const string output, size_t count)
{


    int in = open(input.c_str(), O_RDONLY);
    if(in == -1){
        perror("open");
        exit(1);
    }
    int out = open(output.c_str(), O_WRONLY);
    if(out == -1){
        perror("open");
        exit(1);
    }
    char buf[count];
    int test = 1;
    
    
    do{
        test = read(in, buf, sizeof(buf));
        if(test == -1){
            perror("read");
            exit(1);
        }
        int check =  write(out,buf,test);
        if(check == -1){
            perror("write");
            exit(1);
        }
    }while(test != 0);
            
    int check = close(in);
    if(check == -1){
        perror("close");
        exit(1);
    }
    check = close(out);
    if(check == -1){
        perror("close");
        exit(1);
    }
}


int main(int argc, char *argv[]){

    if(argc < 3 || argc > 4){
        cerr << "Inappropriate number of arguments" << endl;
        exit(1);
    }
    if (argc == 4)
    {
        
    }
    struct stat statbuf;
    if(stat(argv[2], &statbuf) == 0){
        perror("stat");
        exit(1);
    }
    else{
        int check = creat(argv[2], S_IRUSR | S_IWUSR);
        if(check == -1){
            perror("creat");
            exit(1);
        }
    }
    if (argc == 3)
{
    copyMethod(argv[1],argv[2],BUFSIZ);
}
    else
    {
    
    Timer t;
    double eWall;
    double eUser;
    double eSystem;
    t.start();
      streamMethod(argv[1], argv[2]);

    t.elapsedWallclockTime(eWall);
    t.elapsedUserTime(eUser);
    t.elapsedSystemTime(eSystem);   


      copyMethod(argv[1], argv[2], 1);
      copyMethod(argv[1],argv[2],BUFSIZ);
    }

    

    return 0;
}
