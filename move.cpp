#include "header.h"

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
extern string pwd;	//---
extern struct winsize size;

void move(vector <string> cmds){
	int i,n=cmds.size();
	i=1;
	string dest=getPath(cmds[n-1]);
	while(i<n-1){
		string path=pwd+cmds[i];
		char Path[100];
		if(File_OR_Dir(path)==2){	//file copy
			strcpy(Path, path.c_str());
			copyFile(path, dest, cmds[i]);
			int fd=remove(Path);
			if(fd==-1)
				cout<<"error in deleting FILE "<<cmds[i]<<endl;
			removeFileEntry(path);
		}
		else{
			char SRC[200], DEST[200];
			strcpy(SRC, path.c_str());
			dest+=cmds[i];
			strcpy(DEST, dest.c_str());
			copyDirectory(path, SRC, dest, DEST);
			delete_one_dir(SRC, path);
			removeDirEntry(path);
		}
		i++;
	}
}