#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>

#define BUFFSIZE 4096

char buff[BUFFSIZE]; //buffer for reading and writing
typedef void (*sig_t) (int);
int cnt = 0;
void sig_handler(int signo) {

 	if(signo == SIGINT) {
		printf("SIGINT is received\n");
	}
	else if(signo == SIGQUIT) {
		printf("SIGQUIT is received\n");
		cnt++;
		printf("%d",cnt);
	}
  else if(signo == SIGTERM) {
		printf("SIGTERM is received\n");
	}
  else if(signo == SIGHUP) {
		printf("SIGHUP is received\n");
	}
}


int main(int argc, char *argv[]) {

    int n;
    int fd1 = open(argv[1], O_RDONLY);
    int fd2 = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);

    signal(SIGINT, (sig_t) sig_handler);
    signal(SIGQUIT, (sig_t) sig_handler);
    signal(SIGTERM, (sig_t) sig_handler);
    signal(SIGHUP, (sig_t) sig_handler);

    if(signal(SIGINT, sig_handler) == SIG_ERR) {
		fprintf(stderr, "Could not catch SIGINT\n");
		exit(1);
	  }
    if(signal(SIGQUIT, sig_handler) == SIG_ERR) {
      fprintf(stderr, "Could not catch SIGQUIT\n");
    }
    if(signal(SIGTERM, sig_handler) == SIG_ERR) {
      fprintf(stderr, "Could not catch SIGTERM\n");
    }
    if(signal(SIGHUP, sig_handler) == SIG_ERR) {
      fprintf(stderr, "Could not catch SIGHUP\n");
    }
    while(1) {
        
      while((n = read(fd1, buff, BUFFSIZE)) > 0) {
        if(write(fd2, buff, n) != n) {
          perror("Could not write into file.\n");
          exit(1);
        }
      }
      sleep(1);
    }
    close(fd1);
    close(fd2);

    return 0;

}