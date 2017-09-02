#define _POSIX_SOURCE
#include <unistd.h>
#undef _POSIX_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>
////////////////////////////////////
/////////////////////////////////////////////////
//TO REMOVE SPACES AND TABS IN THE COMMAND//////
//////////////////////////////////////////
////////////////////////////////////////
static void realify(char *a,char* x)
{
	int len=strlen(x);
	int flag=1;
	int i,j=0;
	for(i=0;i<len;i++)
	{
		if((x[i]==' ' ||x[i]=='\t'))
		{
			flag=0;
			continue;
		}
		else if(flag==0)
		{
			a[j]=' ';
			j++;
			flag=1;
			a[j]=x[i];
			j++;
			continue;
		}
		else
		{
			a[j]=x[i];
			j++;
			continue;
		}
	}
	a[j]='\0';

}
////////////////////////////////////////
/////////////////////////MAIN FUNCTION//////
////////////////////////////////////////////
int main(void) {
	/////////////////STORES THE BUFFER TO PRINT FOR EVERY TIME////

	struct utsname buffer;
	int pid,status1;

	errno = 0;
	if (uname(&buffer) != 0)
	{
		perror("uname");
		exit(EXIT_FAILURE);
	}
	char prompt[50],chin[256],ch[256],pwdtrue[256],pwdtemp[256],fake_prompt[256],input[256];
	int len,lentemp;
	/////////////STORES PROMPT TO SHOW IT AT EACH TIME//////
	strcpy(prompt,"<");
	strcat(prompt,buffer.nodename);
	strcat(prompt,"@");
	strcat(prompt,buffer.sysname);
	strcat(prompt,":");
	getcwd(pwdtrue,sizeof(pwdtrue));
	len=strlen(pwdtrue);
	while(1)
	{

		strcpy(fake_prompt,prompt);
		getcwd(pwdtemp,sizeof(pwdtemp));
		lentemp=strlen(pwdtemp);
		if(strstr(pwdtemp,pwdtrue)!= NULL)
		{
			if(lentemp>len)
			{
				strcat(fake_prompt,"~");
				strcat(fake_prompt,pwdtemp + len);
			}
			else if(lentemp==len)
			{
				strcat(fake_prompt,"~");
			}

		}
		else
		{
			strcat(fake_prompt,pwdtemp);
		}
		strcat(fake_prompt,">");
		/////////////////////////////////////////////////////////////////////
		//THE PROMPT BEFORE TAKING IN COMMANDS IS PRINTED////////////////
		////////////////////////////////////////////////////////////
		printf("%s ",fake_prompt);
		scanf(" %[^\n]",chin);
		realify(ch,chin);

		//////////////CHILD PROCESS IS CREATED AND PARENT IS MADE TO WAIT/////////

		pid=fork();
		if(ch[strlen(ch)-1]!='&')
		{
			///////////////////////////////////////////////////////
			//THE METHODS ARE EXECUTED IN THE FORKED CHILD PROCESS/////
			//////////////////////////////////////////////////////////
			if(pid==0)
			{
				////////////////////////////////////////////////
				/////////////LS COMMAND EXECUTION//////////////


				if(ch[0]=='l' && ch[1]=='s')
				{
					char *token;
					token = strtok(ch," ");
					token = strtok(NULL," ");

					char arg[2];
					arg[0]='.';
					arg[1]='\0';


					//FOR LS COMMAND EXECUTION///
					if(token == NULL)
					{
						DIR *mydir;
						struct dirent *myfile;
						struct stat mystat;

						char buf[512];
						mydir = opendir(arg);
						while((myfile = readdir(mydir)) != NULL)
						{
							sprintf(buf, "%s/%s", arg, myfile->d_name);
							stat(buf, &mystat);
							if(myfile->d_name[0]!='.')
								printf("%s ", myfile->d_name);
						}
						printf("\n");
						closedir(mydir);
					}

					//FOR LS <dir> COMMAND//

					else if(token[0]!='-')
					{

						DIR *mydir;
						struct dirent *myfile;
						struct stat mystat;
						char* arg = token;

						char buf[512];
						mydir = opendir(arg);
						while((myfile = readdir(mydir)) != NULL)
						{
							sprintf(buf, "%s/%s", arg, myfile->d_name);
							stat(buf, &mystat);
							//printf("%ld",mystat.st_size);
							if(myfile->d_name[0]!='.')
								printf(" %s\n", myfile->d_name);
						}
						printf("\n");
						closedir(mydir);
					}
					else
					{
					char check1[2];
					check1[0]='a';
					check1[1]='\0';

					//FOR ls -a COMMAND////////
                                        /*
						if(token[1]==check1[0] && token[2]=='\0')
						{
							DIR *mydir;
							struct dirent *myfile;
							struct stat mystat;
							//	char arg[2];
							arg[0]='.';
							arg[1]='\0';

							char buf[512];
							mydir = opendir(arg);
							while((myfile = readdir(mydir)) != NULL)
							{
								sprintf(buf, "%s/%s", arg, myfile->d_name);
								stat(buf, &mystat);
								//printf("%ld",mystat.st_size);
								printf("%s ", myfile->d_name);
							}
							printf("\n");
							closedir(mydir);
							//	continue;
						}*/

						//for ls -al

						char check2[3];
						check2[0]='l';
						check2[1]='a';
						check2[2]='\0';

						if((token[1]==check2[0] && token[2]==check2[1]) || (token[1]==check2[1] && token[2]==check2[0]))
						{

							DIR *mydir;
							struct dirent *myfile;
							struct stat mystat;
							//	char arg[2];
							arg[0]='.';
							arg[1]='\0';

							char buf[512];
							mydir = opendir(arg);

							char tempo[256];
							getcwd(tempo,sizeof(tempo));

							while((myfile = readdir(mydir)) != NULL)
							{
								sprintf(buf, "%s/%s", arg, myfile->d_name);
								stat(buf, &mystat);
								//printf("%lld",mystat.st_size);
								//printf(" %s\n", myfile->d_name);

								struct stat fileStat;
								if(stat(myfile->d_name,&fileStat) < 0)
									return 1;


								//printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
								printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
								printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
								printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
								printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
								printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
								printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
								printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
								printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
								printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");

								//printf("%d ",fileStat.st_size);

								printf(" %lu ",fileStat.st_nlink);
								printf("%s ",buffer.nodename);
								printf("%s ",buffer.nodename);
								printf("%lu ",fileStat.st_ino);

								struct stat attr;

								strcat(tempo,myfile->d_name);

								stat(tempo, &attr);

								//	ctime(&attr.st_mtime)[strlen(ctime(&attr.st_mtime))-2]='\0';

								int v=0;
								while(ctime(&attr.st_mtime)[v]!='\0')
								{
									if(ctime(&attr.st_mtime)[v]!='\n')
										printf("%c",ctime(&attr.st_mtime)[v]);

									v++;
								}


								printf(" %s",myfile->d_name);
								printf("\n");

							}
							closedir(mydir);


						}

						//FOR ls -l COMMAND/////

						if(token[1]==check2[0])
						{
							DIR *mydir;
							struct dirent *myfile;
							struct stat mystat;
							//	char arg[2];
							arg[0]='.';
							arg[1]='\0';

							char buf[512];
							mydir = opendir(arg);

							char tempo[256];
							getcwd(tempo,sizeof(tempo));

							while((myfile = readdir(mydir)) != NULL)
							{
								sprintf(buf, "%s/%s", arg, myfile->d_name);
								stat(buf, &mystat);
								//printf("%lld",mystat.st_size);
								//printf(" %s\n", myfile->d_name);

								struct stat fileStat;
								if(stat(myfile->d_name,&fileStat) < 0)
									return 1;

								if(myfile->d_name[0]!='.')
								{
									//printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
									printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
									printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
									printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
									printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
									printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
									printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
									printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
									printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
									printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");

									//printf("%d ",fileStat.st_size);

									printf(" %lu ",fileStat.st_nlink);
									printf("%s ",buffer.nodename);
									printf("%s ",buffer.sysname);
									printf("%lu ",fileStat.st_ino);

									struct stat attr;

									strcat(tempo,myfile->d_name);

									stat(tempo, &attr);

									//		ctime(&attr.st_mtime)[strlen(ctime(&attr.st_mtime))-2]='\0';
									int v=0;
									while(ctime(&attr.st_mtime)[v]!='\0')
									{
										if(ctime(&attr.st_mtime)[v]!='\n')
											printf("%c",ctime(&attr.st_mtime)[v]);

										//printf("%c\n",ctime(&attr.st_mtime)[v]);
										v++;
									}

									//	printf("%s ", ctime(&attr.st_mtime));

									printf(" %s",myfile->d_name);
									printf("\n");
								}

							}
							closedir(mydir);


						}
					}

				}
				////////////////////////////////////////////////
				//EXECUTION OF ECHO COMMAND///////////////////
				///////////////////////////////////////////

				else if(ch[0]=='e' && ch[1]=='c' && ch[2]=='h' && ch[3]=='o')
				{
					char *token;
					token = strtok(ch," ");
					int i,flag,j;
					token = strtok(NULL," ");
					while(token != NULL)
					{
						flag=0;
						for(i=0;i<strlen(token);i++)
						{
							if(token[i]=='"')
							{continue;}
							else
							{printf("%c",token[i]);
								flag=1;

							}
						}
						if(flag==0)
						{
							printf(" ");
						}

						token = strtok(NULL," ");
						printf(" ");

					}
					printf("\n");
				}

					//////////////////////////////////////////////
					//EXECUTION OF PINFO COMMAND/////////////////
					///////////////////////////////////////////
				else if(ch[0]=='p' && ch[1]=='i' && ch[2]=='n' && ch[3]=='f' && ch[4]=='o')
				{
					char *toke;
					toke=strtok(ch," ");
					toke = strtok(NULL," ");
					char *pi;
					///////FOR PINFO///////
					if(toke==NULL)
					{
						int p;
						p =getpid();
						char sp[50];
						sprintf(sp,"%d",p);
						
						pi = sp;
					}
					////FOR PINFO <PROC> ////
					else if(toke!=NULL)
					{
						
						pi=toke;
					}

					char pa[256]="/proc/";
					
					strcat(pa,pi);
					strcat(pa,"/stat");

					
					FILE *in_f; 
					in_f= fopen(pa,"r");
					char in_file[256];
					fscanf(in_f,"%[^\n]",in_file);
					//printf("%s\n",in_file);

					char *tk;
					char *tk0;
					char *tk1;
					char *tk2;
					char *tk8;

					tk=strtok(in_file," ");
					tk0=tk;
					tk=strtok(NULL," ");

					tk1=tk;
					
					tk=strtok(NULL," ");

					tk2=tk;
					
					tk=strtok(NULL," ");
					tk=strtok(NULL," ");
					tk=strtok(NULL," ");
					tk=strtok(NULL," ");
					tk=strtok(NULL," ");
					tk=strtok(NULL," ");
					tk8=tk;
						
					printf("pid -- %s\n",tk0);
					printf("Process Status -- %s\n",tk2);
					printf("Virtual Memory -- %s\n",tk8);
					printf("Executable Path -- %s\n",tk1);
					
				}
				//////////////////////////////////
				///EXECUTION OF PWD//////////////
				/////////////////////////////////

				else if(strcmp("pwd",ch)==0)
				{
					printf("%s\n",pwdtemp);
				}
				//////////////////////////////////
				//EXECUTION OF CD////////////////
				////////////////////////////////
				else if(ch[0]=='c' && ch[1]=='d')
				{

					char *token;
					token = strtok(ch," ");
					token = strtok(NULL," ");
					if(chdir(token)==-1)
						printf("%s directory not present\n",token);
					else{
						continue;
					}
				}
				////////////////////////////////
				//EXECUTION OF COMMANDS LIKE FIREFOX,GEDIT ETC///////
				////////////////////////////////////////////////
				else

				{
					char *a[1024];
					char *token;
					token = strtok(ch," ");
					int z=0;

					while(token != NULL)
					{

						a[z] = token;
						token = strtok(NULL," ");
						z++;

					}

					execvp(*a,a);

				}

			}
			////////////////////////////////
			//PARENT IS WAITING//////////
			else
			{
				waitpid(pid,&status1,0);

			}
		}
		/////////////////////////////////
		//IF & IS GIVEN AT LAST ie., BACKGROUND PROCESS///
		////////////////////////////////////////////////
		else
		{
			if(pid==0)
			{
				char *a[1024];
				ch[strlen(ch)-1]='\0';
				char *token;
				token = strtok(ch," ");
				int z=0;

				while(token != NULL)
				{

					a[z] = token;
					token = strtok(NULL," ");
					z++;

				}

				execvp(*a,a);
			}

		}

	}
	return EXIT_SUCCESS;
}
