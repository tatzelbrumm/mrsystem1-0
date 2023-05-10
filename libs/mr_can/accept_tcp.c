/**********************************************************************\
* Dateiname: accept_tcp.c
* Autor    : Michael Bernstein
* Projekt  : mr_can
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: accept TCP connection from app
*
\**********************************************************************/

/*--- #includes der Form <...> ---------------------------------------*/
#include <stdio.h>      /* for printf() */
#include <sys/socket.h> /* for accept() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
/*--- #includes der Form "..." ---------------------------------------*/
#include "mr_can.h"

/**********************************************************************\
* Funktionsname: MrEthCs2Accept
*
* Kurzbeschreibung:
* accept TCP conenction from app
*
\**********************************************************************/
int MrEthCs2Accept(int ServSock)
{  int ClntSock;
   struct sockaddr_in ClntAddr;
   unsigned int ClntLen;

   /* Set the size of the in-out parameter */
   ClntLen = sizeof(ClntAddr);
   /* Wait for a client to connect */
   ClntSock = accept(ServSock, (struct sockaddr *)&ClntAddr, &ClntLen);
   return(ClntSock);
}
