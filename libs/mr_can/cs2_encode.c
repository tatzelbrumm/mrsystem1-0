/**********************************************************************\
* Dateiname: cs2_encode.c
* Autor    : Michael Bernstein
* Projekt  : mr_can
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: encode of ms2 CAN messages
*
\**********************************************************************/

/*--- #includes der Form <...> ---------------------------------------*/
#include <string.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <bitmask.h>
/*--- #includes der Form "..." ---------------------------------------*/
#include "mr_can.h"

/**********************************************************************\
* Funktionsname: MrCs2Encode
*
* Kurzbeschreibung: encodes the CAN id from cs2 fields
*
* Parameter:
* CanMsg   : data, from which the CAN frame should be encoded
* CanFrame : CAN frame which should be encoded
*
\**********************************************************************/
void MrCs2Encode(MrCs2CanDataType *CanMsg, struct can_frame *CanFrame)
{  unsigned CanHash, i;

   CanHash = (MrCs2GetHash(CanMsg) & ~MR_CS2_MASK_HASH_MAGIC) | MR_CS2_HASH_MAGIC;
   MrCs2SetId(CanMsg, (CanHash << MR_CS2_SHIFT_HASH) |
                      (MrCs2GetResponse(CanMsg) << MR_CS2_SHIFT_RESP) |
                      (MrCs2GetCommand(CanMsg) << MR_CS2_SHIFT_COMMAND) |
                      (MrCs2GetPrio(CanMsg) << MR_CS2_SHIFT_PRIO));
   CanFrame->can_id = MrCs2GetId(CanMsg);
   CanFrame->can_id &= CAN_EFF_MASK;
   CanFrame->can_id |= CAN_EFF_FLAG;
   CanFrame->can_dlc = MrCs2GetDlc(CanMsg);
   for (i = 0; i < 8; i++)
      CanFrame->data[i] = CanMsg->Data[i];
}
