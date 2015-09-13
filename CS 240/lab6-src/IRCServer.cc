
const char * usage =
"                                                               \n"
"IRCServer:                                                   \n"
"                                                               \n"
"Simple server program used to communicate multiple users       \n"
"                                                               \n"
"To use it in one window type:                                  \n"
"                                                               \n"
"   IRCServer <port>                                          \n"
"                                                               \n"
"Where 1024 < port < 65536.                                     \n"
"                                                               \n"
"In another window type:                                        \n"
"                                                               \n"
"   telnet <host> <port>                                        \n"
"                                                               \n"
"where <host> is the name of the machine where talk-server      \n"
"is running. <port> is the port number you used when you run    \n"
"daytime-server.                                                \n"
"                                                               \n";

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "IRCServer.h"
#include "nextword.h"

int QueueLength = 5;
int spaceCount;
int userCount = 0;
int roomCount;
const char * argsRoomName = (char *) malloc (100 * sizeof(char));
const char * currentMessage = (char *) malloc (1000 * sizeof(char));

struct UserPass {
	char * username;
	char * password;
	int rooms[50];
};

char username[100];
char password[100];

struct Rooms {
	char * users[100];
	char * roomName;
	char * messages[100];
	int userRoomCount;
	int currentMessageCount;
	int overDrive;
};

typedef Rooms Rooms;
Rooms * RoomArray;

typedef UserPass UserPass;
UserPass * UserArray; //= (UserPass *) malloc (1000 * sizeof(UserPass));
//nextword n;

int IRCServer::open_server_socket(int port) {

	// Set the IP address and port for this server
	struct sockaddr_in serverIPAddress; 
	memset( &serverIPAddress, 0, sizeof(serverIPAddress) );
	serverIPAddress.sin_family = AF_INET;
	serverIPAddress.sin_addr.s_addr = INADDR_ANY;
	serverIPAddress.sin_port = htons((u_short) port);
  
	// Allocate a socket
	int masterSocket =  socket(PF_INET, SOCK_STREAM, 0);
	if ( masterSocket < 0) {
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

	return masterSocket;
}

void
IRCServer::runServer(int port)
{
	int masterSocket = open_server_socket(port);

	initialize();
	
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
		
		// Process request.
		processRequest( slaveSocket );		
	}
}

int main (int argc, char ** argv) {
	// Print usage if not enough arguments
	if ( argc < 2 ) {
		fprintf( stderr, "%s", usage );
		exit( -1 );
	}
	
	// Get the port from the arguments
	int port = atoi( argv[1] );

	IRCServer ircServer;

	// It will never return
	ircServer.runServer(port);
	
}

//
// Commands:
//   Commands are started y the client.
//
//   Request: ADD-USER <USER> <PASSWD>\r\n
//   Answer: OK\r\n or DENIED\r\n
//
//   REQUEST: GET-ALL-USERS <USER> <PASSWD>\r\n
//   Answer: USER1\r\n
//            USER2\r\n
//            ...
//            \r\n
//
//   REQUEST: CREATE-ROOM <USER> <PASSWD> <ROOM>\r\n
//   Answer: OK\n or DENIED\r\n
//
//   Request: LIST-ROOMS <USER> <PASSWD>\r\n
//   Answer: room1\r\n
//           room2\r\n
//           ...
//           \r\n
//
//   Request: ENTER-ROOM <USER> <PASSWD> <ROOM>\r\n
//   Answer: OK\n or DENIED\r\n
//
//   Request: LEAVE-ROOM <USER> <PASSWD>\r\n
//   Answer: OK\n or DENIED\r\n
//
//   Request: SEND-MESSAGE <USER> <PASSWD> <MESSAGE> <ROOM>\n
//   Answer: OK\n or DENIED\n
//
//   Request: GET-MESSAGES <USER> <PASSWD> <LAST-MESSAGE-NUM> <ROOM>\r\n
//   Answer: MSGNUM1 USER1 MESSAGE1\r\n
//           MSGNUM2 USER2 MESSAGE2\r\n
//           MSGNUM3 USER2 MESSAGE2\r\n
//           ...\r\n
//           \r\n
//
//    REQUEST: GET-USERS-IN-ROOM <USER> <PASSWD> <ROOM>\r\n
//    Answer: USER1\r\n
//            USER2\r\n
//            ...
//            \r\n
//

