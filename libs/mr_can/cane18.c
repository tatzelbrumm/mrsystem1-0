/**********************************************************************\
* Dateiname: cane18.c
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
* Funktionsname: MrCs2EncPing0
*
* Kurzbeschreibung:
* encode function for CAN ping command messages
*
* Parameter:
* CanMsg   : data, which hold the encoded CAN data
*
\**********************************************************************/
void MrCs2EncPing0(MrCs2CanDataType *CanMsg)
{
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_PING);
   MrCs2SetDlc(CanMsg, 0);
}

/**********************************************************************\
* Funktionsname: MrCs2EncPing8
*
* Kurzbeschreibung:
* encode function for CAN ping command messages
*
* Parameter:
* CanMsg    : data, which hold the encoded CAN data
* Uid       : uid of receiver
* SwVersion : sw version for receiver
* DeviceId  : device id for receiver
*
\**********************************************************************/
void MrCs2EncPing8(MrCs2CanDataType *CanMsg, unsigned long Uid,
                   int SwVersion, int DeviceId)
{
   SetLongToByteArray((char *)CanMsg->Data, Uid);
   SetIntToByteArray((char *)&(CanMsg->Data[4]), SwVersion);
   SetIntToByteArray((char *)&(CanMsg->Data[6]), DeviceId);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_PING);
   MrCs2SetDlc(CanMsg, 8);
}
