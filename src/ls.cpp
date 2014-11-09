#include <sys/stat.h>
#include <string.h>
#include <iomanip>
#include <vector>
#include <pwd.h>
#include <grp.h>
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
    bool lsFlag = true;
    bool aFlag = false;
    bool lFlag = false;
    bool rFlag = false;
int numBlocks(const dirent *direntp){
    
    int num;
    struct stat statbuf;
    if(stat(direntp->d_name, &statbuf) == 1){
        perror("stat");
        exit(1);
    }
    else{
        num =  statbuf.st_blocks/2;
    }
    return num;
}


void longList(dirent *direntp){
            struct stat statbuf;
            if(stat(direntp->d_name, &statbuf)  == 1){
		        perror("stat");
	        }
            else{
                //cout << "44: " << direntp->d_name << endl;
                //Outputs Permissions
                if(S_ISLNK(statbuf.st_mode)){
                    cout << "l";
                }
                else if(S_ISDIR(statbuf.st_mode)){
                	cout << "d";
            	}
                //else if(S_ISLNK(statbuf.st_mode)){
                    //cout << "l";
                //}
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
	            cout << " "; 
                //Outputs # of hard links
                cout << statbuf.st_nlink
                     << " ";

                //Outputs owner name
                struct passwd *p;
             
                p = getpwuid(statbuf.st_uid);
                if(p == NULL){
                    perror("getpwuid");
                    exit(1);
                }
                cout << p->pw_name << " " ;
 
                //Outputs group name
                struct group *g;
                g = getgrgid(statbuf.st_gid);
                if(g == NULL){
                    perror("getgrgid");
                    exit(1);
                }
                cout << g->gr_name << " ";

                //Outputs Size
	    	    cout << statbuf.st_size
                     << " ";

                //Outputs timestamp
                //cout << statbuf.st_mtime << " ";
                time_t t = statbuf.st_mtime;
                struct tm lt;
                localtime_r(&t, &lt);
                char timbuf[80];
                int check = strftime(timbuf, sizeof(timbuf), "%b %e %I:%M", &lt); 
                if(check == 0){
                    cerr << "Possible error: strftime returned '0'" << endl;
                }
                cout << timbuf << " "; 

                    if(S_ISDIR(statbuf.st_mode)){
                        if(direntp->d_name[0] == '.'){
                            cout << "\033[0;40;34m" << direntp->d_name << "\033[0;00m" << " ";
                        }
                        else{
                            cout << "\033[0;0;34m" << direntp->d_name << "\033[0;00m" << " ";
                        }
                    }
                    else if(statbuf.st_mode & S_IXUSR){
                        if(direntp->d_name[0] == '.'){
                            cout << "\033[0;40;32m" << direntp->d_name << "\033[0;00m" << " ";
                        }
                        else{
                            cout << "\033[0;0;32m" << direntp->d_name << "\033[0;00m" << " ";
                        }
                    }
                    else if(direntp->d_name[0] == '.'){
                            cout << "\033[0;0;40m" << direntp->d_name << "\033[0;00m" << " ";
                    }
                    else{
                       cout <<  direntp->d_name << " ";
                    }

                cout << endl;
            }
}

