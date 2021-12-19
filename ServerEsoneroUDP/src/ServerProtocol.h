/*
 ============================================================================
 Name        : ServerProtocol.h
 Author      : Antonio Sequenza
 Date        : 19/12/2021
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#ifndef SERVERPROTOCOL_H_
#define SERVERPROTOCOL_H_

#define ECHOMAX 255
#define PORT 45300

struct operation {
	int num1;
	int num2;
	char oper;
	int res1;
	float res2;
	char flag;
};

#endif /* SERVERPROTOCOL_H_ */
