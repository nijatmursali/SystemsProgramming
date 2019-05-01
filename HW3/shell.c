//libraries 
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/ioctl.h>


//color variables for shell
#define COLOR_GREEN "\x1b[1;32m" 
#define COLOR_LIGHT_BLUE "\x1b[1;94m" 
#define COLOR_RESET "\x1b[0m" 

//defines
#define SIZE 1024
#define MAX_SIZE 256

//variables 

typedef char *multipleTokenization_t;
struct stat st; // struct for getting stats of file


//getting username 
char *getlogin();

void ChangingDirectory(char **args) {
    if(args[1] == NULL) {
        printf("Enter the second argument please\n");

    }
    else {
        if(chdir(args[1]) !=0) {
        }
    }
}


//Splits the character that has more more characters
//stackoverflow answer https://stackoverflow.com/q/29788983 by Jerry Coffin
char *multipleTokenization(char *input, multipleTokenization_t *inptChar, char *delimiter)
{
    if (input != NULL)  *inptChar = input;

    if (*inptChar == NULL) return *inptChar;

    char *end = strstr(*inptChar, delimiter);
    if (end == NULL)
    {
        char *temp = *inptChar;
        *inptChar = NULL;
        return temp;
    }

    char *temp = *inptChar;

    *end = '\0';
    *inptChar = end + strlen(delimiter);
    return temp;
}

multipleTokenization_t init() { 
    return NULL; 
    }

/* signal handler */
void sig_handler(int signo);

char *argv[];
int backgroundPro;
void backgroundProcess() {
    if(strcmp(argv[0], "&") == 0) {
        backgroundPro = 1;
        argv[0] = NULL;
    }
}
int main(int argc, char *argv[])
{
    FILE *fp;

    
    // command shell loop
    while(1)
    {
        int proceed = 0;
        char line[MAX_SIZE];
        char *commandLine = (char*)calloc(SIZE, sizeof(char*));

	signal(SIGUSR1, sig_handler);

        // read command lines
        while (proceed == 0)
        {
            // read a line
            char *p = getenv("USER");
            if(p==NULL) return EXIT_FAILURE;
            
            
            printf("%s%s%s%ssimpleshell%s:",COLOR_GREEN,p,COLOR_RESET,COLOR_LIGHT_BLUE,COLOR_RESET);
            fgets(line, MAX_SIZE, stdin);
            printf("\n");
            if (strlen(line) == 0)  break;
            if(line[strlen(line) - 1] == '\n') line[strlen(line) - 1] = '\0';
            if(line[strlen(line) - 1] == '\\')
            {
                proceed = 0;
                line[strlen(line) - 1] = '\0';
            }
            else proceed = 1;

            if ((strlen(commandLine) > 0) && (line[strlen(line) - 2] == '&')) strcat(commandLine, "&&");
            strcat(commandLine, line);
        }

        pid_t pid;

        //backgroundProcess(); // homework 3 task 3

        int status;
        multipleTokenization_t s = init();

        
        char *cmdline = multipleTokenization(commandLine, &s, "&&"); //3rd task
        while(cmdline != NULL)
        {
            if (strlen(cmdline) == 0) break;
            // command line arguments 
            if (strcmp(cmdline, "exit") == 0) {
                exit(0);
            }
            if(strcmp(cmdline, "cd") == 0) { 
                ChangingDirectory(argv[0]); 
                //printf("not finished yet\n");
            }
            if(strcmp(cmdline, "clr") == 0) {
                char c[50];
                strcpy(c, "clear");
                system(c);
            }
            if(strcmp(cmdline, "cd") == 0) {
                char c[50];
                strcpy(c, "cd");
                system(c);
            }
            if(strchr(argv[0], '/') == NULL) {
            // try to execute
            if ((pid = fork()) < 0)
            {
                fprintf(stderr, "Process is not created. Try again.\n");
                return -1;
            }
            else if (pid == 0) { // child process
                // 5th task here
                

				if((fp = popen(cmdline, "r")) == NULL) {
					fprintf(stderr, "ERROR: Cannot create the child process for shell.");
					exit(EXIT_FAILURE);
				}
				
				char *line = NULL;
				size_t length = 0;
				
				while(getline(&line, &length, fp) != -1) printf("%s", line);
				
				pclose(fp);
				exit(0);
               
            }
            // Implement here
            else {
                if(backgroundPro) {
                    printf("background job for %d\n starting",pid);
                    
                }else {
                    waitpid(pid, NULL, 0);
                }
                //backgroundPro = 0;
            }
            }

        }
    }

    return 0;
}

void sig_handler(int signo){
	printf("Signal has been received: %d", signo);
}