void IRCServer::processRequest (int fd) {
	// Buffer used to store the comand received from the client
	const int MaxCommandLine = 1024;
	char commandLine[ MaxCommandLine + 1 ];
	int commandLineLength = 0;
	int n;
	
	// Currently character read
	unsigned char prevChar = 0;
	unsigned char newChar = 0;
	
	//
	// The client should send COMMAND-LINE\n
	// Read the name of the client character by character until a
	// \n is found.
	//

	// Read character by character until a \n is found or the command string is full.
	while ( commandLineLength < MaxCommandLine &&
		read( fd, &newChar, 1) > 0 ) {

		if (newChar == '\n' && prevChar == '\r') {
			break;
		}
		
		commandLine[ commandLineLength ] = newChar;
		commandLineLength++;

		prevChar = newChar;
	}
	
	// Add null character at the end of the string
	// Eliminate last \r
	commandLineLength--;
        commandLine[ commandLineLength ] = 0;
	char * commandWord;

	//Adds Command name
	const char * command = strdup(nextWordString(commandLine));
	int c = strlen(command) + spaceCount;
	
	//Adds User name
	const char * user = strdup(nextWordString(commandLine + c));
	int u = strlen(user) + spaceCount;
	
	//Adds Password
	const char * password = strdup(nextWordString(commandLine + c + u));
	int p = strlen(password) + spaceCount;
	
	//Adds Args
	const char * args;
	if (strlen(commandLine) > c + u + p) {
		args = strdup(commandLine + c + u + p);
		
		char * temp = strdup(args);
		//Get Room name

		argsRoomName = strdup(nextWordString(temp));
		int r = strlen(argsRoomName) + spaceCount;

		//Get Message
		if (strlen(args) > strlen(argsRoomName)) {
			currentMessage = strdup(args + r);
		} else {
			currentMessage = "";
		}
		//char * temp = (char *) malloc (1000 * sizeof(char));
		//temp = strdup(args);
		//argsRoomName = strdup(nextWordString(temp));
		//int r = strlen(argsRoomName) + spaceCount;
	} else {
		printf("Write something\n");
		args = "";
		argsRoomName = "";
		currentMessage = "";
	}
	
	//Seperate args to roomName and message
	//char * temp = (char *) malloc (1000 * sizeof(char));
	//temp = strdup(args);
	
	//argsRoomName = strdup(nextWordString(temp));
	//int r = strlen(argsRoomName) + spaceCount;

	//currentMessage = strdup(argsRoomName + r);
	
	printf("RECEIVED: %s\n", commandLine);

	/*printf("The commandLine has the following format:\n");
	printf("COMMAND <user> <password> <arguments>. See below.\n");
	printf("You need to separate the commandLine into those components\n");
	printf("For now, command, user, and password are hardwired.\n");

	const char * command = "ADD-USER";
	const char * user = "peter";
	const char * password = "spider";
	const char * args = "";
	*/

	printf("command=%s\n", command);
	printf("user=%s\n", user);
	printf( "password=%s\n", password );
	printf("args=%s\n", args);
	printf("roomName=%s\n", argsRoomName);
	printf("currentMessage=%s\n", currentMessage);
	/*for (int i = 0; i < userCount; i++) {
		printf("Username: %s\nPassword: %s\n", UserArray[i].username, UserArray[i].password);
	}*/
	
	
	if (!strcmp(command, "ADD-USER")) {
		addUser(fd, user, password, args);
	}
	else if (!strcmp(command, "CREATE-ROOM")) {
		createRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "LIST-ROOMS")) {
		listRooms(fd, user, password, args);
	}
	else if (!strcmp(command, "ENTER-ROOM")) {
		enterRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "LEAVE-ROOM")) {
		leaveRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "SEND-MESSAGE")) {
		sendMessage(fd, user, password, args);
	}
	else if (!strcmp(command, "GET-MESSAGES")) {
		getMessages(fd, user, password, args);
	}
	else if (!strcmp(command, "GET-USERS-IN-ROOM")) {
		getUsersInRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "GET-ALL-USERS")) {
		getAllUsers(fd, user, password, args);
	}
	else {
		const char * msg =  "UNKNOWN COMMAND\r\n";
		write(fd, msg, strlen(msg));
	}

	// Send OK answer
	//const char * msg =  "OK\n";
	//write(fd, msg, strlen(msg));

	close(fd);	
}

