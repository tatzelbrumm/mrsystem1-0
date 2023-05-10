/**********************************************************************\
* Dateiname: cane0080.c
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
* Funktionsname: MrCs2EncSysSystemReset
*
* Kurzbeschreibung:
* encode function for CAN system command reset message
*
* Parameter:
* CanMsg      : data, which hold the encoded CAN data
* Uid         : Uid of receiver
* ResetTarget : target to reset
*
\**********************************************************************/
void MrCs2EncSysSystemReset(MrCs2CanDataType *CanMsg, unsigned long Uid,
                            int ResetTarget)
{
   SetLongToByteArray((char *)CanMsg->Data, Uid);
   CanMsg->Data[5] = ResetTarget;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_SYSTEM);
   MrCs2SetSystemSubcmd(CanMsg, MR_CS2_SUBCMD_SYSTEM_SYSTEM_RESET);
   MrCs2SetDlc(CanMsg, 5);
}
