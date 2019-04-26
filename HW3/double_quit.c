#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define N 255
int cnt = 0;

void alarm_handler(int signo) {

	if(signo == SIGALRM) {
		printf("%d seconds have passed\n", ++cnt);
	
		alarm(3);
	}
}

char buf[255];


int main(int argc, char *argv[]) {


	if(signal(SIGALRM, alarm_handler) == SIG_ERR) {
		fprintf(stderr, "Error handling signal\n");
		exit(1);
	}

	alarm(1);
	
	while(fgets(buf, 255, stdin) !=NULL) {

		fprintf("%s", buf);
	}
	

}
