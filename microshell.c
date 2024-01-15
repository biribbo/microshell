#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>  
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>

char* login;
char buff[1024];

int movefile(const char* file, const char* path) {
	int fd1 = open(file, O_RDONLY);
	DIR *d=opendir(path);
	struct stat dir;
	stat(path, &dir);
	if (S_ISDIR(dir.st_mode)==0) return -1;
	if (d==0) return -1;
	int len=strlen(path)+2+strlen(file);
	char newpath[len];
	snprintf(newpath, len, "%s/%s", path, file);
    int fd2 = open(newpath, O_CREAT, 0640);
    int num;
    while ((num = read(fd1, &buff, 1024)) > 0) {
        write(fd2, &buff, num);
    }
    close(fd1);
    close(fd2);
    unlink(file);
	return 0;
}

int removedir(const char *path) {
	DIR *d=opendir(path);
	struct stat dir, newdir;
	struct dirent *ent;
	stat(path, &dir);
	if (S_ISDIR(dir.st_mode)==0) return -1;
	if (d==0) return -1;
	while((ent=readdir(d))!=NULL) {
		if (strcmp(ent->d_name, ".")==0 || strcmp(ent->d_name, "..")==0) continue;
		int len=strlen(path)+2+strlen(ent->d_name);
		char newpath[len];
		snprintf(newpath, len, "%s/%s", path, ent->d_name);
		stat(newpath, &newdir);
		if (S_ISREG(newdir.st_mode)!=0) {
			int proc=unlink(newpath);
			if (proc!=0) return -1;
		} else removedir(newpath);
	}
	int proc = rmdir(path);
	if (proc!=0) return -1;
	closedir(d);
	return 0;
}

int main() {
    printf("\n        \x1b[33m 	 __   __   __   __        ___      \x1b[0m        \n");
	printf("________\x1b[33m |\\/| | /  ` |__) /  \\ /__` |__| |__  |    |    \x1b[0m________\n");                                              
	printf("        \x1b[33m |  | | \\__, |  \\ \\__/ .__/ |  | |___ |___ |___ \x1b[0m        \n\n\n");
	char input[1000];
	login = getlogin();
	printf("¦\t Witaj, \x1b[36m%s\x1b[0m! Wpisz \x1b[32m\x1b[3mhelp\x1b[0m, aby uzyskaæ pomoc.        \t¦\n\n", login);
	while(1) {
		char cwd[1000];
		getcwd(cwd, sizeof(cwd));
		printf("[\x1b[36m%s\x1b[0m:%s]$ ", login, cwd);
		fgets(input, 1000, stdin);
		if(input[strlen(input)-1] == '\n') input[strlen(input)-1] = 0;
		char *tok;
		char whitespaces[]="\t ";
		tok=strtok(input, whitespaces);
		if(strcmp(tok, "cd")==0) {
			tok=strtok(NULL, whitespaces);
			if (strcmp(tok, "~")==0 || strcmp(tok, "~/")==0) tok=getenv("HOME");
			int proc = chdir(tok);
			if (proc != 0) {
				perror("B³¹d");
				printf("\x1b[31mNumer b³êdu: %d.\x1b[0m\n", errno);
			}
		}
		else if(strcmp(tok, "mv")==0) {
			const char* file1 = strtok(NULL, whitespaces);
			const char* file2 = strtok(NULL, whitespaces);
			if(file1[strlen(file1)-1] != '/') {
				if(file2[strlen(file2)-1] == '/') {
					int proc = movefile(file1, file2);
    				if (proc != 0) {
    					perror("B³¹d");
    					printf("\x1b[31mNumer b³êdu: %d.\x1b[0m\n", errno);
					}
				} else {
					int proc = rename(file1, file2);
    				if (proc != 0) {
    					perror("B³¹d");
    					printf("\x1b[31mNumer b³êdu: %d.\x1b[0m\n", errno);
					}
				}
			} else {
				int proc = rename(file1, file2);
    			if (proc != 0) {
    				perror("B³¹d");
    				printf("\x1b[31mNumer b³êdu: %d.\x1b[0m\n", errno);
				}
			}
		}
		else if(strcmp(tok, "rm")==0) {
			const char *file = strtok(NULL, whitespaces);
			if (strcmp(file, "-r")==0) {
				const char *path = strtok(NULL, whitespaces);
				while (path!=NULL) {
					int proc = removedir(path);
					if (proc!=0) {
						perror("B³¹d");
    					printf("\x1b[31mNumer b³êdu: %d.\x1b[0m\n", errno);
					}
					path = strtok(NULL, whitespaces);
				}
				
			} else {
				while (file!=NULL) {
				int proc = unlink(file);
					if (proc!=0) {
						perror("B³¹d");
    					printf("\x1b[31mNumer b³êdu: %d.\x1b[0m\n", errno);
					}
					file = strtok(NULL, whitespaces);
				}
			}
		}
		else if(strcmp(tok, "help")==0) {
			printf("\n¦\n¦\tAutorem programu jest Barbara Ratajczak.\n¦\n¦\tProgram oferuje obs³ugê funkcji ze zmiennej PATH oraz\n¦\tsamodzielnie zaimplementowanych funkcji \x1b[32m\x1b[3mcd\x1b[0m, \x1b[32m\x1b[3mmv\x1b[0m i \x1b[32m\x1b[3mrm\x1b[0m (tak¿e z flag¹ \x1b[32m\x1b[3m-r\x1b[0m).\n¦\tNie mo¿na jednak przenosiæ wiêcej ni¿ jednego pliku naraz.\n¦\n¦\tAby wyjœæ, wpisz \x1b[32m\x1b[3mexit\x1b[0m.\n¦\n\n");
		}
		else if(strcmp(tok, "exit")==0) break;
		else {
			char* toks[100];
			int i=0;
			while(tok!=NULL) {
				toks[i]=tok;
				tok = strtok(NULL, whitespaces);
				i++;
			}
			toks[i]=NULL;
			pid_t id = fork();
			const char* tok2 = strtok(input, whitespaces);
			if (id==0) {
				execvp(tok2, toks);
				perror("B³¹d");
				printf("\x1b[31mNumer b³êdu: %d.\x1b[0m\n", errno);
				exit(1);
			} else if (id<0) {
				perror("B³¹d");
				printf("\x1b[31mNumer b³êdu: %d.\x1b[0m\n", errno);
				exit(1);
			} else {
				wait(NULL);
			}
		}
	}
	exit(0);
	return 0;
}
