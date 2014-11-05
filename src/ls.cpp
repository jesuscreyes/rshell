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

/* This is a BARE BONES example of how to use opendir/readdir/closedir.  Not * ice that there is no error checking on these functions.  You MUST add err * orchecking yourself.
 */

void executeStat(dirent *direntp){
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

int main()
{

    bool run = true;

    string s;

    while(run){

        cout << endl << "$ ";
        getline(cin,s);
        if(s == "exit"){
            exit(1);
        }   
        else{
            
            bool lsFlag = false;
            bool aFlag = false;
            bool lFlag = false;
            bool rFlag = false;

            unsigned int i;
            for(i = 0; i < s.size(); i++){
                if(s[i] != 'l' &&
                   s[i] != 's' &&
                   s[i] != ' '){
                    cout << "First Error" << endl;
                    exit(1);
                }
                
                if(s[i] == 'l'){
                    //cout << "s[i]: " << s[i] << endl;
                    //cout << "s[i+1]: " << s[i+1] << endl;
                    if(s[i+1] != 's'){
                        cout << "Error1" << endl;
                        exit(1);
                    }
                    else{
                        lsFlag = true;
                        break;
                    }
                }
            }

            i = i + 2;
            for(; i < s.size(); i++){
                if(s[i] != ' ' &&
                   s[i] != '-' &&
                   s[i] != 'a' &&
                   s[i] != 'l' &&
                   s[i] != 'R'){
                    cout << "Error!!!" << endl;
                    cout << "s[i]: " << s[i] << endl;
                }
                else{
                   if(s[i] == '-'){
                        if(s[i+1] == 'a'){
                            aFlag = true;
                        }
                        else if(s[i+1] == 'l'){
                            lFlag = true;
                        }
                        else if(s[i+1] == 'R'){
                            rFlag = true;
                        }
                        else{
                            cout << "Error Yo" << endl;
                            exit(1);
                        }
                    }
                }
            }      

            char const *dirName = ".";    

            DIR *dirp = opendir(dirName);
            if(dirp == NULL){
	            perror("opendir");
            }
    
            dirent *direntp;
                int cnt = 1;    
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
                                executeStat(direntp);
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
    
            int check = closedir(dirp);
            if(check == -1){
                perror("closedir");
            }

        }
    }
    return 0;
}
