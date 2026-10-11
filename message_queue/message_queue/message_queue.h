#define SEND_MSG_CALL 435
#define GET_MSG_CALL 436

int send_msg (char* msg, char* username);
int get_msg (char* msg, char* username);
int write_top_msg(int fd);
