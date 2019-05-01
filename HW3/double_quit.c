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

 	if(signo == SIGALRM) {
		printf("SIGALRM is received.\n");
		signal(SIGQUIT, sig_handler);
		alarm(5);	
	}
	else if(signo == SIGQUIT) {
		printf("SIGQUIT is received\n");
		cnt++;
		printf("%d",cnt);
		signal(SIGQUIT, sig_handler);
	}
	
}

char buf[255];


int main(int argc, char *argv[]) {
	memset(buf, 0, 255);
	if(signal(SIGALRM, sig_handler) == SIG_ERR) {
		fprintf(stderr, "Could not catch SIGQUIT\n");
	}
	if(signal(SIGQUIT, sig_handler) == SIG_ERR) {
		fprintf(stderr, "Could not catch SIGQUIT\n");
	}
	else {
		
	}
	
	while(1)
    {
        printf("Enter string: \n");
        fgets(buf, 255, stdin);
        if (strlen(buf) == 0) continue;
	printf("%s", buf);
    }    
	return 0;

}
