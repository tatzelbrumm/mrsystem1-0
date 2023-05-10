/**********************************************************************\
* Dateiname: cane05.c
* Autor    : Michael Bernstein
* Projekt  : mr_can
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: encode of CS2/MS2 CAN commands
*
\**********************************************************************/

/*--- #includes der Form <...> ---------------------------------------*/
#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
/*--- #includes der Form "..." ---------------------------------------*/
#include "mr_can.h"

/**********************************************************************\
* Funktionsname: MrCs2EncDirection4
*
* Kurzbeschreibung:
* encode function for CAN loc direction command messages
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* LocId  : loc id of receiver
*
\**********************************************************************/
void MrCs2EncDirection4(MrCs2CanDataType *CanMsg, unsigned long LocId)
{
   SetLongToByteArray((char *)CanMsg->Data, LocId);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_DIRECTION);
   MrCs2SetDlc(CanMsg, 4);
}

/**********************************************************************\
* Funktionsname: MrCs2EncDirection5
*
* Kurzbeschreibung:
* encode function for CAN loc direction command messages
*
* Parameter:
* CanMsg    : data, which hold the encoded CAN data
* LocId     : loc id of receiver
* Direction : direction of reciever
*
\**********************************************************************/
void MrCs2EncDirection5(MrCs2CanDataType *CanMsg, unsigned long LocId,
                        int Direction)
{
   SetLongToByteArray((char *)CanMsg->Data, LocId);
   CanMsg->Data[4] = Direction;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_DIRECTION);
   MrCs2SetDlc(CanMsg, 5);
}
