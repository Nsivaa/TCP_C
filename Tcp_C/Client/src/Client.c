/*
 ============================================================================
 Name        : Client.c
 Author      : Mark
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */



#include "protocols.h"

int main(int argc, char *argv[]) {
	int connectionfailed=0;
	char ipport[30];
	char opz;
	char ip[20];
	int port;
	strcpy(ip, "127.0.0.1" );
	port=27015;
	setvbuf(stdout, NULL, _IONBF, 0);
	int result = WSAStartup(MAKEWORD(2 ,2), &wsa_data);
	if (result != 0) {
		printf ("error at WSAStartup\n");
		return -1;
	}
	c_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (c_socket < 0) {
		errorhandler("socket creation failed.\n");
		closesocket(c_socket);
		clearwinsock();
		return -1;
	}
	while(1){
		/*
		 * Standard IP is localhost
		 */
		if(argc==1 || connectionfailed){
		do{
			printf("Would you like to use standard IP address and port? Y/N ");
			opz=getchar();
			fflush(stdin);
		}
		while(opz!='Y' && opz!='y' && opz!='N' && opz!='n' );



		/*
		 * User can input different IP and port than localhost
		 */
		if(opz=='N' || opz=='n')
		{
			printf("\nInput ip and port ");
			gets(ipport);
			printf("\n");
			strcpy(ip,strtok(ipport, " ") );
			port=atoi(strtok(NULL, " "));
			fflush(stdin);
		}
		else
			/*
			 * localhost
			 */
		{
			strcpy(ip, "127.0.0.1" );
			port=27015;
			printf("\n");
		}
		}
		else{
			strcpy(ip, argv[1]);
			port=atoi(argv[2]);
			printf("\n");
		}

		memset(&sad, 0, sizeof(sad));
		/*
		 * connection to server
		 */
		sad.sin_family = AF_INET;
		sad.sin_addr.s_addr = inet_addr(ip);
		sad.sin_port = htons(port);
		if (connect(c_socket, (struct sockaddr *)&sad, sizeof(sad))< 0)
		{
			errorhandler( "Failed to connect.\n" );
			connectionfailed=1;
		}
		else{
			connectionfailed=0;
			printf("Connected.\n");
			printf("Input your operands in this format: [operator] [number] [number]\n");
			printf("The valid operators are: x, +, - and /\n");
			printf("Input = to quit\n");
			do{
				printf("\nWaiting for an input\n");
				printf("Input :");
				gets(msg.message);
				if(msg.message[0]=='='){break;}
				if (send(c_socket, (void *)&msg, sizeof(msg), 0 ) != sizeof(msg))
				{
					/*
					 * Socket is closed if send() fails
					 */
					errorhandler("send() sent a different number of bytes than expected");
					closesocket(c_socket);
					clearwinsock();
					return -1;
				}

				/*
				 * Receiving result from server
				 */
				printf("Result:");
				if ((bytes_rcvd = recv(c_socket, buf, BUFFERSIZE - 1, 0)) <= 0)
				{
					/*
					 * Socket is closed if recv() fails
					 */
					errorhandler("recv() failed or connection closed prematurely");
					closesocket(c_socket);
					clearwinsock();
					return -1;
				}
				/*
				 * Size(in bytes) of received data
				 */
				total_bytes_rcvd += bytes_rcvd;
				buf[bytes_rcvd] = '\0';
				printf("%s \n", buf);
				/*
				 * Connection is interrupted when user inputs '='
				 */
				if(msg.message[0]=='='){break;}
			}
			while(1);
		}
		if(msg.message[0]=='='){break;}
	}
	closesocket(c_socket);
	clearwinsock();
	printf("Disconnected.\n");
	system("pause");
	return(0);
}




