/**********************************************************************\
* Dateiname: cane0001.c
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
* Funktionsname: MrCs2EncSysGo
*
* Kurzbeschreibung:
* encode function for CAN system command go message
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* Uid    : Uid of receiver
*
\**********************************************************************/
void MrCs2EncSysGo(MrCs2CanDataType *CanMsg, unsigned long Uid)
{
   SetLongToByteArray((char *)CanMsg->Data, Uid);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_SYSTEM);
   MrCs2SetSystemSubcmd(CanMsg, MR_CS2_SUBCMD_SYSTEM_GO);
   MrCs2SetDlc(CanMsg, 4);
}