int executeCmd(char const *temp){
    
    
    char const *dirName = temp;    
    DIR *dirp;
    DIR *dirp2;
    //dirp = opendir(dirName);
    //if(dirp == NULL){
	    //perror("opendir");
        //exit(1);
    //}

///////////////////////////////
//This block of code is used to
//acquire number of BLOCKS of
//'-l' flag is set
//////////////////////////////
    cout << "dirName: " << dirName << endl;
    dirent *direntp;

  
  if(lFlag){
        dirp = opendir(dirName);
        cout << "dirp: " << dirp << endl;
        if(dirp == NULL){
            perror("opendir");
        }
        int total = 0;
        errno = 0;
        while((direntp = readdir(dirp))){
            //cout << "name: " << direntp->d_name << endl;
            if(errno != 0){
                perror("readdir");
            
            }
            else if(lsFlag){
                if(lFlag){
                    if(direntp->d_name[0] != '.'){
                        total += numBlocks(direntp);
                    }
                }
            }
        }
        cout << "total " << total << endl; 
        int check = closedir(dirp);
        if(check == -1){
            perror("closedir");
        }
    }


//////////////////////////////   
//Re-assignment of dirp variable, so that we can go through the directory again.
//////////////////////////////

    dirp2 = opendir(dirName); 
    errno = 0;
    int cnt = 0;
    while ((direntp = readdir(dirp2))){
        if(errno != 0){
            perror("readdir");
        }
        else{
            

            if(lsFlag){
                //cout << "lsFlag is set" << endl;
                if(lFlag){
                    //Condition that makes it so only public files are displayed
                    if(aFlag){
                        longList(direntp);
                    }
                    else{
                        if(direntp->d_name[0] != '.'){
                            //cout << "direntp->d_name: " << direntp->d_name << endl;
                            longList(direntp);
                        }
                    }
                }
                else if(aFlag){
                    struct stat statbuf;
                    int statCheck = stat(direntp->d_name,&statbuf);
                    if(statCheck == -1){
                        perror("statCheck");
                        exit(1);
                     }
                    if(S_ISDIR(statbuf.st_mode)){
                        if(direntp->d_name[0] == '.'){
                            cout << "\033[0;40;34m" << direntp->d_name << "\033[0;00m";
                        }
                        else{
                            cout << "\033[0;0;34m" << direntp->d_name << "\033[0;00m";
                        }
                        cnt++;
                        if(cnt == 3){
                            cout << endl;
                            cnt = 0;
                        }
                        else{
                            cout << setw(30 - strlen(direntp->d_name));
                        }
                    }
                    else if(statbuf.st_mode & S_IXUSR){
                        cout << "\033[0;0;32m" << direntp->d_name << "\033[0;00m";
                        cnt++;
                        if(cnt == 3){
                            cout << endl;
                            cnt = 0;
                        }
                        else{
                            cout << setw(30 - strlen(direntp->d_name));
                        }
                    }
                    else if(direntp->d_name[0] == '.'){
                        cout << "\033[0;0;40m" << direntp->d_name << "\033[0;00m";
                        cnt++;
                        if(cnt == 3){
                            cout << endl;
                            cnt = 0;
                        }
                    }
                    else{
                        cout << direntp->d_name;
                        cnt++;
                        if(cnt == 3){
                            cout << endl;
                            cnt = 0;
                        }
                        else{
                            cout << setw(30 - strlen(direntp->d_name));
                        }
                    }
                    //cout << direntp->d_name << endl;
                }
                else{ 
                    struct stat statbuf;
                    int statCheck = stat(direntp->d_name,&statbuf);
                    if(statCheck == -1){
                        perror("statCheck");
                        exit(1);
                    }
                    if(direntp->d_name[0] != '.'){                   
                        if(S_ISDIR(statbuf.st_mode)){
                            cout << "\033[0;0;34m" << direntp->d_name << "\033[0;00m";
                            cnt++;
                            if(cnt == 3){
                                cout << endl;
                                cnt = 0;
                            }
                            else{
                                cout << setw(30 - strlen(direntp->d_name));
                            }
                        }
                        else if(statbuf.st_mode & S_IXUSR){
                            cout << "\033[0;0;32m" << direntp->d_name << "\033[0;00m";
                            cnt++;
                            if(cnt == 3){
                                cout << endl;
                                cnt = 0;
                            }
                            else{
                                cout << setw(30 - strlen(direntp->d_name));
                            }
                        }
                        else{
                            cout << direntp->d_name;
                            cnt++;
                            if(cnt == 3){
                                cout << endl;
                                cnt = 0;
                            }
                            else{
                                cout << setw(30 - strlen(direntp->d_name));
                            }
                        }
                    }
                }
            }
        }
    }
    cout << endl;
    
    int check = closedir(dirp2);
    if(check == -1){
        perror("closedir");
    }

/////////////////////////
    
}

