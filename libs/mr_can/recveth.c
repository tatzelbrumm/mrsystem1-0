/**********************************************************************\
* Dateiname: sendeth.c
* Autor    : Michael Bernstein
* Projekt  : mr_can
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: receive UDP broadcast
*
\**********************************************************************/

/*--- #includes der Form <...> ---------------------------------------*/
#include <sys/socket.h>
#include <arpa/inet.h>
/*--- #includes der Form "..." ---------------------------------------*/
#include "mr_can.h"

/**********************************************************************\
* Funktionsname: MrEthCs2Recv
*
* Kurzbeschreibung:
* receive UDP data
*
* Parameter:
* Socket    : local socket for receive
* ClntAddr  : remote addr
* Data      : CAN UDP frame
*
\**********************************************************************/
BOOL MrEthCs2Recv(int Socket, struct sockaddr_in *ClntAddr, char *Data)
{  unsigned int ClntAddrLen;
   int RecvMsgSize;

   /* Set the size of the in-out parameter */
   ClntAddrLen = sizeof(*ClntAddr);
   RecvMsgSize = recvfrom(Socket, Data, MR_CS2_UDP_LENGTH, 0,
                          (struct sockaddr *)ClntAddr, &ClntAddrLen);
   return(RecvMsgSize > 0);
}
