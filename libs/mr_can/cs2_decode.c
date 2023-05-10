/**********************************************************************\
* Dateiname: cs2_decode.c
* Autor    : Michael Bernstein
* Projekt  : mr_can
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: decode of ms2 CAN messages
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
* Funktionsname: Ms2Decode
*
* Kurzbeschreibung:
*
* Parameter:
* CanMsg   : data, which should hold then decoded CAN data
* CanFrame : CAN frame which should be decoded
*
\**********************************************************************/
void MrCs2Decode(MrCs2CanDataType *CanMsg, struct can_frame *CanFrame)
{  unsigned CanHash, i;

   MrCs2SetId(CanMsg, CanFrame->can_id);
   MrCs2SetDlc(CanMsg, CanFrame->can_dlc);
   for (i = 0; i < 8; i++)
      CanMsg->Data[i] = CanFrame->data[i];
   CanHash = (CanMsg->Id & MR_CS2_MASK_HASH) >> MR_CS2_SHIFT_HASH;
   MrCs2SetHash(CanMsg, CanHash & ~MR_CS2_MASK_HASH_MAGIC);
   MrCs2SetResponse(CanMsg,
                    (CanMsg->Id & MR_CS2_MASK_RESP) >> MR_CS2_SHIFT_RESP);
   MrCs2SetCommand(CanMsg,
                   (CanMsg->Id & MR_CS2_MASK_COMMAND) >> MR_CS2_SHIFT_COMMAND);
   MrCs2SetPrio(CanMsg, (CanMsg->Id & MR_CS2_MASK_PRIO) >> MR_CS2_SHIFT_PRIO);
   MrCs2SetIsCs2(CanMsg,
                 ((CanHash & MR_CS2_MASK_HASH_MAGIC) == MR_CS2_HASH_MAGIC) ||
                 (CanHash == 0 && MrCs2GetCommand(CanMsg) == MR_CS2_CMD_CFGDAT_STREAM));
}
