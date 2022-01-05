NAME - SAURAV KORANGA

ROLL NO. - 20CS60R65

ASSIGNMENT -3

INSTRUCTIONS TO RUN CODE:

1. SERVER should start executing first.
	COMMAND TO RUN SERVER:
		gcc server.c -o server
		./server portnumber

2. CLIENT after server starts executing.
	COMMAND TO RUN CLIENT:
		gcc client.c -o client
		./client localhost portnumber
		
3. If the filename is already present in the client or server end that user is requesting(RETR) or
   sending(STOR) then the program will print a message that the filename is already present at the
   server and client side respectively.

Error cases:

1. If any command is typed in wrong format in client side then server sends an Invalid command message.
	eg. dele or DEL in place of DELE.

2. RETR command: if file already present at client side then drops a message at client end.

3. RETR command: if file not in server directory then print a wrong file name message.

4. RETR command: if any filename other than .c or .txt is typed then prints a message.

5. STOR command same error handling as RETR command.

6. DELE command: if wrong filename given then prints a message.



------------------------------THANK YOU----------------------------
