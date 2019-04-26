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
#define CHECK_CPU    1
#define CHECK_MEM    1 << 1
#define CHECK_BLK    1 << 2
#define CHECK_ALL    7

//variables 

typedef char *multipleTokenization_t;
struct rlimit rlim; //struct for getting the limits
struct rusage usage; //struct for usage
struct stat st; // struct for getting stats of file
int CPU, MEM, STACK;
int limitforCPU = 2; // default 2 seconds
int limitforMEM = 16*1024*1024; // default 16 MB
int limitforSTK = 1024*1024;    // default 1 MB
char operation;
int check = 0;
int shellRestricted = 0;

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

char *lsPermissions(char *string) {

    char *val = calloc(10, sizeof(char));

    if(lstat(string, &st) == 0) {
        mode_t permissions = st.st_mode;

        if(S_ISREG(permissions)) val[0] = '-';
        if(S_ISDIR(permissions)) val[0] = 'd';
        if(S_ISLNK(permissions)) val[0] = ';';
        if(S_ISCHR(permissions)) val[0] = 'c';
        if(S_ISBLK(permissions)) val[0] = 'b';
        if(S_ISSOCK(permissions)) val[0] = 's';
        if(S_ISFIFO(permissions)) val[0] = 'f';

        val[1] = (permissions & S_IRUSR) ? 'r' : '-';
        val[2] = (permissions & S_IWUSR) ? 'w' : '-';
        val[3] = (permissions & S_IXUSR) ? 'x' : '-';

        val[4] = (permissions & S_IRGRP) ? 'r' : '-';
        val[5] = (permissions & S_IWGRP) ? 'w' : '-';
        val[6] = (permissions & S_IXGRP) ? 'x' : '-';

        val[7] = (permissions & S_IROTH) ? 'r' : '-';
        val[8] = (permissions & S_IWOTH) ? 'w' : '-';
        val[9] = (permissions & S_IXOTH) ? 'x' : '-';

        return val;
    }
}
char *gettingColors(char *string) {

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


int main(int argc, char *argv[])
{
    FILE *fp;

    // parse command switches and set environment variables
    while ((operation = getopt(argc, argv, "cbmar")) != -1)
    {
        switch (operation)
        {
            case 'c': check |= CHECK_CPU; break;
            case 'm': check |= CHECK_MEM; break;
            case 'b': check |= CHECK_BLK; break;
            case 'a': check = CHECK_ALL;  break;
            case 'r':
                shellRestricted = 1;
                
                for (int i = 0; i < argc; i++)
                {
                    if ((strcmp(argv[i], "-r") == 0) && (i < argc - 3))
                    {
                        if (argv[i + 1]) CPU = atoi(argv[i + 1]);
                        if (argv[i + 2]) MEM = atoi(argv[i + 2]);
                        if (argv[i + 3]) STACK = atoi(argv[i + 3]);
                    }
                    if (CPU > 0) limitforCPU = CPU;
                    if (MEM > 0) limitforMEM = MEM;
                    if (STACK > 0) limitforSTK = STACK;
                }
                break;
        }
    }

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

            // try to execute
            if ((pid = fork()) < 0)
            {
                fprintf(stderr, "Process is not created. Try again.\n");
                return -1;
            }
            else if (pid == 0) { // child process
                // 5th task here
                if (shellRestricted == 1)
                {
                    
                    int err = 0;
                    
                    // CPU limit
                    if ((getrlimit(RLIMIT_CPU, &rlim) == 0) && ((int)rlim.rlim_cur > limitforCPU))
                    {
                        err = 1;
                        fprintf(stderr, "CPU time: %d > %d\n", (int)rlim.rlim_cur, (int)limitforCPU);
                    }
                    // memory limit
                    if ((getrlimit(RLIMIT_AS, &rlim) == 0) && ((int)rlim.rlim_cur > limitforMEM))
                    {
                        err = 1;
                        fprintf(stderr, "Virtual Memory: %d > %d\n", (int)rlim.rlim_cur, (int)limitforMEM);
                    }
                    
                    // Stack size limit
                    if ((getrlimit(RLIMIT_STACK, &rlim) == 0) && ((int)rlim.rlim_cur > limitforSTK))
                    {
                        err = 1;
                        fprintf(stderr, "Stack size: %d > %d\n", (int)rlim.rlim_cur, (int)limitforSTK);
                    }
                    if (err == 1) exit(-1);
                }

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

            wait4(pid, &status, RUSAGE_SELF, &usage);
            // dump information related processes as the flags
            if (check & CHECK_CPU) printf("User time used by process is =%ld\tSystem time used by process is =%ld\n", usage.ru_utime.tv_usec, usage.ru_stime.tv_usec);
            if (check & CHECK_MEM) printf("Size of residential memory used by process is =%ld\n", usage.ru_maxrss);
            if (check & CHECK_BLK) printf("Number of block inputs are =%ld\t Number of block outputs are =%ld\n", usage.ru_inblock, usage.ru_oublock);

            cmdline = multipleTokenization(NULL, &s, "&&");
        }
    }

    return 0;
}

void sig_handler(int signo){
	printf("Signal has been received: %d", signo);
}
