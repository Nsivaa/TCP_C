
#include "protocols.h"
#define NO_ERROR 0

/*
 * addition between integers
 */
int add(int n1, int n2){
	int risultato=n1+n2;
	return risultato;
}

/*
 * multiplication between integers
 */
int mult(int n1, int n2){
	int risultato=n1*n2;
	return risultato;
}

/*
 * subtraction between integers
 */

int sub(int n1, int n2){
	int risultato=n1-n2;
	return risultato;
}

/*
 * division between reals
 */

float division(float n1, float n2){
	if(n2==0){return NAN;}
	float risultato=n1/n2;
	return risultato;
}
/*
 *Main function of the application
 */

int main(int argc, char *argv[]) {
	int my_socket;
	int client_socket;
	int client_len;
	setvbuf(stdout, NULL, _IONBF, 0);
	int result = WSAStartup(MAKEWORD(2,2), &wsa_data);
	if (result != NO_ERROR) {
		printf("Error at WSAStartup()\n");
		return 0;
	}
	my_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	sad.sin_family = AF_INET;
	/*
	 * Standard IP and port
	 */
	sad.sin_addr.s_addr = inet_addr( "127.0.0.1" );
	sad.sin_port = htons( 27015 );
	/*
	 * Socket is closed if bind() fails
	 */
	if (bind(my_socket, (struct sockaddr*) &sad, sizeof(sad)) <0) {
		errorhandler("bind() failed.\n");
		closesocket(my_socket);
		return -1;
	}
	while (1) {
		/*
		 * length of client queue
		 */
		int qlen = 5;
		/*
		 * Socket is closed if listen() fails
		 */
		if (listen (my_socket, qlen) < 0) {
			errorhandler("listen() failed.\n");
		}
		else{
			printf("Waiting for a client to connect...");
		}
		client_len = sizeof(cad);
		/*
		 * Socket is closed if accept() fails
		 */
		if ((client_socket = accept(my_socket, (struct sockaddr*)&cad, &client_len)) < 0) {
			errorhandler("accept() failed.\n");
			closesocket(client_socket);
		}
		else{
			printf("Connection established with %s:%d\n", inet_ntoa(cad.sin_addr), ntohs(cad.sin_port));
		}
		int bytes_rcvd;
		int total_bytes_rcvd = 0;
		while(1)
		{/*
		 * Communication happens using a struct
		 */
			if ((bytes_rcvd = recv(client_socket,(void *)&msg, sizeof(msg), 0)) <= 0)
			{
				errorhandler("Client disconnected\n");
				closesocket(client_socket);
				break;
			}
			/*
			 * Size (in bytes) of received data
			 */
			total_bytes_rcvd += bytes_rcvd;
			msg.message[bytes_rcvd] = '\0';
			/*
			 * Character matrix that will contain client's input
			 */
			char token[2][BUFFERSIZE];
			/*
			 * String that will contain computation's result
			 */
			char ris[BUFFERSIZE];
			strcpy(ris,msg.message);
			int n1, n2;
			float sol;
			/*
			 * Client's input is tokenized
			 */
			strcpy(token[0],strtok(msg.message, " "));

			/*
			 * Controls on the validity of input
			 */
			if(token[0][0]=='+' || token[0][0]=='-' || token[0][0]=='/' || token[0][0]=='x')
			{
				strcpy(token[1],strtok(NULL, " ") );
				strcpy(token[2],strtok(NULL, " ") );
				if(token[0][0]=='/' && atoi(token[2])==0)
				{ strcpy(ris,"Operation invalid"); }
				else{
					n1=atoi(token[1]);
					n2=atoi(token[2]);
					if(token[0][0]=='+'){
						sol=add(n1, n2 );
					}
					if(token[0][0]=='-'){
						sol=sub(n1, n2 );
					}
					if(token[0][0]=='/'){
						sol=division(n1, n2 );
					}
					if(token[0][0]=='x'){
						sol=mult(n1, n2 );
					}
					if(isnan(sol)){
						strcpy(ris,"Can't divide by 0");
					}
					else{
						sprintf(ris, "%.2f", sol);
					}
				}
			}
			else{
				strcpy(ris,"Operation invalid");
			}
			int string_len = strlen(ris);
			if (send(client_socket, ris, string_len, 0) != string_len)
			{
				errorhandler("send() sent a different number of bytes than expected\n");
				closesocket(client_socket);
			}
		}
	}
	closesocket(my_socket);
	clearwinsock();
	return 0;
}





