README
Program 1  
Terezie Schaller
CS 372, Fall 2018

This assignment was written and tested on flip 2.

Server: Python
Client: C

To run:
1. Open a command line prompt, navigate to location of files, and start the server

chmod +x chatserv (first time only)
python chatserve <port#>

2. Open a separate window and navigate to location of files. Compile and run

gcc -o chatclient chatclient.c
./chatclient localhost <port#>

3. Begin chatting. When client first contacts the server, the user will be asked to enter a user name, and then must send the first message to server.

4. Client and server must alternate messages beginning with client.

5. Enter "\quit" from either terminal to stop chatting.

Note to grader: Server keeps running and will accept new clients, but does not rename them. Quitting from the client is slightly buggy, and I would have liked to work on it more.

Sources are cited at the top of each program. Server program relies heavily on CS 372 lecture material. Client program relies heavily on CS 344 lecture examples by Benjamin Brewster. 