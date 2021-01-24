#include "header.h"

extern int refresh;     
extern unordered_map <string,int> path2list;
extern vector <vector<string>> allDirList;
extern vector <string> DirList;
extern int  cursor_pos;            
extern int row_len, dir_len,idx;
extern string pwd;
extern struct winsize size;

void createFile(vector <string> cmds){
	int i,n=cmds.size();
	i=1;
	string dest=getPath(cmds[n-1]);
	while(i<n-1){
		string path=dest+cmds[i];
		cout<<path<<endl;
		char Path[100];
		strcpy(Path, path.c_str());
		int fd=open(Path,O_RDONLY | O_CREAT| O_WRONLY);
		if(fd==-1)
			cout<<"error in creating file "<<cmds[i]<<endl;
		i++;
		close(fd);
		chmod(Path,S_IRUSR|S_IWUSR);
	}
}


void createDir(vector <string> cmds, int &start, int &end){
	int i,n=cmds.size(), idx;
	i=1;
	string path, dest=getPath(cmds[n-1]);
	idx=path2list[dest];
	i=dest.size()-1;
	while(dest[i]!='/'){
		i--;
	}
	string parent=dest.substr(0,i+1);
	int index=path2list[parent];
	i=1;
	for(auto x : allDirList[index])
		cout<<x<<endl;
	while(i<n-1){
		path=dest+cmds[i];
		char Path[100];
		strcpy(Path, path.c_str());
		int fd=mkdir(Path,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP |S_IROTH|S_IXUSR);
		if(fd==-1)
			cout<<"error in creating directory "<<cmds[i]<<endl;
		if(pwd==parent){
			allDirList[index].push_back(cmds[i]);	
		}
		i++;
	}
	if(pwd==parent){
		dir_len=allDirList[index].size();
		start=0;
		row_len=size.ws_row-4;
		cursor_pos=(dir_len > row_len) ? row_len : dir_len;
		end=cursor_pos;
		cursor_pos=0;
		refresh=1;
	}
}
