/*
 * SigHandler.h
 *
 *  Created on: Nov 19, 2017
 *      Author: ntt3013
 */
#ifndef SIGHANDLER_H_
#define SIGHANDLER_H_

//	some error codes
//		use these to make
//		the code more readable
#define	RETURN_SUCCESS		0
#define RETURN_FAILURE		-1
//method for signal handling
void myHandler(int signal);
void setUpHandler();

extern int Exit;//Exit (global) is set by the handler


#endif /* SIGHANDLER_H_ */
