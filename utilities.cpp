#include "header.h"

extern string HOME, HOME_without_slash;	//---
extern int curr_idx;	//---
extern unordered_map <string,int> path2list;	//---
extern vector <vector<string>> allDirList;	//---
extern int mode, cursor_pos;     //---      
extern int row_len, dir_len,idx;	//---
extern string pwd;		//---			
extern struct winsize size;

void Clear(stack <string> &S){
	while(!S.empty())
		S.pop();
}

int File_OR_Dir(string path){			// 2-file , 1-dir
	struct stat sb;
	char curr_path[200];
	strcpy(curr_path, path.c_str());
	if (stat(curr_path, &sb) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }
    switch (sb.st_mode & S_IFMT) {
    case S_IFDIR:  return 1;
    		        break;
    case S_IFREG:  return 2;
                    break;
    }
}

int getch(void)
{
    int ch;
    struct termios oldt;
    struct termios newt;
    tcgetattr(STDIN_FILENO, &oldt); 
    newt = oldt;  
    newt.c_lflag &= ~(ICANON | ECHO); 
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar(); 
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); 
    return ch; 
}

void printDetails(string file){
    struct stat sb;
    char curr_path[200];
    string file_path=pwd+file;
    strcpy(curr_path, file_path.c_str());
    if (stat(curr_path, &sb) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }
    printf((sb.st_mode & S_IRUSR)? "r":"-");
    printf((sb.st_mode & S_IWUSR)? "w":"-");
    printf((sb.st_mode & S_IXUSR)? "x":"-");
    printf(" ");
    printf((sb.st_mode & S_IRGRP)? "r":"-");
    printf((sb.st_mode & S_IWGRP)? "w":"-");
    printf((sb.st_mode & S_IXGRP)? "x":"-");
    printf(" ");
    printf((sb.st_mode & S_IROTH)? "r":"-");
    printf((sb.st_mode & S_IWOTH)? "w":"-");
    printf((sb.st_mode & S_IXOTH)? "x":"-");
    printf("\t");
    struct passwd *pw = getpwuid(sb.st_uid);
    struct group  *gr = getgrgid(sb.st_gid);
    if(pw != 0){
        cout<<pw->pw_name;
    }
    printf("\t");
    if(gr!=0){
        cout<<gr->gr_name;
    }
    printf("\t");
    stringstream ss;
    ss << ctime(&sb.st_mtime);
    string ts = ss.str();
    string mod_time="";
    int i=0,len=ts.length();
    while(i<len-1){
        mod_time+=ts[i];
        i++;
    }
    cout<<float(sb.st_size/1024)<<"kB";
    printf("\t");
    printf("\t");
    cout<<mod_time;
    printf("\t");
    cout<<file<<endl;
}

void display(int start, int end){
    cout<<"\033c";//clear screen
    while(start < end){
        printDetails(allDirList[idx-1][start]);
        start++;
    }
} 

void openDir(char path[], string newpath){
    DIR *directory;
    dirent *fileInDir;
    directory=opendir(path);
    vector <string> tmp;
    tmp.push_back(".");
    tmp.push_back("..");
    while((fileInDir=readdir(directory))!=NULL){
        string fileName(fileInDir->d_name);
        if(fileName[0]!='.' )
            tmp.push_back(fileName);
    }
    allDirList.push_back(tmp);
    tmp.clear();
    path2list[newpath]=idx;
    curr_idx=idx;
    idx++;
}

void changeDirectory(string newPath, int &start, int &end){
    char NewDirPath[200];
    //string newPath=pwd+allDirList[idx-1][cursor_pos]+'/';
    strcpy(NewDirPath, newPath.c_str());
    if(chdir(NewDirPath)<0){
        cout<<"error"<<endl;
        exit(0);
    }
    openDir(NewDirPath,newPath);
    //back_track.push(newPath);
    //dir_len=allDirList[idx-1].size();     //   uncomment -----------
    dir_len=allDirList[curr_idx].size();

    pwd=newPath;
    start=0;
    row_len=size.ws_row-4;
    cursor_pos=(dir_len > row_len) ? row_len : dir_len;
    end=cursor_pos;
    cursor_pos=0;
    display(start,end);
    cout << "\033[H";
}

void tokenize(string command, vector <string> &cmds){
    //cout<<"in tokenize...\n";
    
    command+=" ";
    string str="";
    int i,n=command.length();
    for(i=0;i<n;i++){
        if(command[i]!=' '){
            str+=command[i];
        }
        else{
            cmds.push_back(str);
            str="";
        }
    }
}

string getPath(string path){
    //cout<<"in get path...\n";
    string dest="";
    if(path[0]=='/' && path[1]=='h' && path[2]=='o' && path[3]=='m' && path[4]=='e'){
        dest=path;
    }
    else if(path[0]=='~'){
        dest=HOME_without_slash+path.substr(1,path.length());
    }
    else if(path[0]=='/'){
        dest=HOME_without_slash+path;   
    }
    else if(path[0]=='.' && path.length()==1){
        dest=pwd;
    }
    else if(path[0]=='.'){
        dest=pwd+path.substr(2,path.length());
    }
    else{
        dest=pwd+path.substr(1,path.length());
    }
    return dest;
}

void updatePermissions(char source[], char destn[]){
    mode_t mode = 0;
    struct stat dest, src;
    if (stat(destn, &dest) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }
    if (stat(source, &src) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }
    
    if( src.st_mode & S_IRUSR )
        mode |= 0400;
    if( src.st_mode & S_IWUSR )
        mode |= 0200;
    if( src.st_mode & S_IXUSR )
        mode |= 0100;
    if( src.st_mode & S_IRGRP )
        mode |= 0040; 
    if( src.st_mode & S_IWGRP )
        mode |= 0020; 
    if( src.st_mode & S_IXGRP )
        mode |= 0010; 
    if( src.st_mode & S_IROTH )
        mode |= 0004; 
    if( src.st_mode & S_IWOTH )
        mode |= 0002;
    if( src.st_mode & S_IXOTH )
        mode |= 0001;
    chmod(destn, mode);
}