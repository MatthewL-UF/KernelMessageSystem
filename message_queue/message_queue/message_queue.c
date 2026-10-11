#include "message_queue.h"
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
//Source code here



int send_msg(char* message, char* username){
	char* nmsg;
	//Check for length.
	if(strlen(message) > 1025){
		//Account for Null Terminator
		strncpy(nmsg, message, 1025);
		nmsg[1024] = '\0';
		message = nmsg;
	}


	//Because we have username, use it to get uid_t to pass into syscall
	struct passwd *recipient = getpwnam(username);
	uid_t recId;
	//Check for existence
	if(recipient != NULL){
		recId = recipient->pw_uid;
	} else {
		//Unknown reptilian recipient
		printf("User does not exist\n");
		return -1;
	}
	//Run
	long error = syscall(SEND_MSG_CALL, message, recId);

	return error;

}


int get_msg(char* message, char* username){
	//Get ID to pass
	uid_t curr_User = getuid();

	long error = syscall(GET_MSG_CALL, message, &curr_User);

	// If error is one, then no messages directed to you have been found.
	if(error == 1){
		printf("Inbox empty\n");
		return -error;
	} else if (error != 0){
		return error;
	}

	struct passwd *sender = getpwuid(curr_User);

	// Ensure name corruption does not occur as original passwd structure is deleted.

	// printf("%s\n", sender->pw_name);
	snprintf(username, 33, "%s", sender->pw_name);
	// strncpy(username, sender->pw_name, sizeof(sender->pw_name));
	// printf("%s\n", username);



	return error;
}


int write_top_msg(int fd){
	//Declare room beforehand.
	char msgFinal[1024];
	char copy[33];

	//Use get to retrieve message
	long error = get_msg(msgFinal, copy);
	//Copy now holds the name of the sender, and msgFinal the final Message
	// printf("%s\n", msgFinal);
	if (error != 0){
		return -1;
	}
	//Get the corresponding recipient (the current user)
	struct passwd *recipient = getpwuid(getuid());

	//Move all information into a single string
	char output[1200];
	int len = snprintf(output, sizeof(output), "From: %s\nTo: %s\nText: %s\n", copy, recipient->pw_name, msgFinal);

	//Write said string, using fd as instructed
	write(fd, output, len);

	return 0;
}

