/**********************************************************************\
* Dateiname: cane0009.c
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
* Funktionsname: MrCs2EncSysResetMfxCounter
*
* Kurzbeschreibung:
* encode function for CAN system command reset mfx counter message
*
* Parameter:
* CanMsg  : data, which hold the encoded CAN data
* Uid     : Uid of receiver
' Counter :counter
*
\**********************************************************************/
void MrCs2EncSysResetMfxCounter(MrCs2CanDataType *CanMsg, unsigned long Uid,
                                int Counter)
{
   SetLongToByteArray((char *)CanMsg->Data, Uid);
   SetIntToByteArray((char *)&(CanMsg->Data[5]), Counter);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_SYSTEM);
   MrCs2SetSystemSubcmd(CanMsg, MR_CS2_SUBCMD_SYSTEM_RESET_MFX_COUNTER);
   MrCs2SetDlc(CanMsg, 6);
}
