/*
 ============================================================================
 Name        : ClientProtocol.h
 Author      : Antonio Sequenza
 Date        : 19/12/2021
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#ifndef CLIENTPROTOCOL_H_
#define CLIENTPROTOCOL_H_

#define ECHOMAX 255
#define PORT 45300

struct operation {
	int num1;
	int num2;
	char oper;
	int res1;
	float res2;
};

#endif /* CLIENTPROTOCOL_H_ */
