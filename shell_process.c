#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>

#define MAXARGUMENT 512
#define ANSI_COLOR_BLUE    "\x1b[34m"

static char* args[MAXARGUMENT];
static char line[64];

int main(){ 
        while(1)
	{
		char * cmd;
		
		printf(ANSI_COLOR_BLUE   "Shell_process >> ");
		fflush(NULL);
		
		if(!fgets(line,64,stdin))
			return 0;
		
		/* Procesamiento de parametros*/
		cmd=strtok(line," ");
		int argNumber = 0;
		
		while(cmd !=NULL){
			args[argNumber] = cmd;
			argNumber++;
			cmd = strtok(NULL," ");			
		}	
			
		int len=strlen(args[argNumber-1]);
		char wordFixed[len-1];
		
		int i;
		for (i=0;i<len-1;i++)
		{
			wordFixed[i]=args[argNumber-1][i];
		}
		wordFixed[i]='\0';
		args[argNumber-1]=wordFixed;
		
		if(strcmp(args[0],"exit")==0)
			return 1;
		
		command_process();
	}
	exit(0);
} 

void command_process()
{
	int opt,status;
	int pfd[2];
	pid_t pid;
	
	
	/* Creacion de pipe */
	if(pipe(pfd)<0){
		perror("Error en el pipe");
		exit(1);
	}
	/* Creacion del proceso hijo*/
	if((pid=fork())<0){
		perror("Error en el fork");
		exit(1);
	}
	/* En el proceso hijo ejecutamos el command_process*/
	if(pid==0){
		dup2(pfd[1],stdout);
		close(pfd[0]);
		/* ... */
		execvp(args[0], args);
		fflush(stdout);
		perror("Error en el exec");
		_exit(127);
	}
	close(pfd[1]);
	
	/* Leer la salida de command_process*/
	if(read(pfd[0],line,3)<0){
		perror("Error en el read");
		exit(1);
	}
	
	/* Cerrar el pipe y esperar para salir*/
	close(pfd[0]);
	waitpid(pid,&status,0);
	
}