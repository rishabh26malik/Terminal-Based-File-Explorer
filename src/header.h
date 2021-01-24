#include<iostream>
#include <sys/ioctl.h> 
#include<bits/stdc++.h>
#include<stdlib.h>
#include<fstream>
#include<iostream>
#include<dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include<string>
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h> 
#include <errno.h>  
#include <sys/wait.h>
using namespace std;

/*
extern string HOME, HOME_without_slash;
extern struct stat orig_file;
extern int refresh;		
extern stack <string> back_track, frwd_track;
extern int curr_idx;
extern unordered_map <string,int> path2list;
extern vector <vector<string>> allDirList;
extern vector <string> DirList;
extern int mode, cursor_pos;			
extern int row_len, dir_len,idx;
extern string pwd;
extern struct winsize size;
*/


void Clear(stack <string> &S);
int File_OR_Dir(string);
int getch(void);
void printDetails(string file);
void display(int start, int end);
void openDir(char path[], string newpath);
void changeDirectory(string newPath, int &start, int &end);
void tokenize(string command, vector <string> &cmds);
string getPath(string path);
void updatePermissions(char source[], char destn[]);
void copyFile(string src, string dest, string filename);
void copyDirectory(string src, char SRC[], string dest, char DEST[]);
void copyCmd(vector <string> cmds);
void removeFileEntry(string path);
void removeDirEntry(string path);
void delete_one_dir(char *path, string Path);
void move(vector <string> cmds);
void createFile(vector <string> cmds);
void createDir(vector <string> cmds, int &start, int &end);
void deleteFile(vector <string> cmds);
void deleteDir(vector <string> cmds, int &start, int &end );
void searchRecusively(string Path, string name, bool isfile, bool &found);
string search(vector <string> cmds);
void goto_(vector <string> cmds, int &start, int &end);
void rename(vector <string> cmds);
string execute_cmd(string command, int &start, int &end);
void navigate();
