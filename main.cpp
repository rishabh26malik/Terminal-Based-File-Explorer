#include "header.h"


string HOME, HOME_without_slash;
struct stat orig_file;
int refresh=0;		
stack <string> back_track, frwd_track;
int curr_idx=0;
unordered_map <string,int> path2list;
vector <vector<string>> allDirList;
vector <string> DirList;
int mode=1, cursor_pos;			// 1-normal mode
int row_len, dir_len,idx=0;
string pwd;
//int start=0,end;
struct winsize size;

/*
dir_len = # of dir/files in DirList
*/
/*
extern int refresh=0;	
extern int dir_len,idx=0;
extern int curr_idx=0;
extern int mode=1;
*/
int main()
{
	struct passwd *pw = getpwuid(getuid());
	char *homedir = pw->pw_dir;
	HOME_without_slash=homedir;
	HOME=homedir;
	HOME+='/';
	pwd=HOME;
	if(chdir(homedir)<0){
		cout<<"error"<<endl;
		exit(0);
	}
	DIR *directory;
	dirent *fileInDir;
	directory=opendir(homedir);
	DirList.push_back(".");
	DirList.push_back("..");
	while((fileInDir=readdir(directory))!=NULL){
		string fileName(fileInDir->d_name);
		if(fileName[0]!='.')
			DirList.push_back(fileName);
	}
	allDirList.push_back(DirList);
	if(path2list.find(HOME)==path2list.end()){
		path2list[HOME]=idx;
		idx++;
	}
	curr_idx=idx;
	dir_len=DirList.size();
	navigate();
	return 0;
}

/**************************************************
1.) correct navigation of /home	----------------------------------------------------------------------DONE
2.) open files using vi
3.) escape KEY 	--------------------------------------------------------------------------------------DONE
4.) stat printing  -----------------------------------------------------------------------------------DONE
5.) copying mp4 file ---------------------------------------------------------------------------------DONE
6.) maintaining files permissions on copying & moving-------------------------------------------------DONE
7.) search implmentation -----------------------------------------------------------------------------DONE
8.) open correct dir in /home/subDir (one above wali open hori) --------------------------------------DONE
9.) update vector if a new file is copied or deleted in previously visited folder---------------------NO NEED, AS WE MAKE A NEW ROW IN 2D VECTOR
10.) remove vector entry if a folder is deleted - just delete its entry from map not from vector------NO NEED, AS WE MAKE A NEW ROW IN 2D VECTOR
11.) mp3 file copied but is locked -------------------------------------------------------------------DONE
12.) copy dir -----------------------------------------------------------------	XXXX NO NEED TO BE IN ASSG
13.) if dir from pwd iss deleted then  display updated list with effect	------------------------------DONE
     delete_dir function complete krr
14.) goto 	------------------------------------------------------------------------------------------DONE
15.) on deleteing on file after another, 3rd/4th file gives "stat: No such file or directory" ERROR---------leave it
     -BUT file is deleted - something wrong while displaying the directory
16.) update display if dir from pwd is deleteDir------------------------------------------------------DONE
17.) on delete_dir -> dir is deleted-> but "stat: No such file or directory" ERROR--------------------DONE
18.) dir copy-----------------------------------------------------------------------------------------DONE
19.) print ownership details -------------------------------------------------------------------------DONE
20.) resize terminal check
21.) on creating dir in pwd, update display ----------------------------------------------------------DONE
22.) after dir copy , where to...---------------------------------------------------------------------DONE
23.) move dir-----------------------------------------------------------------------------------------DONE
24.) replace "/home/rishabh/" by generic--------------------------------------------------------------DONE
-----------------------
		DONE
for deleted file -> its vector entry is deleted
for deleted DIR -> its MAP ENTRY is deleted

**************************************************/