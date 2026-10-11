libmessage_queue: message_queue.o
	ar cr libmessage_queue.a message_queue.o

message_queue: message_queue.h, message_queue.c
	gcc -fPIC -c message_queue.c -o message_queue.o
