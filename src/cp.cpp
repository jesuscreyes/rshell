#include <stdio.h>
#include <string.h>
#include <fstream>
#include <dirent.h>
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
    //char buf[count];
    char *buf;
    buf = new char[count];
    int test = 1;
    
    
    do{
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

bool flag = false;

    if(argc < 3 || argc > 4){
        cerr << "Inappropriate number of arguments" << endl;
        exit(1);
    }
    if (argc == 4)
    {
       for(int i = 1; i < argc; i++){
            if(argv[i][0] == '-'){
                if(argv[i][1] == 'f'){
                    flag = true;
                }
                else{
                    cerr << "Error: Wrong flag" << endl;
                    exit(1);
                }  
            }
        }
    }

string input;
string output;

    struct stat statbuf;
    for(int i = 1; i < argc; i++){
        if(argv[i][0] != '-'){
            if(stat(argv[i], &statbuf) == -1){
                cout << "argv[i]: " << argv[i] << endl;
                perror("stat1");
                exit(1);
            }
            input = argv[i];
            break;
        }
    }
    for(int i = 2; i < argc; i++){
            if(argv[i][0] != '-' && argv[i][0] != '/'){

                //Check if file already exists
                char const *dirName = ".";
                DIR *dirp = opendir(dirName);
                dirent *direntp;
                while((direntp = readdir(dirp))){
                    if(direntp->d_name[0] == argv[i][0]){
                        int j;
                        for(j = 1; i < abs(strlen(argv[i])); j++){
                                if(direntp->d_name[j] == 1 &&
                                    argv[i][j] == 0){
                                    cout << "Error: Output file already exists" << endl;
                                exit(1);
                                }
                                else{
                                    break;
                                }
                        }
                    }
                }

                int check = creat(argv[i], S_IRUSR | S_IWUSR);
                if(check == -1){
                    perror("creat");
                    exit(1);
                }
                else{
                    output = argv[i];
                }
            }
    }
        //}

    if (argc == 3)
    {
        copyMethod(input,output, BUFSIZ);
    }
    else if(flag)
    {
    
    Timer t1;
    Timer t2;
    Timer t3;
    double eWall;
    double eUser;
    double eSystem;
    t1.start();
      streamMethod(input, output);

    t1.elapsedWallclockTime(eWall);
    t1.elapsedUserTime(eUser);
    t1.elapsedSystemTime(eSystem);   

    cout << "Method 1" << endl;
    cout << "wallclock: " << eWall << endl;
    cout << "user: " << eUser << endl;
    cout << "system: " << eSystem << endl << endl;

    t2.start();
    copyMethod(input, output, 1);
    t2.elapsedWallclockTime(eWall);
    t2.elapsedUserTime(eUser);
    t2.elapsedSystemTime(eSystem);
    cout << "Method 2" << endl;
    cout << "wallclock: " << eWall << endl;
    cout << "user: " << eUser << endl;
    cout << "system: " << eSystem << endl << endl;

    t3.start();
    copyMethod(input, output, BUFSIZ);
    t3.elapsedWallclockTime(eWall);
    t3.elapsedUserTime(eUser);
    t3.elapsedSystemTime(eSystem);
    cout << "Method 3" << endl;
    cout << "wallclock: " << eWall << endl;
    cout << "user: " << eUser << endl;
    cout << "system: " << eSystem << endl << endl;
    }

    

    return 0;
}
