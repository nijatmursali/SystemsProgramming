#include <stdio.h>     
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>   
#include <sys/types.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>   
#include <stdlib.h>  
#include <string.h>  
#include <sys/socket.h>   

//Colors 
#define GREEN "\033[92m"
#define DARKCYAN "\033[36m"
#define BOLDGREEN "\033[1m\033[92m"
#define BOLDRED "\033[1m\033[91m"
#define RESET "\033[0m"
   
int main(int argc, char** argv) {
  int portnumber;
  
  if (argc > 1)
    portnumber = atoi(argv[1]);
  else 
    portnumber = 9000; // puts 9000 as port as default
  
  char *turningoffPort = malloc(1024);
  sprintf(turningoffPort, "fuser -k -n tcp %d", portnumber);
  system(turningoffPort); // closing the port
  
  char *webserver;
  webserver = "127.0.0.1";
  char * weblocation;
  weblocation = getenv("PWD");
  
  int filedescrptforsocket, newfiledescrptforsocket; 
  
  if ((filedescrptforsocket = socket(AF_INET, SOCK_STREAM, 0)) > 0) {   
    fprintf(stdout, "\n%sSocket has been initialized..%s\n", BOLDGREEN, RESET);
  } else {
    fprintf(stderr, "%sThere was error with creating the socket.%s\n", BOLDRED, RESET);
  }
   
  struct sockaddr_in addr;
  
  addr.sin_family = AF_INET;   
  addr.sin_addr.s_addr = inet_addr(webserver);   
  addr.sin_port = htons(portnumber);   
  
  if (bind(filedescrptforsocket, (struct sockaddr *) &addr, sizeof(addr)) == 0){   
    fprintf(stdout, "%sSocket is binding..%s\n\n", BOLDGREEN, RESET);
  } else {
    fprintf(stderr, "%sThere was error with binding the socket.%s\n", BOLDRED, RESET);
  }   
   
  while (1) { 
    if (listen(filedescrptforsocket, 10) < 0) {   
      fprintf(stderr, "%sError happened while listening to socket.%s\n", BOLDRED, RESET);   
      exit(1);   
    }
  
    socklen_t addrlength;   
  
    if ((newfiledescrptforsocket = accept(filedescrptforsocket, (struct sockaddr *) &addr, &addrlength)) < 0) {   
      fprintf(stderr, "%sError happened while accepting the socket.%s\n", BOLDRED, RESET);   
      exit(1);   
    }
    
    char *messagefromsocket = malloc(1024 * 1024); 

    recv(newfiledescrptforsocket, messagefromsocket, 1024 * 1024, 0);
    fprintf(stdout, "%s%s%s\n", DARKCYAN, messagefromsocket, RESET);
    
    int filedescrpt, readeverychar, dataforchar;
    dataforchar = 1024 * 1024;
    char *filebuffer = malloc(dataforchar);
    char path[1024];
    
    char * httptoken[2];
    httptoken[0] = strtok(messagefromsocket, " \t\n");

    if (strncmp(httptoken[0], "GET\0", 4) == 0) {
      httptoken[1] = strtok(NULL, " \t");

      if (strncmp(httptoken[1], "/\0", 2) == 0)
        httptoken[1] = "/index.html";

       strcpy(path, weblocation);
       strcpy(&path[strlen(weblocation)], httptoken[1]);
       

       filedescrpt = open(path, O_RDONLY);
      
 
       if (filedescrpt > 0) {
         send(newfiledescrptforsocket, "HTTP/1.0 200 OK\n\n", 17, 0);
         while ((readeverychar = read(filedescrpt, filebuffer, dataforchar)) > 0)
           write(newfiledescrptforsocket, filebuffer, readeverychar);
       } else {
         send(newfiledescrptforsocket, "HTTP/1.0 404 Not Found\n\n", 24, 0);
         write(newfiledescrptforsocket, "<h1>404 Not Found</h1>", 22);
         //write(newfiledescrptforsocket, "404.html", 8);
       }
    }
    
    close(newfiledescrptforsocket);   
  }   
  
  close(filedescrptforsocket);
  
  return 0;   
}