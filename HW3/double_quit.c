#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define N 255
int cnt = 0;

void exitFunction(int sig) {
	_exit(0);
	
}
void sig_handler(int signo) {

 	if(signo == SIGINT) {
		printf("SIGINT is received\n");
	}
	else if(signo == SIGQUIT) {
		printf("SIGQUIT is received\n");
		cnt++;
		printf("%d",cnt);
	}
	else if(signo == SIGALRM) {
		printf("SIGALRM is received.\n");
	}
}

char buf[255];


int main(int argc, char *argv[]) {


	if(signal(SIGINT, sig_handler) == SIG_ERR) {
		fprintf(stderr, "Error handling signal\n");
		exit(1);
	}
	if(signal(SIGQUIT, sig_handler) == SIG_ERR) {
		fprintf(stderr, "Could not catch SIGQUIT\n");
	}
	if(signal(SIGALRM, sig_handler) == SIG_ERR) {
		fprintf(stderr, "Could not catch SIGQUIT\n");
	}else {
		printf("I will shut down automatically after 5 seconds.\n");
		signal(SIGALRM, exitFunction);
		alarm(5);	
	}
	
	while(1)
    {
	memset(buf, 0, 255);
        printf("Enter string: \n");
        fgets(buf, 255, stdin);
        if (strlen(buf) == 0) continue;
	printf("%s", buf);
    }    
	return 0;

}
