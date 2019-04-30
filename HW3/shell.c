#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>

#define COLOR_GREEN "\x1b[1;32m" // color for executables
#define COLOR_LIGHT_BLUE "\x1b[1;94m" // color for directories
#define COLOR_CYAN "\x1b[1;96m" // color for links
#define COLOR_BACKG_RED "\x1b[0;41m"
#define COLOR_RESET "\x1b[0m" // reset color settings
#define COLOR_PURPLE "\x1b[0;35m"
#define COLOR_BACKG_YELLOW "\x1b[0;43m"

#define CPU 2
// Exiting functions
void exitingfromShell(char **args) {
    exit(0);
}

// changing directory
void ChangingDirectory(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "Enter the second argument.\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("");
        }
    }
}

// Usage --help
void gettinghelpCase(char **args) {
    char *gethelpCase =
        "Test cases that included are:\n"
        "  cd       Change the working directory.\n"
        "  exit     Exit the shell.\n"
        "  --help   Print this help text.\n"
        "  -c       user and system CPU time used by the process\n"
        "  -m       size of the operating residential memory used by the process\n"
        "  -b       number of block input and output operations performed by the process\n"
        "  -a       all options at once.\n"
        ;
    printf("%s", gethelpCase);
}

void usrsysCPUTime() {
    
    clock_t startingtime, endingtime;
    int check = 0;
    double CPUthatused = 0;

    startingtime = clock();

    check |= CPU;    

    endingtime = clock();

    CPUthatused = ((double)(endingtime - startingtime))/ CLOCKS_PER_SEC;
    printf("Time that is used by processor is: %f.\n", CPUthatused);
    
}
void sizeofOpresidentMemory() {


   // printf("Size of the operating residential memory used by the process is:\n");
}
void sizeofblockInputOutput() {

   // printf("Time that is used by processor is:.\n");

}
void allOptionsatonce() {
    usrsysCPUTime();
    sizeofOpresidentMemory();
    sizeofblockInputOutput();
}

void SimpleShellRestricted() {
    
}

typedef char *multi_tok_t;
char *MultipleTokenize(char *inpt, multi_tok_t *str, char *delimeter) {
    if(inpt != NULL) *str = inpt;
    if(*str == NULL) return *str;

    char *end = strstr(*str, delimeter);
    if(end == NULL) {
        char *temp = *str;
        *str = NULL;
        return temp;

    }
    char *temp = *str;
    *end = '\0';
    *str = end +strlen(delimeter);
    return temp;
}

multi_tok_t init() {
    return NULL;
}

// A builtin instance associates a command name with a handler function.
struct usage {
    char *name;
    void (*func)(char **args);
};

// Array of built in commands.
struct usage usageforFunctions[] = {
    {"--help", gettinghelpCase},
    {"exit", exitingfromShell},
    {"cd", ChangingDirectory},
    {"-c", usrsysCPUTime},
    {"-m", sizeofOpresidentMemory},
    {"-b", sizeofblockInputOutput},
    {"-a", allOptionsatonce},
    {"./shell -r", ChangingDirectory},
};

// Returns the number of registered commands.
int sizeofUsageFunctions() {
    return sizeof(usageforFunctions) / sizeof(struct usage);
}
int backgroundProcess = 0;
void SimpleExec(char **args) {
    for (int i = 0; i < sizeofUsageFunctions(); i++) {
        if (strcmp(args[0], usageforFunctions[i].name) == 0) {
            usageforFunctions[i].func(args);
            return;
        }
    }
    backgroundProcessing();

    //if(strchr(args, '/') == NULL) {
        int i;
        pid_t cpid = fork();
        struct rlimit rlim;

        if (cpid = 0) {
            execvp(args[0], args);
            perror("");
            exit(1);
        } else if (cpid > 0) {
            int status;
            do {
                waitpid(cpid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        } else {
            if(!backgroundProcess) {
                waitpid(cpid, NULL, 0);
            }
            backgroundProcess = 0;
        }
    //}
}

void backgroundProcessing(char **args) {
    // if(strcmp(args[0], "&") == 0) {
    //     backgroundProcess = 1;
    //     args[0] = NULL;
    // }
}


char** SplittingLines(char *line) {
    int length = 0;
    int capacity = 16;

    char **tokens = malloc(capacity * sizeof(char*));
    if (!tokens) {
        perror("");
        exit(1);
    }

    char *delimiters = " \t\r\n";
    char *token = strtok(line, delimiters);

    while (token != NULL) {
        tokens[length] = token;
        length++;

        if (length >= capacity) {
            capacity = (int) (capacity * 1.5);
            tokens = realloc(tokens, capacity * sizeof(char*));
            if (!tokens) {
                perror("");
                exit(1);
            }
        }

        token = strtok(NULL, delimiters);
    }

    tokens[length] = NULL;
    return tokens;
}

char* ReadingAllPossibleLines() {
    char *line = NULL;
    size_t buflen = 0;
    errno = 0;
    ssize_t strlen = getline(&line, &buflen, stdin);
    if (strlen < 0) {
        if (errno) {
            perror("");
        }
        exit(1);
    }
    return line;
}


int main(int argc, char *argv[]) {

    while (true) {
        printf("shell:> ");
        char *line = ReadingAllPossibleLines();
        char **tokens = SplittingLines(line);

        if(strcmp(argv[0],"clr") == 0){
            /* uses system function to clear the screen*/
            char c[50];
            strcpy( c, "clear");
            system(c);
        }
 
        if (tokens[0] != NULL) {
            SimpleExec(tokens);
        }

        free(tokens);
        free(line);
    }
}