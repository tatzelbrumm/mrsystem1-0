/**********************************************************************\
* Dateiname: deceth.c
* Autor    : Michael Bernstein
* Projekt  : mr_can
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: decode UDP frame to CAN message
*
\**********************************************************************/

/*--- #includes der Form <...> ---------------------------------------*/
#include <string.h>
#include <strings.h>
/*--- #includes der Form "..." ---------------------------------------*/
#include "mr_can.h"

/**********************************************************************\
* Funktionsname: MrEthCs2Decode
*
* Kurzbeschreibung:
* decode UDP frame to CAN message
*
* Parameter:
* CanMsg   : CAN message which holds the decoded UDP frame
* UdpFrame : UDP frame which should be decoded
*
\**********************************************************************/
void MrEthCs2Decode(MrCs2CanDataType *CanMsg, char *UdpFrame)
{  unsigned CanHash, i;

   MrCs2SetId(CanMsg,
              ((unsigned long)UdpFrame[0] << 24) |
              ((unsigned long)UdpFrame[1] << 16) |
              ((unsigned long)UdpFrame[2] <<  8) |
              ((unsigned long)UdpFrame[3] <<  0));
   MrCs2SetDlc(CanMsg, UdpFrame[4]);
   for (i = 0; i < 8; i++)
      CanMsg->Data[i] = UdpFrame[5 + i];
   CanHash = (CanMsg->Id & MR_CS2_MASK_HASH) >> MR_CS2_SHIFT_HASH;
   MrCs2SetIsCs2(CanMsg,
                 (CanHash & MR_CS2_MASK_HASH_MAGIC) == MR_CS2_HASH_MAGIC);
   MrCs2SetHash(CanMsg, CanHash & ~MR_CS2_MASK_HASH_MAGIC);
   MrCs2SetResponse(CanMsg,
                    (CanMsg->Id & MR_CS2_MASK_RESP) >> MR_CS2_SHIFT_RESP);
   MrCs2SetCommand(CanMsg,
                   (CanMsg->Id & MR_CS2_MASK_COMMAND) >> MR_CS2_SHIFT_COMMAND);
   MrCs2SetPrio(CanMsg, (CanMsg->Id & MR_CS2_MASK_PRIO) >> MR_CS2_SHIFT_PRIO);
}
