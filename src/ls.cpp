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
            stat(direntp->d_name, &statbuf);
            cout << "size: " << statbuf.st_size << endl;
            if(S_ISDIR(statbuf.st_mode)){
                cout << "d-" << endl << endl;
            }
            else{
                cout << "--" << endl << endl;
                }
        }
    }
    
    int check = closedir(dirp);
    if(check == -1){
        closedir(dirp);
    }   
}
