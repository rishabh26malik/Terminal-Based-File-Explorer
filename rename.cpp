#include "header.h"

extern string pwd;

void rename(vector <string> cmds){
	char src[100],dest[100];
	cmds[1]=pwd+cmds[1];
	cmds[2]=pwd+cmds[2];
	strcpy(src, cmds[1].c_str());
	strcpy(dest, cmds[2].c_str());
	int fd=rename(src,dest);
	if(fd==-1)
			cout<<"error in renaming file "<<cmds[1]<<endl;
}