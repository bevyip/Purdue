
const char * usage =
"                                                               \n"
"usage: http-server                                             \n"
"                                                               \n"
"Different Concurrency options [-f|-f|-p] or empty              \n"
"	[Empty] - Runs a basic server with no concurrency           \n"
"	-f - Create a new Process for each request                  \n"
"	-t Create a new Thread for each request                     \n"
"	-p Use a Pool of Threads                                    \n"
"                                                               \n"
"Where 1024 < port < 65536.                                     \n"
"    if no Port is specified then a default port 1087 is used   \n";

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <string>

int QueueLength = 5;

// Processes time request
void processRequest( int socket );
void processRequestThread( int socket );
void poolSlave( int socket );

pthread_mutex_t mt;
pthread_mutexattr_t mattr;

int main( int argc, char ** argv ) {
    // Print usage if not enough arguments
    int port;
    int mode = -1;
    if ( argc < 2 ) {
        //fprintf( stderr, "%s", usage );
        //exit(1);
        port = 1087;
        mode = 0;
    } else if ( argc == 2 ) {
        //printf("It comes here!\n");
        port = atoi(argv[1]);
        mode = 0;

        if (!strncmp(argv[1], "-", 1)) {
            printf("So this happened!\n");
            port = 1087;
            printf("The default port is: %d\n", port);
            if (argv[1][1] == 'f') {
                mode = 1;
            } else if (argv[1][1]  == 't') {
                mode = 2;
            } else if (argv[1][1] == 'p') {
                printf("yolo\n");
                mode = 3;
            } else {
                fprintf(stderr, "%s\n", usage);
                exit(1);
            }
        }
    } else if ( argc == 3 ) {
        if (argv[1][0] == '-') {
            if (argv[1][1] == 'f') {
                mode = 1;
            } else if (argv[1][1]  == 't') {
                mode = 2;
            } else if (argv[1][1] == 'p') {
                mode = 3;
            } else {
                fprintf(stderr, "%s\n", usage);
                exit(1);
            }
        }
        if (atoi(argv[2]) == 0) {
            fprintf(stderr, "%s", usage);
            exit(1);
        } else {
            port = atoi(argv[2]);
        }
    }

    if (port < 1024 || port > 65535) {
        fprintf(stderr, "%s", usage);
        exit(1);
    }

    printf("And it comes here! The port number is: %d\n", port);

  // Set the IP address and port for this server
  struct sockaddr_in serverIPAddress;
  memset( &serverIPAddress, 0, sizeof(serverIPAddress) );
  serverIPAddress.sin_family = AF_INET;
  serverIPAddress.sin_addr.s_addr = INADDR_ANY;
  serverIPAddress.sin_port = htons((u_short) port);

  // Allocate a socket
  int masterSocket =  socket(PF_INET, SOCK_STREAM, 0);
  if ( masterSocket < 0 ) {
    perror("socket");
    exit( -1 );
  }

  // Set socket options to reuse port. Otherwise we will
  // have to wait about 2 minutes before reusing the sae port number
  int optval = 1;
  int err = setsockopt(masterSocket, SOL_SOCKET, SO_REUSEADDR,
		       (char *) &optval, sizeof( int ) );

  // Bind the socket to the IP address and port
  int error = bind( masterSocket,
		    (struct sockaddr *)&serverIPAddress,
		    sizeof(serverIPAddress) );
  if ( error ) {
    perror("bind");
    exit( -1 );
  }

  // Put socket in listening mode and set the
  // size of the queue of unprocessed connections
  error = listen( masterSocket, QueueLength);
  if ( error ) {
    perror("listen");
    exit( -1 );
  }

  if (mode == 3) {
      printf("Mode == 3\n");
      pthread_mutexattr_init(&mattr);
      pthread_mutex_init(&mt, &mattr);
      pthread_t tid[QueueLength];
      pthread_attr_t attr;
      pthread_attr_init(&attr);
      pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

      for (int i = 0; i < QueueLength; i++) {
          pthread_create(&tid[i], &attr, (void * (*)(void *))poolSlave, (void *)masterSocket);
      }
      pthread_join(tid[0], NULL);
  } else {
      while ( 1 ) {
        // Accept incoming connections
        struct sockaddr_in clientIPAddress;
        int alen = sizeof( clientIPAddress );
        int slaveSocket = accept( masterSocket,
                    (struct sockaddr *)&clientIPAddress,
                    (socklen_t*)&alen);
        if ( slaveSocket < 0 ) {
          perror( "accept" );
          exit( -1 );
        }

        if (mode == 0) {
            if ( slaveSocket < 0 ) {
              perror( "accept" );
              exit( -1 );
            }
            processRequest( slaveSocket );
            close( slaveSocket );
        } else if (mode == 1) {
            pid_t slave = fork();
            if (slave == 0) {
                processRequest(slaveSocket);
                close(slaveSocket);
                exit(1);
            }
            close(slaveSocket);
        } else if (mode == 2) {
            pthread_t tid;
            pthread_attr_t attr;

            pthread_attr_init(&attr);
            pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

            pthread_create(&tid, &attr, (void * (*)(void *)) processRequestThread, (void *) slaveSocket);
        }
    }
  }
}

void processRequestThread(int socket) {
    processRequest(socket);
    close(socket);
}