int recurCmd(const char* tempR){
    int numDir = 0;
    vector<string> dir;

    char const *dirname = tempR;
    DIR *dirp = opendir(dirname);
    //ERROR CHECk
    dirp = opendir(dirname);
    dirent *direntp;
    while(direntp = readdir(dirp)){
        if(direntp == NULL){
            perror("readdir");
        }
        else{
            struct stat statbuf3;
            cout << "name: " << direntp->d_name << endl;
            if(stat(direntp->d_name, &statbuf3) == -1){
                perror("stat");
            }
            else{
                if(S_ISDIR(statbuf3.st_mode)){
                    if(direntp->d_name[1] != '.'){
                        dir.push_back(direntp->d_name);
                        cout << "Dir nam: " <<  direntp->d_name << endl;
                        numDir++;
                    }
                }
            }
        }
    }

    if(dir.size() == 0){
        return 0;
    }   
 
    for(unsigned int i =0; i < dir.size()-1; i++){
        cout << "Size of vector: " << dir.size() << endl;
        cout << dir[i] << ":" << endl;
        const char *d = dir[i].c_str();
        executeCmd(d);
        //dir.erase(dir.begin());
        cout << endl << endl;
        cout << "dir[i+1]" << dir[i+1] << endl; 
        cout << "calling recurCmd again" << endl << endl;
        recurCmd(dir[i+1].c_str());
    }
    while(dir.size() != 0){
        dir.pop_back();
    }
}

