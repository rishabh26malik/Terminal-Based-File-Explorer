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

void navigate(){
	int flag=0;
	int start=0,end;
	char key;
	string last_cmd_exec;
	//struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
	row_len=size.ws_row-4;
	cursor_pos=(dir_len > row_len) ? row_len : dir_len;
	
	end=cursor_pos;
	cursor_pos=0;
	display(start, end);
	cout << "\033[H";
	while(1){
		
		//cout<<"at label...\n";
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
		row_len=size.ws_row-4;
		if(mode==1)		// normal mode
		{
			label:		// comes back after GOTO 
			key=getch();
			if(key=='k'){
				if(cursor_pos > start){
						cursor_pos--;
						printf("\033[1A");
					}
					else if(cursor_pos == start && start > 0){
						flag=0;
						start--;
						cursor_pos--;
						end--;
						display(start,end);
						int tmp=row_len;
						while(tmp--){
							cout<<"\033[1A";	
						}						
					}
			}
			else if(key=='l'){
				if(cursor_pos < end){
						//if(start>0)
						flag=0;
						cursor_pos++;
						printf("\033[1B");
					}
					else if(cursor_pos == end && end < dir_len){
						flag=1;
						end++;
						cursor_pos++;
						start++;
						display(start,end);
						cout<<"\033[1A";
					}
			}
			else if(key==27){
				key=getch();
				key=getch();
				if(key=='A'){
					if(cursor_pos > start){
						cursor_pos--;
						printf("\033[1A");
					}
					else if(cursor_pos == start && start > 0){
						flag=0;
						start--;
						cursor_pos--;
						end--;
						display(start,end);
						int tmp=row_len;
						while(tmp--){
							cout<<"\033[1A";	
						}						
					}					
				}
				else if(key=='B'){
					if(cursor_pos < end){
						//if(start>0)
						flag=0;
						cursor_pos++;
						printf("\033[1B");
					}
					else if(cursor_pos == end && end < dir_len){
						flag=1;
						end++;
						cursor_pos++;
						start++;
						display(start,end);
						cout<<"\033[1A";
					}
					
				}	
				else if(key=='C'){		// right
					flag=0;
					if(!frwd_track.empty()){
						back_track.push(pwd);
						string newPath=frwd_track.top();
						frwd_track.pop();
						pwd=newPath;
						changeDirectory(newPath, start, end);
					}
				}
				else if(key=='D'){		// left
					flag=0;
					if(!back_track.empty()){
						frwd_track.push(pwd);
						string newPath=back_track.top();
						back_track.pop();
						pwd=newPath;
						changeDirectory(newPath, start, end);
					}
				}
			}
			else if (key==127 || key==8){		// backspace
				//cout<<"in backspace....\n";
				flag=0;
				if(pwd!=HOME){
					Clear(frwd_track);
					int i=pwd.length()-2;
					while(i>=0 && pwd[i]!='/')
						i--;

					string parent=pwd.substr(0,i+1);
					cout<<pwd<<"   "<<parent<<endl;
					changeDirectory(parent, start, end);
					pwd=parent;
				}
			}
			else if (/*key==104 || key==72*/ key=='h'){		// home
				//cout<<"in home....\n";
				flag=0;
				if(pwd!=HOME){
					Clear(frwd_track);
					string newPath=pwd;
					back_track.push(pwd);
					changeDirectory(HOME, start, end);
					pwd=HOME;
				}
			}
			else if( key==10 ){								//  ENTER KEY
				int toEnter=(flag==1) ? cursor_pos-1:cursor_pos;
				//cout<<"in enter block ---- "<<allDirList[idx-1][toEnter]<<" "<<flag<<endl;
				//cout<<pwd+allDirList[idx-1][toEnter]<<endl;
				
				if(File_OR_Dir(pwd+allDirList[idx-1][toEnter])==2){		//FILE opening
					//cout<<"in enter FILE block\n";
					/*if (fork() == 0) {
						execl("/usr/bin/xdg-open", "xdg-open", (allDirList[idx-1][toEnter]).c_str(), (char *)0);
						exit(1);
					}*/
					char openFile[200];
					strcpy(openFile,allDirList[idx-1][toEnter].c_str());
					int fileOpen=open(openFile,O_WRONLY);
					dup2(fileOpen,2);
					close(fileOpen);
					pid_t processID = fork();
					if(processID == 0)
					{
						execlp("xdg-open","xdg-open",openFile,NULL);
						exit(0);
					} 
				}
				else{											// directory changeallDirList[idx-1][toEnter]
					char NewDirPath[200];
					string newPath=pwd+allDirList[idx-1][toEnter]+'/';
					strcpy(NewDirPath, newPath.c_str());
					if(chdir(NewDirPath)<0){
						cout<<"error"<<endl;
						exit(0);
					}
					back_track.push(pwd);
					//openDir(NewDirPath,newPath);  //   uncomment it ----------
					changeDirectory(newPath, start, end);
					cout << "\033[H";
				}
			}
			else if(key==':'){
				CMD_MODE:
				ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
				int goDown=size.ws_row-2, cursor=cursor_pos;
				int tmp=cursor;
				while(cursor<goDown){
					//cout<<"_";
					printf("\033[1B");
					cursor++;	
				}
				cout<<": ";
				mode=2;					// mode = 2 -->> command mode
				string command="";
				char ch;
				int store[3];
				store[0]=start;
				store[1]=end;
				store[2]=cursor_pos;
				while(ch=getch()){
					if(ch==27){
						mode=1;
						display(start,end);
						cout << "\033[H";
						goto label;
					}					
					else if(ch==127){
						cout<<"\b \b";			
						command.pop_back();	
					}
					else if(ch!=27 && ch!=10){
						cout<<ch;
						command.push_back(ch);	
					}
					else if(ch==10){
						last_cmd_exec=execute_cmd(command,start,end);
						if(last_cmd_exec=="goto"){		// goto ko bhi check krr for after execution, cursor stuck problem
							mode=1;
							goto label;
						}
						if(refresh==1){
							display(start,end);
							refresh=0;
							cursor_pos=0;
							goto CMD_MODE;
						}
						else{
							int len=command.length();
							while(len--){
								cout<<"\b \b";	
							}
							command="";	
						}
					}
					
				}
			}
		
		}
	}
			
}
