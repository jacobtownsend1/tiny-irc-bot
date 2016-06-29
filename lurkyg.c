/*
This program is a tiny IRC bot originallymeant to be a girlfriend for Lurky in the TIN IRC.
Inspired by another tiny IRC bot I saw.

Author: r00tm3
6-27-16
hf-tin.ddns.net
*/
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdarg.h>
char sbuf[512];
int sockfd;

//create function for sending information.
void raw(char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(sbuf, 512, fmt, ap);
	va_end(ap);
	printf("<< %s", sbuf);
	write(sockfd, sbuf, strlen(sbuf));
}

int main(int  argc, char *argv[]) {
	int n;
	char recvline[513], out[513];
	char *host = "ip";
	char *chan = "#channel";	
	char *port = "6667";
	char *nick = "nick";
	struct addrinfo addr, *res;

	//create and connect to the socket
   	memset(&addr, 0, sizeof(addr));
   	addr.ai_family = AF_INET;
   	addr.ai_socktype = SOCK_STREAM;
   	getaddrinfo(host, port, &addr, &res);
   	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	connect(sockfd, res->ai_addr, res->ai_addrlen);
	//set nick and user, join IRC channel
	raw("USER %s 0 0 :%s\r\n", nick, nick);
        raw("NICK %s\r\n", nick);
	raw("JOIN %s\r\n", chan);
	raw("PRIVMSG %s :Hi, I'm %s!\r\n",chan, nick);
	
	//recieve information and check if bot has been given a command. respond to PING
	while(1) {
		recvline[0] = 0;
		n = read(sockfd, recvline, sizeof(sbuf));		
		
		if (n > 0) {
			recvline[n] = 0;
			if(strstr(recvline, "PING") != NULL) {
				recvline[1] = 'O';
				raw(recvline);
			} else if (strstr(recvline, ":!tinybot\r\n")) {
				raw("PRIVMSG %s :Yes?\r\n", chan);
			} 
		} 
	}
}