void poolSlave (int socket) {
    while (1) {
        pthread_mutex_lock(&mt);

        struct sockaddr_in clientIPAddress;
		int alen = sizeof(clientIPAddress);
        int slaveSocket = accept(socket,
			 (struct sockaddr *)&clientIPAddress,
			 (socklen_t*)&alen);

		pthread_mutex_unlock(&mt);

		processRequest(slaveSocket);
		shutdown(slaveSocket,1);
		close(slaveSocket);
    }
}

void processRequest( int socket ) {
    // Buffer used to store the name received from the client
    const int MaxName = 1024;
    char name[ MaxName + 1 ];
    int nameLength = 0;
    int n;
    char newChar;
    int getCheck = 0;
    char nameDoc[1025];
    int nameDocLength = 0;
    int docDone = 0;
    int docCheck = 0;

    while ((n = read(socket, &newChar, sizeof(newChar)))) {
        //printf("%c", newChar);
        name[nameLength++] = newChar;

        if (docCheck) {
            //printf("It came in here\n");
            //int counter = 0;
            char character;
            nameDoc[nameDocLength++] = newChar;
            while ((n = read(socket, &newChar, sizeof(newChar)))) {
                //printf("It went in here: %d\n", counter++);
                if (newChar == ' ') {
                    docDone = 1;
                    nameDoc[nameDocLength++] = '\0';
                    break;
                }
                name[nameLength++] = newChar;
                nameDoc[nameDocLength++] = newChar;
                //printf("%c", newChar);
            }
        }
        if (docDone) {
            //printf("nameDoc: %s\n", nameDoc);
            docDone = 0;
            docCheck = 0;
        }

        if (newChar == 'T') {
            if (name[nameLength - 2] == 'E' && name[nameLength - 3] == 'G') {
                //printf("\nGET Found Bitches\n");
                getCheck = 1;
            }
        }
        if (newChar == ' ') {
            if (getCheck) {
                getCheck = 0;
                docCheck = 1;
            }
        }

        //Break out only when you hit /r/n/r/n
        if (newChar == '\n') {
            if (name[nameLength - 2] == '\r' && name[nameLength - 3] == '\n'
            && name[nameLength - 4] == '\r') {
                break;
            }
        }
    }

    printf("The GET request is: \n%s\n", name);

    //printf("okay it came out\n");

    //Mapping the document path to the real path file
    //char * cwd = (char *) malloc (256 * sizeof(char));
    char cwd[256] = {0};
    getcwd(cwd, 256);
    //printf("Current working directory: %s\n", cwd);

    if (!strncmp(nameDoc, "/icons", strlen("/icons"))
     || !strncmp(nameDoc, "/htdocs", strlen("/htdocs"))
     || !strncmp(nameDoc, "/cgi-bin", strlen("/cgi-bin"))) {
        strcat(cwd, "/http-root-dir/");
        strcat(cwd, nameDoc);
    } else {
        if (!strcmp(nameDoc, "/")) {
            strcpy(nameDoc, "/index.html");
            strcat(cwd, "/http-root-dir/htdocs");
            strcat(cwd, nameDoc);
        } else if (!strcmp(nameDoc, "..")) {
            strcat(cwd, "/http-root-dir/htdocs/");
            strcat(cwd, nameDoc);
        } else {
            strcat(cwd, "/http-root-dir/htdocs");
            strcat(cwd, nameDoc);
        }
    }

    //printf("cwd: %s\n", cwd);

    //printf("nameDoc Updated: %s\n", nameDoc);

    //printf("name finale: %s\n", name);

    if (strstr(nameDoc, "..") != 0) {
        char resolved[MaxName + 1] = {0};
        char * r = realpath(nameDoc, resolved);
        if (r == NULL) {
            perror("realpath");
            exit(0);
        }
        //printf("Real Path: %s\n", resolved);
    }

    char contentType[MaxName + 1] = {0};

    if (strstr(nameDoc, ".html") != 0) {
        strcpy(contentType, "text/html");
    } else if (strstr(nameDoc, ".jpg") != 0) {
        strcpy(contentType, "image/jpeg");
    } else if (strstr(nameDoc, ".gif") != 0) {
        strcpy(contentType, "image/gif");
    } else {
        strcpy(contentType, "text/plain");
    }

    //printf("contentType: %s\n", contentType);

    FILE * file = fopen(cwd, "rb");

    //printf("Does it come here!\n");

    if (file != NULL) {
        //printf("It reaches!\n");

        write(socket, "HTTP/1.1 200 OK", 15);
        write(socket, "\r\n", 2);
        write(socket, "Server: MyServer/1.0", 20);
        write(socket, "\r\n", 2);
        write(socket, "Content-Type ", 14);
        write(socket, contentType, strlen(contentType));
        write(socket, "\r\n", 2);
        write(socket, "\r\n", 2);

        //printf("It reachesssss!\n");

        long count = 0;

        char c;

        while (count = read(fileno(file), &c, sizeof(c))) {
            if (write(socket, &c, sizeof(c)) != count) {
                perror("write");
                return;
            }
        }

        fclose(file);

    } else {
        const char * message = "File not found";

        write(socket, "HTTP/1.1 404 File Not Found", 27);
        write(socket, "\r\n", 2);
        write(socket, "Server: MyServer/1.0", 20);
        write(socket, "\r\n", 2);
        write(socket, "Content-Type: text/html", 23);
        write(socket, "\r\n", 2);
        write(socket, "\r\n", 2);
        write(socket, message, strlen(message));
    }
}
