/**********************************************************************\
* Dateiname: starteth.c
* Autor    : Michael Bernstein
* Projekt  : mr_can
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: open UDP unicast socket for ethernet connection
*
\**********************************************************************/

/*--- #includes der Form <...> ---------------------------------------*/
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
/*--- #includes der Form "..." ---------------------------------------*/
#include "mr_can.h"

/**********************************************************************\
* Funktionsname: MrEthCs2StartServer
*
* Kurzbeschreibung:
* open UDP unicast socket for ethernet connection
* port is MR_CS2_UDP_LOCAL_PORT 15731
*
\**********************************************************************/
int MrEthCs2StartServer(void)
{  int Socket;
   struct sockaddr_in ServAddr;

   /* Create socket for sending/receiving datagrams */
   Socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
   if (Socket >= 0)
   {
      /* Construct local address structure */
      memset(&ServAddr, 0, sizeof(ServAddr));
      ServAddr.sin_family = AF_INET;
      ServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
      ServAddr.sin_port = htons(MR_CS2_UDP_LOCAL_PORT);
      /* Bind to the local address */
      if (bind(Socket, (struct sockaddr *) &ServAddr, sizeof(ServAddr)) < 0)
      {
         close(Socket);
         Socket = -1;
      }
   }
   return(Socket);
}
