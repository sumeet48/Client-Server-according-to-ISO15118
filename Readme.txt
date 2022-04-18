Code explanation:

1) The code establishes client(EV) and server(SECC) concept according to ISO 15118.
2) The code is written in C programming.
3) Code functionality
     - Server(SECC) will be waiting for the client(EV) to connect.
     - Clinet(EV) connetcs to server(SECC) and sends hearder and payload to server(SECC).
     - Server(SECC) recives the client(EV) header and payload and replies client(EV) with server(SECC) header and payload.

Steps to run code
1) Export server.c and client.c to vi editor.
2) Compile server.c by using command gcc server.c -o server
3) Run server.c by using command ./server (port number)
4) Complie client.c by using comand gcc clinet.c -o client
5) Run client.c by using comm  ./client(port number)

	 