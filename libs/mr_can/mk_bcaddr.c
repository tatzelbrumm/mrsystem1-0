/**********************************************************************\
* Dateiname: mk_bcaddr.c
* Autor    : Michael Bernstein
* Projekt  : mr_can
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: build a broadcast deestination addr
*
\**********************************************************************/

/*--- #includes der Form <...> ---------------------------------------*/
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
/*--- #includes der Form "..." ---------------------------------------*/
#include "mr_can.h"

/**********************************************************************\
* Funktionsname: MrEthCs2MkBcAddr
*
* Kurzbeschreibung:
* build broadcasr addr
* port is MR_CS2_UDP_LOCAL_PORT 15731
*
* Parameter:
* baddr : broadcast addr
*
\**********************************************************************/
void MrEthCs2MkBcAddr(struct sockaddr_in *baddr, char *IpAddr)
{
   bzero(baddr, sizeof(*baddr));
   baddr->sin_family = AF_INET;
   baddr->sin_port = htons(MR_CS2_UDP_REMOTE_PORT);
   inet_pton(AF_INET, IpAddr, &baddr->sin_addr);
}
