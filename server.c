#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
 
int main(void) {
  int listenfd = 0, connfd = 0, clientMsg = 0;

  // Info that the socket will be bound to
  struct sockaddr_in serv_addr;

  char sendBuff[1024];
  char recvBuff[1024];
  int numrv;

  // Set listenfd as our socket
  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  printf("socket retrieve success\n");

  // Clear the socket info and send buffer so we don't send null
  memset( & serv_addr, '0', sizeof(serv_addr));
  memset(sendBuff, '0', sizeof(sendBuff));

  // Set port etc. for the socket
  serv_addr.sin_family = AF_INET; // Address family
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // IP address
  serv_addr.sin_port = htons(5000); // Port number

  // Bind the server's socket to the info in serv_addr
  bind(listenfd, (struct sockaddr * ) & serv_addr, sizeof(serv_addr));

  // Start listening to the socket
  if (listen(listenfd, 10) == -1) {
    printf("Failed to listen\n");
    return -1;
  }

  while (1) {
    // Accept any requests from the client, creating new socket connfd
    connfd = accept(listenfd, (struct sockaddr * ) NULL, NULL);

    // Read incoming message from client
    clientMsg = read(connfd, recvBuff, sizeof(recvBuff) - 1);

    printf("Size of client msg: %d\n", clientMsg);
    printf("%s\n", recvBuff);

    // Copy message to sendBuff
    strcpy(sendBuff, "Message from server33");

    // Send message in sendBuff over socket connfd to client
    write(connfd, sendBuff, strlen(sendBuff));

    // Close the connection
    close(connfd);
    sleep(1);
  }

  return 0;
}