/*
 ============================================================================
 Name        : ClientEsoneroUDP.c
 Author      : Antonio Sequenza
 Date        : 19/12/2021
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#if defined WIN32
#include <winsock.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#endif
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "ClientProtocol.h"

//This procedure checks the port
void controlPort(int port) {
	if (port < 0 || port > 65535) {
		printf("Bad port number\n");
		system("pause");
		exit(EXIT_FAILURE);
	}
}

//This procedure checks the input operation
void controlInput(struct operation op) {
	if (op.oper == '=') {
		exit(EXIT_FAILURE);
	}

	if (op.oper == '/' && op.num2 == '0') {
		system("pause");
		exit(EXIT_FAILURE);
	}
}

void ErrorHandler(char *errorMessage) {
	printf(errorMessage);
}

void ClearWinSock() {
#if defined WIN32
	WSACleanup();
#endif
}

int main(int argc, char *argv[]) {

	//WinSock initialization
#if defined WIN32
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("error at WSASturtup\n");
		return EXIT_FAILURE;
	}
#endif

	int sock;
	struct sockaddr_in echoServAddr;
	struct sockaddr_in fromAddr;
	struct operation op;
	struct hostent *host;
	int fromSize;
	char *name;
	int port;

	//Creation of the socket
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		ErrorHandler("socket() failed");
	}

	if (argc > 1) {
		name = argv[1];
	} else {
		name = "localhost";
	}

	if (argc > 2) {
		port = atoi(argv[2]);
	} else {
		port = PORT;
	}

	host = gethostbyname(name);
	if (host == NULL) {
		fprintf(stderr, "gethostbyname() failed.\n");
		exit(EXIT_FAILURE);
	}
	struct in_addr *ina = (struct in_addr*) host->h_addr_list[0];
	char *ip = inet_ntoa(*ina);

	memset(&echoServAddr, 0, sizeof(echoServAddr));
	echoServAddr.sin_family = PF_INET;
	echoServAddr.sin_port = htons(port);
	echoServAddr.sin_addr.s_addr = inet_addr(ip);

	//Receiving and sending messages
	printf(
			"Enter the equation to send in this format: [operator] [number] [number]\n");
	while (1) {
		printf(">");
		scanf("%s %d %d", &op.oper, &op.num1, &op.num2);

		controlInput(op);
		sendto(sock, (void*) &op, sizeof(struct operation), 0,
				(struct sockaddr*) &echoServAddr, sizeof(echoServAddr));
		fromSize = sizeof(fromAddr);
		recvfrom(sock, (void*) &op, sizeof(struct operation), 0,
				(struct sockaddr*) &fromAddr, &fromSize);

		//Checking the result of the struct
		if (op.oper == '/') {
			if (op.num1 % op.num2 == 0) {
				printf("Received result from server %s, ip %s: %d %s %d = %d\n",
						name, ip, op.num1, &op.oper, op.num2, op.res1);
			} else {
				printf("Received result from server %s, ip %s: %d %s %d = %f\n",
						name, ip, op.num1, &op.oper, op.num2, op.res2);
			}
		} else {
			printf("Received result from server %s, ip %s: %d %s %d = %d\n",
					name, ip, op.num1, &op.oper, op.num2, op.res1);
		}
	}
	closesocket(sock);
	ClearWinSock();
	system("pause");
	return EXIT_SUCCESS;
}
