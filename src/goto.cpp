#include "header.h"

extern stack <string> back_track, frwd_track;	//---
extern vector <vector<string>> allDirList;	//---
extern int cursor_pos;         //---   
extern int row_len, dir_len,idx;	//---
extern struct winsize size;		//---
extern string pwd;

void goto_(vector <string> cmds, int &start, int &end){
	string dest=getPath(cmds[1]);
	char Path[100];
	strcpy(Path, dest.c_str());
	if(chdir(Path)<0){
		cout<<"error"<<endl;
		exit(0);
	}
	changeDirectory(dest,start,end);
	Clear(frwd_track);			/// <<<<<<<------- CHECK THIS
	back_track.push(pwd);
	dir_len=allDirList[idx-1].size();
	pwd=dest;
	start=0;
	row_len=size.ws_row-4;
	cursor_pos=(dir_len > row_len) ? row_len : dir_len;
	end=cursor_pos;
	cursor_pos=0;
}