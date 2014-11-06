#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>

#include <iostream>

using namespace std;


//Global Variables
    bool lsFlag = false;
    bool aFlag = false;
    bool lFlag = false;
    bool rFlag = false;
int numBlocks(dirent *direntp){
    
    int num;
    struct stat statbuf2;
    if(stat(direntp->d_name, &statbuf2) == 1){
        perror("stat");
        exit(1);
    }
    else{
        num =  statbuf2.st_blocks/2;
    }
    return num;
}


void longList(dirent *direntp){
            struct stat statbuf;
            
            if(stat(direntp->d_name, &statbuf)  == 1){
		        perror("stat");
	        }
            else{
                //Outputs Permissions
                if(S_ISDIR(statbuf.st_mode)){
                	cout << "d";
            	}
            	else{
                	cout << "-";
                }
            	if(statbuf.st_mode & S_IRUSR){
		            cout << "r";
	            }
	   	        else{
		            cout << "-i";
	            }
		        if(statbuf.st_mode & S_IWUSR){
		            cout << "w";
		        }
		        else{
		            cout << "-";
		        }
		        if(statbuf.st_mode & S_IXUSR){
		            cout << "x";
		        }
		        else{
		            cout << "-";
		        }
		        if(statbuf.st_mode & S_IRGRP){
		            cout << "r";
		        }
		        else{
		            cout << "-";
		        }
		        if(statbuf.st_mode & S_IWGRP){
		            cout << "w";
		        }
		        else{
		            cout << "-";
		        }
		        if(statbuf.st_mode & S_IXGRP){
		            cout << "x";
		        }
		        else{
		            cout << "-";
		        }   
		        if(statbuf.st_mode & S_IROTH){
		            cout << "r";
		        }
		        else{
		            cout << "-";
		        }
		        if(statbuf.st_mode & S_IWOTH){
		            cout << "w";
		        }
		        else{
		            cout << "-";
		        }
		        if(statbuf.st_mode & S_IXOTH){
		            cout << "x";
		        }
		        else{
		            cout << "-";
		        }
	            cout << " "; 
	            
                //Outputs # of hard links
                cout << statbuf.st_nlink
                     << " ";

                //Outputs owner name
                cout << statbuf.st_uid
                     << " ";

                //Outputs group name
                cout << statbuf.st_gid
                     << " ";

                //Outputs Size
	    	    cout << statbuf.st_size
                     << " ";

                //Outputs timestampi
                //cout << statbuf.st_mtime << " ";
                time_t t = statbuf.st_mtime;
                struct tm lt;
                localtime_r(&t, &lt);
                char timbuf[80];
                strftime(timbuf, sizeof(timbuf), "%b %e %I:%M", &lt); 
                cout << timbuf << " "; 
                
                cout << direntp->d_name;
                cout << endl;
            }
}

bool isDir(char const *temp){
    char const *dirName = temp;
    DIR *dirp = opendir(dirName);
    if(dirp == NULL){
        return false;
    }
    else{
        closedir(dirp); 
       return true;
    }
}

void executeCmd(char const *temp){
        char const *dirName = temp;    
        DIR *dirp = opendir(dirName);
        if(dirp == NULL){
	        perror("opendir");
            exit(1);
        }

///////////////////////////////
        dirent *direntp;
        if(lFlag){
        int total = 0;
        while((direntp = readdir(dirp))){
            if(lsFlag){
                if(lFlag){
                    if(direntp->d_name[0] != '.'){
                        total += numBlocks(direntp);
                    }
                }
            }
        }
        cout << "total " << total << endl; 
    }
//////////////////////////////   
        //Re-assignment of dirp variable, so that we can go through the directory againi.
        dirp = opendir(dirName); 
        while ((direntp = readdir(dirp))){
            if(direntp == NULL){
                perror("readdir");
            }
            else{
                if(lsFlag){
                    //cout << "lsFlag is set" << endl;
                    if(lFlag){
                        //Condition that makes it so only public files are displayed
                        if(direntp->d_name[0] != '.'){
                            //cout << "name before findBlocks: " << direntp->d_name << endl;
                            
                            //int total = 0;
                            //total = findBlocks();
                            //cout << "name after findBlocks: " << direntp->d_name << endl;
                            //cout << "total after findBlocks" << total << endl;
                            longList(direntp);
                        }
                    }
                    else if(aFlag){
                        cout << direntp->d_name << " ";
                    }
                    else{
                        if(direntp->d_name[0] != '.'){
                            cout << direntp->d_name << " ";
                        }
                    }
                }
            }
        }
        cout << endl;
    
        int check = closedir(dirp);
        if(check == -1){
            perror("closedir");
        }
    }

#define Flag_ls 1
#define Flag_a 2
#define Flag_l 4
#define Flag_R 8

int main(int argc, char**argv)
{
    //cout << "argc: " << argc << endl;

    if(argc == 1){
        cout << "Error. Not enough arguments." << endl;
        exit(1);
    }
            
    int flags = 0;

    if(argv[1][0] == 'l'){
        if(argv[1][1] == 's'){
            lsFlag= true;
        }
        else{
            cout << "Error yo. Where's the s?!" << endl;
            exit(1);
        }
    }
    else{
        cout << "Error yo. That's not l bro!" << endl;
        exit(1);
    }

/////////////////////////////////
//Check if input is a file
/////////////////////////////////


    if(argc >= 2){
        for(int i = 2; i < argc; i++){
            if(argv[i][0] == '-'){
                //for(int j = 1; argv[i][j] != 0; j++){
                    //if(argv[i][j] == 'a'){
                    if(argv[i][1] == 'a'){
                        aFlag = true; 
                        
                        //Check for other flags
                        //Intention to be able to
                        //handle '-alR' type cases
                    }
                    else if(argv[i][1] == 'l'){
                        lFlag = true;
                    }
                    else if(argv[i][1] == 'R'){
                        rFlag = true;
                    }
                    else{
                        cout << "Error. Invalid flag(s)." << endl;
                        exit(1);
                    }
               //}
            }
            else{
                cout << "Should be error"<< endl;
            }
        }
    }


    const char* s = ".";
    //if(isDir(s)){
        //cout << "It's a directory" << endl;
    //}
    //else{
        //cout << "It's not a directory" << endl;
    //}
    executeCmd(s);
    //executeCmd("ls.cpp", 2);
    return 0;
}
