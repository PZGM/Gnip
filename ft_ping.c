#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <iostream>

int main() {
	int sockfd;

	sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	    
}
