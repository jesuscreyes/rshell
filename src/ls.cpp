#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

#include <iostream>

using namespace std;

/* This is a BARE BONES example of how to use opendir/readdir/closedir.  Not * ice that there is no error checking on these functions.  You MUST add err * orchecking yourself.
 */

int main()
{

    char *dirName = ".";
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
            cout << cnt << ": ";
            cnt++;
            cout << direntp->d_name << endl;

            struct stat statbuf;
            if(stat(direntp->d_name, &statbuf)  == 1){
		perror("stat");
	    }
            else{
	    	cout << "size: " << statbuf.st_size << endl;
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
		    cout << "-";
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
	        cout << endl; 
	    }
        }
    }
    int check = closedir(dirp);
    if(check == -1){
        perror("closedir");
    }

    return 0;
}