void IRCServer::initialize() {
	
	userCount = 0;
	UserArray = (UserPass *) malloc (1000 * sizeof(UserPass));
	FILE * file = fopen("userPass.txt", "a+");
	char * d = (char *) malloc (100 * sizeof(char));
	int i = 0;
	while ((d = nextWord(file)) != NULL) {
		if (i == 0) {
			UserArray[userCount].username = strdup(d);
			i = 1;
			continue;
		} else if (i == 1) {
			UserArray[userCount].password = strdup(d);
			i = 0;
			userCount++;
			continue;
		}
	}

	roomCount = 0;
	RoomArray = (Rooms *) malloc (100 * sizeof(Rooms));
	for (int j = 0; j < 100; j++) {
		for (int k = 0; k < 100; k++) {
			RoomArray[j].messages[k] = (char *) malloc (1024 * (sizeof(char)));
		}
	}

	//fclose(file);

	// Open password file
	
	// Initialize users in room

	// Initalize message list
	
}

bool IRCServer::checkPassword(int fd, const char * user, const char * password) {
	// Here check the password
	
	for (int i = 0; i < userCount; i++) {
		if (!strcmp(UserArray[i].username, user)) {
			if (!strcmp(UserArray[i].password, password)) {
				return true;
			} else {
				return false;
			}
		}
	}

	return false;
}

void IRCServer::addUser(int fd, const char * user, const char * password, const char * args) {
	// Here add a new user. For now always return OK.
	int flag = 0;
	FILE * file = fopen("userPass.txt", "a+");
	char * u = strdup(user);
	char * p = strdup(password);
	for (int i = 0; i < userCount; i++) {
		printf("%s , %d\n", UserArray[i].username, strcmp(UserArray[i].username, u));
		if (strcmp(UserArray[i].username, u) == 0) {
			flag = 1;
			//fprintf(file, "Found match");
			break;
		}
	}

	if (flag != 1) {
		UserArray[userCount].username = u;
		UserArray[userCount].password = p;
		fprintf(file, "%s %s\n", u, p);
		userCount++;
	} else {
		const char * msg =  "NO\r\n";
		write(fd, msg, strlen(msg));
		return;	
	}

	fclose(file);
	const char * msg =  "OK\r\n";
	write(fd, msg, strlen(msg));
	return;
}

void IRCServer::createRoom(int fd, const char * user, const char * password, const char * args) {
	int flag  = 0;

	if (checkPassword(fd, user, password)) {
		for (int i = 0; i < roomCount; i++) {
			if (!strcmp(RoomArray[i].roomName, argsRoomName)) {
				flag = 1;
				printf("args %s, struct %s, args %d, struct %d\n", argsRoomName, RoomArray[i].roomName, strlen(argsRoomName), strlen(RoomArray[i].roomName));
				break;
			}
		}
		if (flag != 1) {
			RoomArray[roomCount].roomName = strdup(argsRoomName);
			//RoomArray[roomCount].users[RoomArray[roomCount].userRoomCount] = strdup(user);
			//RoomArray[roomCount].userRoomCount++;
			roomCount++;
			printf("%d\n", roomCount);
			const char * msg =  "OK\r\n";
			write(fd, msg, strlen(msg));
		} else {
			//Go do shit
			printf("Room already exists fool\n");
			const char * ms = "NO\r\n";
			write(fd, ms, strlen(ms));
			return;
		}
	} else {
		//printf("ERROR (Wrong password)\r\n");
		const char * msg = "ERROR (Wrong password)\r\n";
		write(fd, msg, strlen(msg));
	}
}

