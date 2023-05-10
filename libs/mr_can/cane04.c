/**********************************************************************\
* Dateiname: cane04.c
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
* Funktionsname: MrCs2EncSpeed4
*
* Kurzbeschreibung:
* encode function for CAN loc speed command messages
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* LocId  : loc id of receiver
*
\**********************************************************************/
void MrCs2EncSpeed4(MrCs2CanDataType *CanMsg, unsigned long LocId)
{
   SetLongToByteArray((char *)CanMsg->Data, LocId);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_SPEED);
   MrCs2SetDlc(CanMsg, 4);
}

/**********************************************************************\
* Funktionsname: MrCs2EncSpeed6
*
* Kurzbeschreibung:
* encode function for CAN loc speed command messages
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* LocId  : loc id of receiver
* Speed  : speed of receiver
*
\**********************************************************************/
void MrCs2EncSpeed6(MrCs2CanDataType *CanMsg, unsigned long LocId, int Speed)
{
   SetLongToByteArray((char *)CanMsg->Data, LocId);
   SetIntToByteArray((char *)&(CanMsg->Data[4]), Speed);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_SPEED);
   MrCs2SetDlc(CanMsg, 6);
}
