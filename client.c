#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
 
int main(int argc, char * argv[]) {
  int sockfd = 0, n = 0;
  char recvBuff[1024];
  char* hi = "Hi!";
  char* bye = "Bye!";
  char* hostname;

  if (argc == 3) {
    hostname = argv[2];
    printf("Grabbed hostname from command-line argument.\n")
  }

  // Info that the socket will be bound to
  struct sockaddr_in serv_addr;

  struct hostent* H;
  struct in_addr** AddrList;
  if ((H = gethostbyname(hostname)) == NULL) {
    printf("Error calling gethostbyname(), exiting...\n");
    return 1;
  }

  AddrList = (struct in_addr **) H->h_addr_list;

  // Set receive buffer to all 0s
  memset(recvBuff, '0', sizeof(recvBuff));

  // Set sockfd as our socket
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("\n Error : Could not create socket \n");
    return 1;
  }

  // Set port etc. for the socket
  serv_addr.sin_family = AF_INET; // Address family
  serv_addr.sin_port = htons(5000); // Port number
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP address

  // Try to connect to socket
  if (connect(sockfd, (struct sockaddr * ) & serv_addr, sizeof(serv_addr)) < 0) {
    printf("\n Error : Connect Failed \n");
    return 1;
  } else {
    printf("Socket connected successfully.\n");
  }

  if (argc >= 2 && *argv[1] == '1') {
    send(sockfd, hi, strlen(hi), 0);
    printf("\"Hi!\" was sent to the server.\n");
  } 
  else if (argc >= 2 && *argv[1] == '2') {
    send(sockfd, bye, strlen(bye), 0);
    printf("\"Bye!\" was sent to the server.\n");
  }
  else {
    printf("No message was sent to the server.\n");
  }

  // Read server's message
  while ((n = read(sockfd, recvBuff, sizeof(recvBuff) - 1)) > 0) {
    // Set last byte to 0
    recvBuff[n] = 0;
    // Output the contents of the recvBuff
    printf("Server's message: ");
    if (fputs(recvBuff, stdout) == EOF) {
      printf("\n Error : Fputs error");
    }
    printf("\n");
  }

  if (n < 0) {
    printf("\n Read Error \n");
  }

  return 0;
}