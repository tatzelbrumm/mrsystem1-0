/**********************************************************************\
* Dateiname: starteth_bc.c
* Autor    : Michael Bernstein
* Projekt  : mr_can
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: open UDP broadcast socket for ethernet connection
*
\**********************************************************************/

/*--- #includes der Form <...> ---------------------------------------*/
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
/*--- #includes der Form "..." ---------------------------------------*/
#include "mr_can.h"

/**********************************************************************\
* Funktionsname: MrEthCs2StartBcServer
*
* Kurzbeschreibung:
* open UDP broadcast socket for ethernet connection
* port is MR_CS2_UDP_LOCAL_PORT 15731
*
\**********************************************************************/
int MrEthCs2StartBcServer(void)
{  int Socket, on;

   /* Create socket for sending/receiving datagrams */
   Socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
   if (Socket >= 0)
   {
      on = 1;
      if (setsockopt(Socket, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0)
      {
         close(Socket);
         Socket = -1;
      }
   }
   else
   {
      close(Socket);
      Socket = -1;
   }
   return(Socket);
}
