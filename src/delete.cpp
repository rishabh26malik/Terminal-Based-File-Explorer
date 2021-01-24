#include "header.h"


extern int refresh;     //----
extern unordered_map <string,int> path2list;	//----
extern vector <vector<string>> allDirList;	//----
extern int mode, cursor_pos;            
extern string pwd;	//----

void removeFileEntry(string path){
	int index, flag=0;
	string filename="";
	vector<string>::iterator it;
	int i=path.size()-1;
	while(path[i]!='/'){
		filename=path[i]+filename;
		i--;
	}
	string parent=path.substr(0,i+1);
	index=path2list[parent];

	for(it=allDirList[index].begin(); it!=allDirList[index].end(); it++){
		if(*it == filename){
			flag=1;
			cout<<*it<<"  found file to be deleted....\n";
			break;
		}
	}
	if(flag==1)
		allDirList[index].erase(it);

	if(pwd==parent){
		refresh=1;
	}
}

void removeDirEntry(string path){
	int index, flag=0;
	string DIRname="";
	vector<string>::iterator it;
	int i=path.size()-1;
	while(path[i]!='/'){
		DIRname=path[i]+DIRname;
		i--;
	}
	string parent=path.substr(0,i+1);
	index=path2list[parent];

	for(it=allDirList[index].begin(); it!=allDirList[index].end(); it++){
		cout<<*it<<endl;
		if(*it == DIRname){
			flag=1;
			cout<<*it<<"  found DIR to be deleted....\n";
			break;
		}
	}
	if(flag==1)
		allDirList[index].erase(it);
	if(pwd==parent){
		cout<<pwd<<"   "<<parent<<endl;
		refresh=1;
	}
}

void delete_one_dir(char *path, string Path){
	cout<<"delete_one_dir..."<<Path<<endl;
	DIR *directory;
	dirent *fileInDir;
	directory=opendir(path);
	if(directory){
		while((fileInDir=readdir(directory))!=NULL){
			string fileName(fileInDir->d_name);
			string subPath=Path+fileName;
			string DirName=subPath+'/';
			char subpath[100];
			strcpy(subpath, subPath.c_str());
			if(fileName[0]!='.'){
				if(File_OR_Dir(subPath)==2){	//file
					int fd=remove(subpath);
					if(fd==-1){
						cout<<"error in deleting file "<<endl;
						exit(0);
					}
					removeFileEntry(subPath);
				}
				else{
					strcpy(subpath, DirName.c_str());
					delete_one_dir(subpath, DirName);
				}
			}
		}
		closedir(directory);
		int fd=rmdir(path);
		if(fd==-1){
			cout<<"error in deleting dir "<<endl;
			exit(0);
		}
	}
}

void deleteFile(vector <string> cmds){
	int i,n=cmds.size();
	string path=getPath(cmds[n-1]);
	i=1;
	while(i<n){
		//string path=dest+cmds[i];				// CORRECT IT-------NO NEED NOW-----------full path can also be given
		cout<<"delete file path - "<<path<<endl;
		char Path[100];
		strcpy(Path, path.c_str());
		int fd=remove(Path);
		if(fd==-1){
			cout<<"error in deleting file "<<cmds[i]<<endl;
			exit(0);
		}
		removeFileEntry(path);			// removes file's entru from its folder's vector
		i++;
	}
}


void deleteDir(vector <string> cmds, int &start, int &end ){
	string path,parent="";
	int i,n=cmds.size();
	i=1;
	while(i<n){
		path=getPath(cmds[i]);
		cout<<"delete Dir SOURCE path - "<<path<<endl;
		char Path[100];
		strcpy(Path, path.c_str());
		delete_one_dir(Path,path);
		removeDirEntry(path);
		i++;
	}
	i=path.length()-2;
	while(path[i]!='/'){
		i--;
	}
	parent=path.substr(0,i+1);
	if(pwd==parent){
		refresh=1;
	}
}
