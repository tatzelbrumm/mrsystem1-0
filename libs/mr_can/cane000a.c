/**********************************************************************\
* Dateiname: cane000a.c
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
* Funktionsname: MrCs2EncSysOverload
*
* Kurzbeschreibung:
* encode function for CAN system command overload message
*
* Parameter:
* CanMsg  : data, which hold the encoded CAN data
* Uid     : Uid of receiver
* Channel : channel
*
\**********************************************************************/
void MrCs2EncSysOverload(MrCs2CanDataType *CanMsg, unsigned long Uid,
                         int Channel)
{
   SetLongToByteArray((char *)CanMsg->Data, Uid);
   CanMsg->Data[5] = Channel;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_SYSTEM);
   MrCs2SetSystemSubcmd(CanMsg, MR_CS2_SUBCMD_SYSTEM_OVERLOAD);
   MrCs2SetDlc(CanMsg, 5);
}
