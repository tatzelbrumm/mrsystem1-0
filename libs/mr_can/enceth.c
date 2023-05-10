/**********************************************************************\
* Dateiname: enceth.c
* Autor    : Michael Bernstein
* Projekt  : mr_can
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: encode UDP frame from CAN message
*
\**********************************************************************/

/*--- #includes der Form <...> ---------------------------------------*/
#include <string.h>
#include <strings.h>
/*--- #includes der Form "..." ---------------------------------------*/
#include "mr_can.h"

/**********************************************************************\
* Funktionsname: MrEthCs2Encode
*
* Kurzbeschreibung:
* encode UDP frame from CAN message
*
* Parameter:
* UdpFrame : place for the encoded UDP frame, should have 13
*            (MR_CS2_UDP_LENGTH) bytes space
* CanMsg   : CAN message which should be endcoded as UDP frame
*
\**********************************************************************/
void MrEthCs2Encode(char *UdpFrame, MrCs2CanDataType *CanMsg)
{  unsigned CanHash;

   CanHash = MrCs2GetHash(CanMsg) | MR_CS2_MASK_HASH_MAGIC;
   MrCs2SetId(CanMsg, (MrCs2GetHash(CanMsg) << MR_CS2_SHIFT_HASH) |
                      (MrCs2GetResponse(CanMsg) << MR_CS2_SHIFT_RESP) |
                      (MrCs2GetCommand(CanMsg) << MR_CS2_SHIFT_COMMAND) |
                      (MrCs2GetPrio(CanMsg) << MR_CS2_SHIFT_PRIO));
   bzero(UdpFrame, MR_CS2_UDP_LENGTH);
   UdpFrame[0] = (MrCs2GetId(CanMsg) >> 24) & 0x000000FF;
   UdpFrame[1] = (MrCs2GetId(CanMsg) >> 16) & 0x000000FF;
   UdpFrame[2] = (MrCs2GetId(CanMsg) >>  8) & 0x000000FF;
   UdpFrame[3] = (MrCs2GetId(CanMsg) >>  0) & 0x000000FF;
   UdpFrame[4] = MrCs2GetDlc(CanMsg);
   memcpy(&UdpFrame[5], &CanMsg->Data, MrCs2GetDlc(CanMsg));
}
