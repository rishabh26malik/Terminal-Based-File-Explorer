#include "header.h"

extern struct stat orig_file;
extern string pwd;

void copyFile(string src, string dest, string filename){
	dest+=filename;
	char source[100],destn[100];
	strcpy(destn, dest.c_str());
	strcpy(source, src.c_str());
	int fd1 = open(source, O_RDONLY, 0);
	int fd2 = open(destn, O_WRONLY| O_CREAT, 0);
	char c;
	while(read(fd1, &c, 1) != 0){
		write(fd2, &c, 1);
	}
	close(fd1);
	close(fd2);
	chmod(destn,S_IRUSR|S_IWUSR);
	if (stat(source, &orig_file) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }
    updatePermissions(source,destn);
}

void copyDirectory(string src, char SRC[], string dest, char DEST[]){
	int fd=mkdir(DEST, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP |S_IROTH|S_IXUSR);
	if(fd == -1)
	 {
	 	cout<<"error in creating dir.. "<<DEST<<endl;
	 	exit(0);
	 }
	DIR *directory;
	dirent *fileInDir;
	directory=opendir(SRC);
	if(directory){
		while((fileInDir=readdir(directory))!=NULL){
			string fileName(fileInDir->d_name);
			if(fileName[0]!='.'){	
				string copyTO=dest+fileName;
				string copyFROM=src+fileName;
				char newSrc[200], newDest[200];
				strcpy(newSrc, copyFROM.c_str() );
				strcpy(newDest, copyTO.c_str() );
				if(File_OR_Dir(copyFROM)==2){	//file
					copyFile(copyFROM, copyTO, fileName);
				}
				else{
					copyTO+='/';
					copyFROM+='/';
					strcpy(newSrc, copyFROM.c_str() );
					strcpy(newDest, copyTO.c_str() );
					copyDirectory(copyFROM, newSrc, copyTO, newDest);
				}
			}
		}
		closedir(directory);
	}
}

void copyCmd(vector <string> cmds){
	int i,n=cmds.size();
	i=1;
	string path="";
	string dest=getPath(cmds[n-1]);
	while(i<n-1){
		path=pwd+cmds[i];
		if(File_OR_Dir(path)==2){	//file copy
			string src=pwd+cmds[i];
			copyFile(src,dest,cmds[i]);
		}
		else{						// dir copy -    DO WE HAVE TO COPY ALL CONTENTS OF DIR RECURSIVELY
			char SRC[200], DEST[200];
			dest+=cmds[i];
			strcpy(SRC, path.c_str());
			strcpy(DEST, dest.c_str());
			copyDirectory(path, SRC, dest, DEST);
		}
		i++;
	}
}