int main(int argc, char**argv)
{

/////////////////////////////////
//Local Variables
////////////////////////////////
    vector<string> dir;

/////////////////////////////////
//Check if input is a file
/////////////////////////////////


    if(argc >= 1){
        for(int i = 1; i < argc; i++){
            if(argv[i][0] == '-'){
                //for(int j = 1; argv[i][j] != 0; j++){
                    //if(argv[i][j] == 'a'){
                    if(argv[i][1] == 'a'){
                        aFlag = true; 
                        //Handles '-al' case
                        if(argv[i][2] == 'l'){
                            lFlag = true;
                            //Handles '-alR' case
                            if(argv[i][3] == 'R'){
                                rFlag = true;
                            }
                            else if(argv[i][3] != 0){
                                cerr << "Error. Invalid flag(s)." << endl;
                                exit(1);
                            }
                        }
                        //Handles '-aR' case
                        else if(argv[i][2] == 'R'){
                            rFlag = true;
                            //Handles '-aRl' case
                            if(argv[i][3] == 'l'){
                                lFlag = true;
                            }
                            else if(argv[i][3] != 0){
                                cerr << "Error. Invalid flag(s)." << endl;
                                exit(1);
                            }
                        }
                        else if(argv[i][2] != 0){
                            cerr << "Error. Invalid flag(s)." << endl;
                            exit(1);
                        }
                        
                    }
                    else if(argv[i][1] == 'l'){
                        lFlag = true;
                        //Handles '-la' case
                        if(argv[i][2] == 'a'){
                            aFlag = true;
                            //Handles '-laR' case
                            if(argv[i][3] == 'R'){
                                rFlag = true;
                            }
                            else if(argv[i][3] != 0){
                                cerr << "Error. Invalid flag(s)." << endl;
                                exit(1);
                            }
                        }
                        //Handles '-lR' case
                        else if(argv[i][2] == 'R'){
                            rFlag = true;
                            //Handles '-lRa' case
                            if(argv[i][3] == 'a'){
                                aFlag = true;
                            }
                            else if(argv[i][3] != 0){
                                cerr << "Error. Invalid flag(s)." << endl;
                                exit(1);
                            }
                        }
                        else if(argv[i][2] != 0){
                            cerr << "Error. Invalid flag(s)." << endl;
                            exit(1);
                        }
                    }
                    else if(argv[i][1] == 'R'){
                        rFlag = true;
                        //Handles '-Ra' case
                        if(argv[i][2] == 'a'){
                            aFlag = true;
                            //Handles '-Ral' case
                            if(argv[i][3] == 'l'){
                                lFlag = true;
                            }
                            else if(argv[i][3] != 0){
                                cerr << "Error. Invalid flag(s)." << endl;
                                exit(1);
                            }
                        }
                        //Handles '-Rl' case
                        else if(argv[i][2] == 'l'){
                            lFlag = true;
                            //Handles '-Rla' case
                            if(argv[i][3] == 'a'){
                                aFlag = true;
                            }
                            else if(argv[i][3] != 0){
                                cerr << "Error. Invalid flag(s)." << endl;
                                exit(1);
                            }
                        }
                        else if(argv[i][2] != 0){
                            cerr << "Error. Invalid flag(s)." << endl;
                            exit(1);
                        }

                    }
                    else{
                        cout << "Error. Invalid flag(s)." << endl;
                        exit(1);
                    }
               //}
            }
        }
    }

////////////////////////////////////////////////////////////////////
//Go through argv again and use stat function to see if regular file
////////////////////////////////////////////////////////////////////
    struct stat statbuf2;

    for(int i = 1; i < argc; i++){
        if(argv[i][0] != '-'){
        stat(argv[i], &statbuf2);
        //ERROR CHECK!!!!!!
            if(S_ISREG(statbuf2.st_mode)){
                cout << "It's a regular file" << endl;

                if(lFlag){
                    //Outputs Permissions
                    if(S_ISDIR(statbuf2.st_mode)){
                	    cout << "d";
            	    }
                    else if(S_ISLNK(statbuf2.st_mode)){
                        cout << "l";
                    }
            	    else{
                	    cout << "-";
                    }
            	    if(statbuf2.st_mode & S_IRUSR){
		                cout << "r";
	                }
	   	            else{
		                cout << "-";
	                }
		            if(statbuf2.st_mode & S_IWUSR){
		                cout << "w";
		            }
		            else{
		                cout << "-";
		            }
		            if(statbuf2.st_mode & S_IXUSR){
		                cout << "x";
		            }
		            else{
		                cout << "-";
		            }
		            if(statbuf2.st_mode & S_IRGRP){
		                cout << "r";
		            }
		            else{
		                cout << "-";
		            }
		            if(statbuf2.st_mode & S_IWGRP){
		                cout << "w";
		            }
		            else{
		                cout << "-";
		            }
		            if(statbuf2.st_mode & S_IXGRP){
		                cout << "x";
		            }
		            else{
		                cout << "-";
		            }   
		            if(statbuf2.st_mode & S_IROTH){
		                cout << "r";
		            }
		            else{
		                cout << "-";
		            }
		            if(statbuf2.st_mode & S_IWOTH){
		                cout << "w";
		            }
		            else{
		                cout << "-";
		            }
		            if(statbuf2.st_mode & S_IXOTH){
		                cout << "x";
		            }
		            else{
		                cout << "-";
		            }
	                cout << " "; 
	            
                    //Outputs # of hard links
                    cout << statbuf2.st_nlink
                         << " ";

                    //Outputs owner name
                
                    cout << statbuf2.st_uid
                         << " ";

                    //Outputs group name
                    cout << statbuf2.st_gid
                         << " ";

                    //Outputs Size
	    	        cout << statbuf2.st_size
                         << " ";

                    //Outputs timestamp
                    //cout << statbuf2.st_mtime << " ";
                    time_t t = statbuf2.st_mtime;
                    struct tm lt;
                    localtime_r(&t, &lt);
                    char timbuf[80];
                    strftime(timbuf, sizeof(timbuf), "%b %e %I:%M", &lt); 
                    cout << timbuf << " "; 
                
                    cout << argv[i];
                    cout << endl;
                }
                else{
                    cout << argv[i] << endl;
                }
            }
            else if(S_ISDIR(statbuf2.st_mode)){
                //cout << "It's a directory" << endl; 
                //cout << argv[i] << endl;
                dir.push_back(argv[i]);                

                //const char* k = argv[i];
                //executeCmd(k);
                //return 0;
            }
            else if(S_ISLNK(statbuf2.st_mode)){
                cout << "It's a symbolic link" << endl;
            }
            else{
                cout << "Error. Not an existing file/directory." << endl;
                exit(1);
            }
        }
    }


    if(dir.size() == 1){
        executeCmd(dir[0].c_str());
        return 0;
    }
    else if(dir.size() > 1){
        while(dir.size() != 0){
            cout << dir[0] << ":" << endl;
            executeCmd(dir[0].c_str());
            cout << endl << endl;
            dir.erase(dir.begin()+0);
        }
        return 0;
    }

////////////////////////////////////////////////////////////////////
        const char* s = ".";
        executeCmd(s);

/*
        cout << endl << "////////////////////////////"
             << "testing recurCmd"
             << endl;

        recurCmd(s);
        cout << "////////////////////////////" << endl; 
*/
 
return 0;    
}
