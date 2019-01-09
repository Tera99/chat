/********************************************************************* 
 * ** Program Filename: chatclient.c
 * ** Author: Terezie (Tera) Schaller
 * ** Date: 10/28/2018
 * ** Description: client half (in C) of a client-server chat program using socket API.
 * ** Input:hostname and port number on command line
 * ** Output: contacts server listening at given port number
 * *********************************************************************/

//REFERENCES
//Beej's guide
//CS344 - Operating systems, Spring 2018, Benjamin Brewster, Lecture 4.2, client example code
//https://medium.com/@yashitmaheshwary/simple-chat-server-using-sockets-in-c-f72fc8b5b24e
//why is it impossible for me to remember how to print stuff in C?!??!!?
//https://stackoverflow.com/questions/2162758/how-to-print-in-c
//getting user input
//https://stackoverflow.com/questions/6282198/reading-string-from-input-with-space-characteri
//https://stackoverflow.com/questions/1252132/difference-between-scanf-and-fgets
//stackoverflow.com/questions/14232990/comparing-two-strings-in-c
//why cant i remember c strings and pointer stuff?!? arrggg...
//https://stackoverflow.com/questions/17131863/passing-string-to-a-function-in-c-with-or-without-pointers
//https://beginnersbook.com/2014/01/c-passing-pointers-to-functions/
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//ACKNOWLEDGEMENTS
//Thank you to Jiale Lui for extensive help during office hours
//Thank you to all students and TAs that participate in Piazza and Slack
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//libraries
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

/*********************************************************************
 * ** Function: Error function
 * ** REFERENCE: Benjamin Brewster, CS 344 lecture examples
 * ** Description: prints error message and exits
 * ** Parameters: error message
 * ** Pre-Conditions: none
 * ** Post-Conditions: program exits
 * *********************************************************************/

void error(const char *msg){ perror(msg); exit(0); }

/*********************************************************************
 * ** Function: newSocket()
 * ** Description: initiates a new socket
 * ** Parameters: none
 * ** Pre-Conditions: properly set up address struct
 * ** Post-Conditions: new socket is open, returns file descriptor 
 * *********************************************************************/

int newSocket(){//if connecting use arg: struct sockaddr_in * myAddr
	int newSocketFD;

//	printf("This is a new socket\n");
  	//set up the client socket
  	newSocketFD = socket(AF_INET, SOCK_STREAM, 0); //create the socket
  	if (newSocketFD < 0) error("CLIENT: ERROR opening socket");
   
  	//connect to server
  	//having trouble implementing this
  	//cant figure out how to pass address to funcion
	
	return newSocketFD;
}


/*********************************************************************
 * ** Function: mysend()
 * ** Description: sends a message to the server through a previously open socket
 * ** Parameters: socket file descriptor, string
 * ** Pre-Conditions: open socket
 * ** Post-Conditions: message is sent to server via the socket
 * *********************************************************************/

void mysend(int mySocket, char msg[]){
	int numChar;
  	numChar = send(mySocket, msg, strlen(msg), 0);//send message
	//alert user of errors
  	if (numChar < 0) error("CLIENT: ERROR writing to socket");
  	if (numChar < strlen(msg)) printf("CLIENT: WARNING: Not all data written to socket!\n");
}

/*********************************************************************
 * ** Function: myrecv()
 * ** Description: recieves the message from the server
 * ** Parameters: socket file descriptor, cleared string to write to, size of the string
 * ** Pre-Conditions: open socket, message sent by server, memset'd string to write to
 * ** Post-Conditions: server message is written to string
 * *********************************************************************/
void myrecv(int mySocket, char client_msg[], int msg_size){
	int numChar;
	//printf("myrecv: sizeof(client_msg): %d\n", sizeof(client_msg));
  	memset(client_msg, '\0', msg_size); //clear out the buffer again for reuse
 	numChar = recv(mySocket, client_msg, msg_size - 1, 0); //read data from the socket, leaving \0 at the end
  	//printf("myrecv: numChar: %d\n", numChar);
	if (numChar < 0) error("CLIENT: ERROR reading from the socket");

}


//main
int main(int argc, char *argv[])
{
  //declare variables
  int socketFD, portNumber, charsWritten, charsRead;
  struct sockaddr_in serverAddress;
  struct hostent* serverHostInfo;
  char buffer[500];
  char user[10];
  char serv[10];
  char quitstr[] = "\\quit";
  int testSocket;

  //check usage and args
  if (argc < 3){ fprintf(stderr, "USAGE: %s hostname port\n", argv[0]); exit(0);}

  //set up the server address struct
  //relies heavily on B. Brewster CS344 lecture examples
  memset((char*)&serverAddress, '\0', sizeof(serverAddress)); //clear out the address struct
  portNumber = atoi(argv[2]);//convert string to integer to get port number
  serverAddress.sin_family = AF_INET; //create a network capable socket
  serverAddress.sin_port = htons(portNumber); //store the port portNumber
  serverHostInfo = gethostbyname(argv[1]); //convert the machine name into the special form of address
  if (serverHostInfo == NULL) { fprintf(stderr, "CLIENT: ERROR, no such host\n"); exit(0); }
  //copy in the address
  memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length);

  //get user's handle
  printf("Please enter a user name up to 10 characers: ");
  memset(user, '\0', sizeof(user));//clear out user array
  fgets(user, sizeof(user)-1, stdin);
  user[strcspn(user, "\n")] = '\0'; //remove trailing \n added by fgets

  //establish first connection and send handle to server

  //initiate client socket  
  //calls socket and returns int socketFD
  socketFD = newSocket();

  //connect to server
  //I would like to put this in a function
  //so far having trouble getting it right
  if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) //connect to socket address
  error("CLIENT: ERROR connecting");
  
  //send username to server
  mysend(socketFD, user); 

  //get return message from the server
  myrecv(socketFD, serv, sizeof(serv));

  printf("Server name: %s\n", serv);


  close(socketFD); //close the socket
  while(1){
  	//set up the client socket
  	socketFD = newSocket();
   
  	//connect to server
  	if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) //connect to socket address
  	error("CLIENT: ERROR connecting");

  
  	//get an input message from the user
  	printf("%s> ", user);
  	memset(buffer, '\0', sizeof(buffer)); //clear out the buffer array
  	fgets(buffer, sizeof(buffer) - 1, stdin);
 	buffer[strcspn(buffer, "\n")] = '\0'; //remove the trailing \n added by fgets

	//quit client if user types "\quit"
	//might want to send \quit to server first
	//might need to move this
	if (strcmp(buffer, quitstr)==0){
		printf("The server quit. Good bye.\n");
		close(socketFD);
		exit(0);
	}

  	//send message to server
  	mysend(socketFD, buffer);

  	//get return message from the server
  	memset(buffer, '\0', sizeof(buffer)); //clear out the buffer again for reuse
  	myrecv(socketFD, buffer, sizeof(buffer));
	printf("%s> %s\n", serv, buffer);

	//check if server sent quit message
	if (strcmp(buffer, quitstr)==0){
		printf("The server quit. Good bye.\n");
		exit(0);
	}
  
  	close(socketFD); //close the socket
  }
  
  return 0;
}//end of main
