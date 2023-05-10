#include <stddef.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "mr_ipc.h"

#define MAXPENDING 4

int MrIpcStartServer(char *IpAddr, int Port)
{  int sock;
   struct sockaddr_in ServerAddr;

   /* Create socket for incoming connections */
   sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
   if (sock >= 0)
   {
      /* Construct local address structure */
      memset(&ServerAddr, 0, sizeof(ServerAddr));
      ServerAddr.sin_family = AF_INET;
      if (IpAddr != NULL)
         ServerAddr.sin_addr.s_addr = inet_addr(IpAddr);
      else
         ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
      ServerAddr.sin_port = htons(Port);
      /* Bind to the local address */
      if (bind(sock, (struct sockaddr *)&ServerAddr, sizeof(ServerAddr)) < 0)
      {
         close(sock);
	 sock = -1;
      }
      else
      {
         /* Mark the socket so it will listen for incoming connections */
         if (listen(sock, MAXPENDING) < 0)
	 {
            close(sock);
	    sock = -1;
	 }
      }
   }
   return(sock);
}
