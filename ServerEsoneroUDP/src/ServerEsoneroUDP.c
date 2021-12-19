/*
 ============================================================================
 Name        : ServerEsoneroUDP.c
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
#include "ServerProtocol.h"

int add(struct operation op) {
	return op.num1 + op.num2;
}

int mult(struct operation op) {
	return op.num1 * op.num2;
}

int sub(struct operation op) {
	return op.num1 - op.num2;
}

int divisionInt(struct operation op) {
	return op.num1 / op.num2;
}

float divisionFloat(struct operation op) {
	return (float) op.num1 / (float) op.num2;
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
	struct sockaddr_in echoClntAddr;
	struct operation op;
	struct hostent *host;
	const char *ip = "127.0.0.1";
	int cliAddrLen;

	//Creation of the socket
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		ErrorHandler("socket() failed");

	memset(&echoServAddr, 0, sizeof(echoServAddr));
	echoServAddr.sin_family = AF_INET;
	echoServAddr.sin_port = htons(PORT);
	echoServAddr.sin_addr.s_addr = inet_addr(ip);

	if ((bind(sock, (struct sockaddr*) &echoServAddr, sizeof(echoServAddr)))
			< 0)
		ErrorHandler("bind() failed");

	struct in_addr addr;
	addr.s_addr = inet_addr(ip);
	host = gethostbyaddr((char*) &addr, 4, AF_INET);
	char *canonical_name = host->h_name;

	//Receiving and sending messages
	printf("Server ready\n");
	while (1) {
		cliAddrLen = sizeof(echoClntAddr);
		recvfrom(sock, (void*) &op, sizeof(struct operation), 0,
				(struct sockaddr*) &echoClntAddr, &cliAddrLen);

		printf("Requested operation: '%s %d %d' from client %s, ip %s\n",
				&op.oper, op.num1, op.num2, canonical_name, ip);

		switch (op.oper) {
		case '+':
			op.res1 = add(op);
			break;
		case 'x':
			op.res1 = mult(op);
			break;
		case '-':
			op.res1 = sub(op);
			break;
		case '/':
			if (op.num1 % op.num2 == 0) {
				op.res1 = divisionInt(op);
			} else {
				op.res2 = divisionFloat(op);
			}
			break;
		}

		sendto(sock, (void*) &op, sizeof(struct operation), 0,
				(struct sockaddr*) &echoClntAddr, sizeof(echoClntAddr));
	}
}

