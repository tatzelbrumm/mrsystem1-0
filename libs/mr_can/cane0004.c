/**********************************************************************\
* Dateiname: cane0004.c
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
* Funktionsname: MrCs2EncSysCycleEnd
*
* Kurzbeschreibung:
* encode function for CAN system command cycle end message
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* Uid    : Uid of receiver
*
\**********************************************************************/
void MrCs2EncSysCycleEnd(MrCs2CanDataType *CanMsg, unsigned long Uid)
{
   SetLongToByteArray((char *)CanMsg->Data, Uid);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_SYSTEM);
   MrCs2SetSystemSubcmd(CanMsg, MR_CS2_SUBCMD_SYSTEM_CYCLE_END);
   MrCs2SetDlc(CanMsg, 4);
}
