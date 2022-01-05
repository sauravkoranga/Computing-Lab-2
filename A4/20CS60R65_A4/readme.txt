NAME - SAURAV KORANGA

ROLL NO. - 20CS60R65

ASSIGNMENT -4

INSTRUCTIONS TO RUN CODE:

1. SERVER should start executing first.
	COMMAND TO RUN SERVER:
		gcc server.c -o server
		./server portnumber

2. CLIENT after server starts executing.
	COMMAND TO RUN CLIENT:
		gcc client.c -o client
		./client localhost portnumber
		
3. If the filename is already present in the client or server end that user is requesting(RETR),
   sending(STOR) or CODEJUD then the program will print a message that the filename is already present at the
   server and client side respectively.
   
4. METHOD used: 2nd one 
"""Many people also generating output line by line for each testcase line and after that taking all things as a whole and considering if all the outputs lines matched then sending accpt else wrong. And at any point of time if they are getting tle, runtime then they are sending single mssg. In this case if any errors they are finding then they stop."""

Error cases:

1. If any command is typed in wrong format in client side then server sends an Invalid command message.
	eg. dele or DEL in place of DELE.

2. RETR command: if file already present at client side then drops a message at client end.

3. RETR command: if file not in server directory then print a wrong file name message.

4. RETR command: if any filename other than .c or .txt is typed then prints a message.

5. STOR command same error handling as RETR command.

6. DELE command: if wrong filename given then prints a message.

7. CODEJUD command: has all the error handling from RETR command.

8. CODEJUD command: if any filename other than .c or .cpp is typed then prints a message.

9. CODEJUD command: TLE, RTE, files doesn't open while comparing. 

10. All the Clients close connections if Server closes connection.

11. Clear buffers to prevent incorrect strings.

12. Files that doesn't need input or those file that need input both are handled.(by checking if input.txt is present at server side or not)

13. If for a file testcase.txt is not present then it gives error message that file.txt not present. This functionality is not checked for  input.txt because of point 12.

14. If any code gives TLE or RTE then server sends a message to the client that there is error and matching is not performed.

15. In matching phase '\n' and '\r' are removed beforehand from both testcase.txt and output.txt as in bash file is written with return carriage.

16. If wrong extension is given in CODEJUD command it gives a message to enter correct extension.


------------------------------THANK YOU----------------------------
