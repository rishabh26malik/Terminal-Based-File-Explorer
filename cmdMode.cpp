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
extern string pwd;
extern struct winsize size;


/*void move(vector <string> cmds){
	int i,n=cmds.size();
	i=1;
	string dest=getPath(cmds[n-1]);
	while(i<n-1){
		string path=pwd+cmds[i];
		char Path[100];
		strcpy(Path, path.c_str());
		copyFile(path, dest, cmds[i]);
		int fd=remove(Path);
		if(fd==-1)
			cout<<"error in deleting FILE "<<cmds[i]<<endl;
		
		i++;
	}
}*/


string execute_cmd(string command, int &start, int &end){
	vector <string> cmds;
	tokenize(command,cmds); 
	if(cmds[0]=="copy"){
		copyCmd(cmds);
		return "copy";
	}
	else if(cmds[0]=="move"){
		move(cmds);
		return "move";
	}
	else if(cmds[0]=="create_file"){
		createFile(cmds);
		return "create_file";
	}
	else if(cmds[0]=="create_dir"){
		createDir(cmds, start, end);
		return "create_dir";
	}
	else if(cmds[0]=="delete_file"){
		deleteFile(cmds);
		return "delete_file";
	}
	else if(cmds[0]=="delete_dir"){
		deleteDir(cmds, start, end);
		return "delete_dir";
	}
	else if(cmds[0]=="goto"){
		goto_(cmds, start, end);
		return "goto";
	}
	else if(cmds[0]=="rename"){
		rename(cmds);
		return "rename";
	}
	else if(cmds[0]=="search"){
		search(cmds);
		return "search";
	}
}
