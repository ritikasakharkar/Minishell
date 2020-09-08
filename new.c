#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
extern int errno;
#define CommandNumber 6
#define MaxCommandLine 128
#define MaxParamNumber 2
char* promptStr="minishell>";
char* delim=" \n";
char* helpStr=
"Syntax: exit\nSyntax: cd directory\nSyntax: dir [directory]\nSyntax: del file\nSyntax: ren srcfile dstfile\nSyntax: copy srcfile dstfile\n";
enum CommandType
{
ExitType, CDType, DirType, DelType, RenType, CopyType
};
char* commandStr[CommandNumber]=

{
"exit", "cd", "dir", "del", "ren", "copy"
};
void exitShell(char* params[], int paramNumber);
void changeDir(char* params[], int paramNumber);
void listDir(char* params[], int paramNumber);
void delFile(char* params[], int paramNumber);
void renFile(char* params[], int paramNumber) ;
void copyFile(char* params[], int paramNumber);
void (*commandArray[CommandNumber])(char* params[], int paramNumber)=
{
exitShell, changeDir, listDir, delFile, renFile, copyFile
};
int parseCommand(char* cmdStr, char* params[], int* paramNumber);
void dodir(char* path);
void printPrompt();
void errhandle(char* msg);
int main(int argc, char* argv[])
{
char buf[MaxCommandLine];
int n, paramNumber;
int commandType;
char* params[4];
printPrompt();
while ((n=read(STDIN_FILENO, buf, MaxCommandLine))>0)
{
buf[n]='\0';
commandType=parseCommand(buf, params, &paramNumber);
if (commandType==-1)
{
printf("illegal command\n%s");

}
else
{
commandArray[commandType](params, paramNumber);
}
printPrompt();
}
return 0;
}
int parseCommand(char* buf, char* params[], int* paramNumber)
{
int i;
*paramNumber=0;
if ((params[*paramNumber]=strtok(buf, delim))!=NULL)
{
for (i=CommandNumber-1; i>=0; i--)
{
if (strcmp(params[*paramNumber], commandStr[i])==0)
{
break;
}
}
//when not found, i==-1
if (i==-1)
{
return i;
}
}
else
{
return -1;
}
(*paramNumber)++;
//the maximum param number is only 2, so I test for 3 to see if strtok return NULL
while (1)

{
if ((params[*paramNumber]=strtok(NULL, delim))==NULL)
{
break;
}
(*paramNumber)++;
if (*paramNumber==4)
{
//this means the param number is more than 2 and it is wrong
return -1;
}
}
return i;
}
void exitShell(char* params[], int paramNumber)
{
exit(0);
}
void renFile(char* params[], int paramNumber)
{
if (paramNumber!=3)
{
printf(helpStr);
}
else
{
if (strcmp(params[1], params[2])==0)
{
printf("The source file %s and the destination file %s can not be the same.\n", params[1], params[2]);
}
else
{

if (access(params[1], F_OK)<0)
{
printf("The source file %s does not exist.\n", params[1]);
}
else
{
if (rename(params[1], params[2])<0)
{
printf("cannot rename file from %s to file %s\n", params[1], params[2]);
}
}
}
}
}
void copyFile(char* params[], int paramNumber)
{
int status;
if (paramNumber!=3)
{
printf(helpStr);
}
else
{
if (fork()==0)
{
execv("/bin/cp", params);
exit(0);
}
else
{
if (wait(&status)<0)
{
printf("wait error\n");
}
}

}
}
void delFile(char* params[], int paramNumber)
{
if (paramNumber!=2)
{
printf(helpStr);
}
else
{
if (access(params[1], F_OK)<0)
{
printf("The file %s does not exist\n", params[1]);
}
else
{
if (unlink(params[1])<0)
{
printf("cannot delete file %s\n", params[1]);
}
}
}
}
void changeDir(char* params[], int paramNumber)
{
if (paramNumber!=2)
{
printf(helpStr);
}
else
{
if (chdir(params[1])<0)
{
if (errno==ENOTDIR||errno==ENOENT)
{

printf("%The directory %s does not exist\n", params[1]);
}
}
}
}
void dodir(char* path)
{
DIR* dp;
struct dirent* dirnode;
if ((dp=opendir(path))!=NULL)
{
while ((dirnode=readdir(dp))!=NULL)
{
printf("%s\n", dirnode->d_name);
}
}
else
{
printf("The directory %s does not exist\n", path);
}
}
void listDir(char* params[], int paramNumber)
{
if (paramNumber!=1&&paramNumber!=2)
{
printf(helpStr);
}
else
{
if (paramNumber==1)
{
dodir(".");
}
else
{

dodir(params[1]);
}
}
}
void printPrompt()
{
if (write(STDOUT_FILENO, promptStr, strlen(promptStr))!=strlen(promptStr))
{
errhandle("cannot write to stdout");
}
}
void errhandle(char* msg)
{
perror(msg);
exit(1);
}