void IRCServer::listRooms(int fd, const char * user, const char * password, const char * args) {
	if (checkPassword(fd, user, password)) {
		for (int i = 0; i < roomCount; i++) {
			printf("%s\r\n", RoomArray[i].roomName);
			printf("%i", roomCount);
			char * msg;
			strcpy(msg, RoomArray[i].roomName);
			strcat(msg, "\r\n");
			write(fd, msg, strlen(msg));
			printf("msg %s\n", msg);
		}
	} else {
		//printf("ERROR (Wrong password)\r\n");
		const char * msg = "ERROR (Wrong password)\r\n";
		write(fd, msg, strlen(msg));
	}
}

void IRCServer::enterRoom(int fd, const char * user, const char * password, const char * args) {
	int tempRoomArrayPos = 0;
	int flag = 0;
	int anotherflag = 0;
	char * message;
	if (checkPassword(fd, user, password)) {
		for (int j = 0; j < roomCount; j++) {
			if (!strcmp(RoomArray[j].roomName, argsRoomName)) {
				tempRoomArrayPos = j;
				anotherflag = 1;
				break;
			}
		}
		if (anotherflag == 0) {
			strcpy(message, "ERROR (No room)\r\n");
			write(fd, message, strlen(message));
			return;
		}
		for (int i = 0; i < RoomArray[tempRoomArrayPos].userRoomCount; i++) {
			if (!strcmp(user, RoomArray[tempRoomArrayPos].users[i])) {
				flag = 1;
				break;
			}
		}
		if (flag == 0) {
			RoomArray[tempRoomArrayPos].users[RoomArray[tempRoomArrayPos].userRoomCount] = strdup(user);
			RoomArray[tempRoomArrayPos].userRoomCount++;
			const char * msg =  "OK\r\n";
			write(fd, msg, strlen(msg));
		} else {
			//Go fuck yourself a fourth time
			const char * msg =  "OK\r\n";
			write(fd, msg, strlen(msg));
		}
	} else {
		//printf("ERROR (Wrong password)\r\n");
		const char * msg = "ERROR (Wrong password)\r\n";
		write(fd, msg, strlen(msg));
	}
}

void IRCServer::leaveRoom(int fd, const char * user, const char * password, const char * args) {
	int tempRoomArrayPos = 0;
	int tempRoomUserPos = 0;
	int flag = 0;
	int flagtwo = 0;
	char * message = (char *) malloc (40 * sizeof(char));
	if (checkPassword(fd, user, password)) {
		for (int i = 0; i < roomCount; i++) {
			if (!strcmp(RoomArray[i].roomName, argsRoomName)) {
				tempRoomArrayPos = i;
				flag = 1;
				break;
			}
		}
		if (flag != 1) {
			//Go fuck yourself again

		} else {
			for (int i = 0; i < RoomArray[tempRoomArrayPos].userRoomCount; i++) {
				if (!strcmp(RoomArray[tempRoomArrayPos].users[i], user)) {
					tempRoomUserPos = i;
					flagtwo = 1;
					break;
				}
			}
			if (flagtwo == 1) {
				for (int j = tempRoomUserPos; j < RoomArray[tempRoomArrayPos].userRoomCount; j++) {
					RoomArray[tempRoomArrayPos].users[j] = RoomArray[tempRoomArrayPos].users[j + 1];
				}
				RoomArray[tempRoomArrayPos].userRoomCount--;
				const char * msg = "OK\r\n";
				write(fd, msg, strlen(msg));
			} else {
				//How many times do I have to tell you to go fuck yourself?!?!?!?!?
				strcpy(message, "ERROR (No user in room)");
				//const char * ms = "\r\n";
				strcat(message, "\r\n");
				write(fd, message, strlen(message));
				//strcpy(message, "\r\n");
				//write(fd, message, strlen(message));
			}
		}
	} else {
		const char * msg = "ERROR (Wrong password)\r\n";
		write(fd, msg, strlen(msg));
	}
}

