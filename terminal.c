#define _POSIX_SOURCE
#include <unistd.h>
#undef _POSIX_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/utsname.h>

int main(void) {

	struct utsname buffer;

	errno = 0;
	if (uname(&buffer) != 0) {
		perror("uname");
		exit(EXIT_FAILURE);
	}
	char prompt[50],ch[50],pwdtrue[256],pwdtemp[256],toconc[256],fake_prompt[256],input[256];
	int len,lentemp;
	strcpy(prompt,"<");
	strcat(prompt,buffer.nodename);
	strcat(prompt,"@");
	strcat(prompt,buffer.sysname);
	strcat(prompt,":");
	getcwd(pwdtrue,sizeof(pwdtrue));
	len=strlen(pwdtrue);
	//printf("%s\n",pwdtrue);
	while(1)
	{
		strcpy(fake_prompt,prompt);
		getcwd(pwdtemp,sizeof(pwdtemp));
		lentemp=strlen(pwdtemp);
		if(strstr(pwdtemp,pwdtrue)!= NULL)
		{
			if(lentemp!=len)
			{
				//printf("hi\n");
				strncpy(toconc,pwdtemp + len,lentemp-len);
				//printf("%s\n",toconc);
				strcat(fake_prompt,"~");
				strcat(fake_prompt,toconc);
			}
			else
			{
				strcat(fake_prompt,"~");
			}

		}
		else
		{
			strcat(fake_prompt,pwdtemp);
		}
		strcat(fake_prompt,">");
		printf("%s ",fake_prompt);
		scanf(" %[^\n]",ch);
		if(ch[0]=='e' && ch[1]=='c' && ch[2]=='h' && ch[3]=='o')
		{
			const char s[2]=" ";
	                char *token;
			token = strtok(ch, s);
	                token = strtok(NULL, s);
                        
			printf("%s\n",token);
															                                                      
		}
		if(strcmp("pwd",ch)==0)
		{
			printf("%s\n",pwdtemp);
		}
		if(ch[0]=='c' && ch[1]=='d')
		{
			const char s[2]=" ";
			char *token;
			token = strtok(ch, s);
			token = strtok(NULL, s);
			if(chdir(token)==-1)
				printf("%s directory not present\n",token);
			else{
				//strcpy(pwdtrue,token);
				continue;
			}

		}
		if(strcmp("exit",ch)==0)
		{
			break;
		}
	}

	return EXIT_SUCCESS;
}
