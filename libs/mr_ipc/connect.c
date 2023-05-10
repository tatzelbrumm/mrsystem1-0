#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "mr_ipc.h"

int MrIpcConnect(char *IpAddr, int Port)
{  int sock;
   struct sockaddr_in ServerAddr;

   /* Create a reliable, stream socket using TCP */
   sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
   if (sock < 0)
   {
      return(-1);
   }
   else
   {
      /* Construct the server address structure */
      memset(&ServerAddr, 0, sizeof(ServerAddr));     /* Zero out structure */
      ServerAddr.sin_family = AF_INET;                /* Internet address family */
      ServerAddr.sin_addr.s_addr = inet_addr(IpAddr); /* Server IP address */
      ServerAddr.sin_port = htons(Port);              /* Server port */
      /* Establish the connection to the echo server */
      if (connect(sock, (struct sockaddr *)&ServerAddr, sizeof(ServerAddr)) < 0)
      {
         close(sock);
	 return(-2);
      }
      else
      {
         return(sock);
      }
   }
}