void IRCServer::sendMessage(int fd, const char * user, const char * password, const char * args) {
	int tempRoomArrayPos = 0;
	int flag = 0;
	char * temp = (char *) malloc (1000 * sizeof(char));
	if (checkPassword(fd, user, password)) {
		for (int i = 0; i < roomCount; ++i) {
			if (!strcmp(argsRoomName, RoomArray[i].roomName)) {
				tempRoomArrayPos = i;
				flag = 1;
				break;
			}
		}
		int flags = 0;
		for (int j = 0; j < RoomArray[tempRoomArrayPos].userRoomCount; j++) {
			if (!strcmp(RoomArray[tempRoomArrayPos].users[j], user)) {
				flags = 1;
				break;
			}
		}
		
		if (flag != 1) {
			
		}
		if (flags != 1) {
			const char * msg = "ERROR (user not in room)\r\n";
			write(fd, msg, strlen(msg));
			return;
		}
		
		if (RoomArray[tempRoomArrayPos].currentMessageCount < 100) {
			//sprintf(RoomArray[tempRoomArrayPos].messages[RoomArray[tempRoomArrayPos].currentMessageCount], "%d %s %s", RoomArray[tempRoomArrayPos].currentMessageCount, user, currentMessage);
			//write(fd, "message acknowledged", 20);
			char t[5];
			//itoa(RoomArray[tempRoomArrayPos].currentMessageCount, t, 10);
			toString(t, RoomArray[tempRoomArrayPos].currentMessageCount);
			//temp = strdup(currentMessage);
			//sprintf(temp, "%d", RoomArray[tempRoomArrayPos].currentMessageCount);
			//temp = strcat(temp, " ");
			strcpy(temp, t);
			//strcpy(temp, t);
			strcat(temp, " ");
			strcat(temp, user);
			strcat(temp, " ");
			strcat(temp, currentMessage);
			RoomArray[tempRoomArrayPos].messages[RoomArray[tempRoomArrayPos].currentMessageCount] = strdup(temp);
			RoomArray[tempRoomArrayPos].currentMessageCount++;
			const char * msgs = "OK\r\n";
			write(fd, msgs, strlen(msgs));
		} else {
			//Do shit
			int j;
			for (j = 1; j < 100; j++) {
				RoomArray[tempRoomArrayPos].messages[j] = RoomArray[tempRoomArrayPos].messages[j - 1];
			}
			
			//RoomArray[tempRoomArrayPos].messages[j] = strdup(currentMessage);
			//sprintf(RoomArray[tempRoomArrayPos].messages[j], "%d, %s, %s", RoomArray[tempRoomArrayPos].currentMessageCount, user, currentMessage);
			temp = strcpy(temp, user);
			temp = strcat(temp, " ");
			temp = strcat(temp, currentMessage);
			RoomArray[tempRoomArrayPos].messages[j] = strdup(temp);
			RoomArray[tempRoomArrayPos].currentMessageCount++;
			RoomArray[tempRoomArrayPos].overDrive++;
			const char * msg = "OK\r\n";
			write(fd, msg, strlen(msg));
		}
	} else {
		//printf("ERROR (Wrong password)\r\n");
		const char * msg = "ERROR (Wrong password)\r\n";
		write(fd, msg, strlen(msg));
	}
}

