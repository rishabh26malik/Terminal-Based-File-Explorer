#include "header.h"

extern string pwd;			//---

void searchRecusively(string Path, string name, bool isfile, bool &found){
	if(found)
		return;
	DIR *directory;
	dirent *fileInDir;
	char path[200];
	strcpy(path, Path.c_str());
	directory=opendir(path);
	if(directory){
		while((fileInDir=readdir(directory))!=NULL){
			string fileName(fileInDir->d_name);
			string subPath=Path+fileName;
			string DirPath=subPath+'/';
			char subpath[100];
			strcpy(subpath, subPath.c_str());
			if(fileName[0]!='.'){
				if(File_OR_Dir(subPath)==2){	//file
					if(isfile && fileName==name){
						found=true;
						cout<<"  ***FILE FOUND************************";
						return;
					}
				}
				else if(File_OR_Dir(DirPath)==1){
					if(!isfile && fileName==name){
						found=true;
						cout<<"  ***DIR FOUND****************";
						return;		
					}
					searchRecusively(DirPath, name , isfile, found);
				}
			}
		}
		closedir(directory);
	}
}

string search(vector <string> cmds){
	bool file, found=false;
	string searchName=cmds[1];
	string path=pwd;
	if(searchName[searchName.length()-1]=='/'){
		searchName.pop_back();
	}
	if(cmds[1][cmds[1].length()-1]!='/'/*File_OR_Dir(path)==2*/){		// file
		file=true;
	}		
	else{
		file=false;
	}
	searchRecusively(pwd,searchName,file, found);
	if(!found)
		cout<<"  ***NOT FOUND************************";
}