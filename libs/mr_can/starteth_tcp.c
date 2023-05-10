/**********************************************************************\
* Dateiname: starteth_tcp.c
* Autor    : Michael Bernstein
* Projekt  : mr_can
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: open TCP socket for ethernet connection to app
*
\**********************************************************************/

/*--- #includes der Form <...> ---------------------------------------*/
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
/*--- #includes der Form "..." ---------------------------------------*/
#include "mr_can.h"

#define MAXPENDING 5

/**********************************************************************\
* Funktionsname: MrEthCs2StartAppServer
*
* Kurzbeschreibung:
* open TCP socket for ethernet connection to App
* port is MR_CS2_UDP_LOCAL_PORT 15731
x*
\**********************************************************************/
int MrEthCs2StartAppServer(void)
{  int Socket;
   struct sockaddr_in ServAddr;

   Socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
   if (Socket >= 0)
   {
      /* Construct local address structure */
      memset(&ServAddr, 0, sizeof(ServAddr));
      ServAddr.sin_family = AF_INET;
      ServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
      ServAddr.sin_port = htons(MR_CS2_UDP_LOCAL_PORT);
      /* Bind to the local address */
      if (bind(Socket, (struct sockaddr *)&ServAddr, sizeof(ServAddr)) < 0)
      {
         close(Socket);
         Socket = -1;
      }
      else
      {
         /* Mark the socket so it will listen for incoming connections */
         if (listen(Socket, MAXPENDING) < 0)
         {
            close(Socket);
            Socket = -1;
         }
      }
   }
   return(Socket);
}