void IRCServer::getMessages(int fd, const char * user, const char * password, const char * args) {
	int tempRoomArrayPos = 0;
	int flag = 0;
	int t;
	int bflag = 0;
	if (argsRoomName[0] == '0') {
		argsRoomName = strdup(currentMessage);
	} else if (atoi(argsRoomName) != 0) {
		t = atoi(argsRoomName);
		argsRoomName = strdup(currentMessage);
	} else {
		t = RoomArray[tempRoomArrayPos].overDrive;
	}
	if (checkPassword(fd, user, password)) {
		for (int i = 0; i < roomCount; i++) {
			if (!strcmp(RoomArray[i].roomName, argsRoomName)) {
				tempRoomArrayPos = i;
				flag = 0;
				break;
			}
		}
		for (int j = 0; j < RoomArray[tempRoomArrayPos].userRoomCount; j++) {
			if (!strcmp(RoomArray[tempRoomArrayPos].users[j], user)) {
				bflag = 1;
				break;
			}
		}

		char * message = (char *) malloc (1000 * sizeof(char));
		//int t;
		/*if (atoi(argsRoomName) != 0) {
			t = atoi(argsRoomName);
		} else {
			t = RoomArray[tempRoomArrayPos].overDrive;
		}*/

		if (flag == 0) {
			if (bflag == 1) {
				/*if (t > RoomArray[tempRoomArrayPos].currentMessageCount) {
					const char * msg = "NO-NEW-MESSAGES\r\n";
					write(fd, msg, strlen(msg));
					return;
				}*/
				//char m[5];
				//toString(m, t);
				//write(fd, m, strlen(m));
				int ff = 0;
				for (int j = t; j < RoomArray[tempRoomArrayPos].currentMessageCount; j++) {
					ff = 1;
					strcpy(message, RoomArray[tempRoomArrayPos].messages[j]);
					strcat(message, "\r\n");
					write(fd, message, strlen(message));
					printf("%s", message);
				}
				if (ff == 0) {
					//char * mssg = (char *) malloc (50 * sizeof(char));
					strcpy(message, "NO-NEW-MESSAGES\r\n");
					write(fd, message, strlen(message));
					return;
				}
				strcpy(message, "\r\n");
				write (fd, message, strlen(message));
			} else {
				//printf("Chutiye, room mai hai nahi aur messages maang raha hai");
				const char * msg = "ERROR (User not in room)\r\n";
				//strcpy(message, "NO-NEW-MESSAGES\r\n");
				write (fd, msg, strlen(msg));
				return;
			}
		}
	} else {
		const char * msg = "ERROR (Wrong password)\r\n";
		write(fd, msg, strlen(msg));
	}
}

void IRCServer::getUsersInRoom(int fd, const char * user, const char * password, const char * args) {
	int tempRoomArrayPos;
	int flag = 0;
	if (checkPassword(fd, user, password)) {
		for (int i = 0; i < roomCount; i++) {
			if (!strcmp(RoomArray[i].roomName, argsRoomName)) {
				tempRoomArrayPos = i;
				flag = 1;
				break;
			}
		}
		int bflag = 0;
		/*for (int j = 0; j < RoomArray[tempRoomArrayPos].userRoomCount; j++) {
			if (!strcmp(RoomArray[tempRoomArrayPos].users[j], user)) {
				bflag = 1;
				break;
			}
		}*/
		/*if (bflag != 1) {
			const char * msg = "\r\n";
			write(fd, msg, strlen(msg));
			return;
		}*/
		if (flag != 1) {
			//Go fuck yourself
			return;
		}
		char * tempUname = (char *) malloc (80 * sizeof(char));
		for (int j = 0; j < RoomArray[tempRoomArrayPos].userRoomCount - 1; j++) {
			for (int k = j + 1; k < RoomArray[tempRoomArrayPos].userRoomCount; k++) {
				if (strcmp(RoomArray[tempRoomArrayPos].users[j], RoomArray[tempRoomArrayPos].users[k]) > 0) {
					tempUname = strdup(RoomArray[tempRoomArrayPos].users[j]);
					RoomArray[tempRoomArrayPos].users[j] = strdup(RoomArray[tempRoomArrayPos].users[k]);
					RoomArray[tempRoomArrayPos].users[k] = strdup(tempUname);
				}
			}
		}
		char * message = (char *) malloc (80 * sizeof(char));
		for (int i = 0; i < RoomArray[tempRoomArrayPos].userRoomCount; i++) {
			strcpy(message, RoomArray[tempRoomArrayPos].users[i]);
			strcat(message, "\r\n");
			write(fd, message, strlen(message));
			printf("%s\n", RoomArray[tempRoomArrayPos].users[i]);
		}
		strcpy(message, "\r\n");
		write(fd, message, strlen(message));
	} else {
		//printf("ERROR (Wrong password)\r\n");
		const char * msg = "ERROR (Wrong password)\r\n";
		write(fd, msg, strlen(msg));
	}
}

void IRCServer::getAllUsers(int fd, const char * user, const char * password,const  char * args) {
	char * tempUsername = (char *) malloc (100 * sizeof(char));
	char * tempPassword = (char *) malloc (100 * sizeof(char));
	if (checkPassword(fd, user, password)) {
		for (int i = 0; i < userCount - 1; i++) {
			for (int j = i + 1; j < userCount; j++) {
				if (strcmp(UserArray[i].username, UserArray[j].username) > 0) {
					tempUsername = strdup(UserArray[i].username);
					tempPassword = strdup(UserArray[i].password);
					UserArray[i].username = strdup(UserArray[j].username);
					UserArray[i].password = strdup(UserArray[j].password);
					UserArray[j].username = strdup(tempUsername);
					UserArray[j].password = strdup(tempPassword);
				}
			}
		}
		for (int k = 0; k < userCount; k++) {
			char * writes = (char *) malloc (80 * sizeof(char));
			strcpy(writes, UserArray[k].username);
			strcat(writes, "\r\n");
			write(fd, writes, strlen(writes));
			//printf("%s\r\n", UserArray[k].username);
		}
		char * message = (char *) malloc (5 * sizeof(char));
		strcpy(message, "\r\n");
		write(fd, message, strlen(message));
		return;
	} else {
		const char * msg = "ERROR (Wrong password)\r\n";
		write(fd, msg, strlen(msg));
		return;
	}
}

char * IRCServer::nextWordString (char * string) {
	char * temp = string;
	char * word = (char *) malloc (1024 * sizeof(char));
	char * wtemp = word;
	spaceCount = 0;
	while (*temp != '\0') {
		if (*temp == ' ' ||  *temp == '\r') {
			*wtemp = '\0';
			while (*temp == ' ') {
				temp++;
				spaceCount++;
			}
			return word;
		} else {
			*wtemp = *temp;
			wtemp++; temp++;
		}
	}
	if (*temp == '\0') {
		*wtemp = '\0';
		return word;
	}
	return NULL;
}

#define MAXWORD 200
char word[MAXWORD];
int wordLength;
char * IRCServer::nextWord (FILE * fd) {
	int c = 0;
	int i = 0;

	while ((c = fgetc(fd)) != EOF) {
		if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
			if (i > 0) {
				word[i] = '\0';
				return word;
			}
		} else {
			word[i] = c;
			i++;
		}
	}
	if (i > 0) {
		word[i] = '\0';
		return word;
	}
	return NULL;
}

void IRCServer::toString(char str[], int num) {
    int i, rem, len = 0, n;
    n = num;
    if (n == 0) {
    	str[len] = '0';
	str[len + 1] = '\0';
	return;
    }
	 while (n != 0) {
		len++;
		n /= 10;
	}
	for (i = 0; i < len; i++) {
		rem = num % 10;
		num = num / 10;
		str[len - (i + 1)] = rem + '0';
	}
	str[len] = '\0';
}
