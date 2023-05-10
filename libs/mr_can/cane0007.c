/**********************************************************************\
* Dateiname: cane0007.c
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
* Funktionsname: MrCs2EncSysMfxFastRead
*
* Kurzbeschreibung:
* encode function for CAN system command mfx fast read message
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* Uid    : Uid of receiver
* MfxSid : mfx sid
*
\**********************************************************************/
void MrCs2EncSysMfxFastRead(MrCs2CanDataType *CanMsg, unsigned long Uid,
                            int MfxSid)
{
   SetLongToByteArray((char *)CanMsg->Data, Uid);
   SetIntToByteArray((char *)&(CanMsg->Data[5]), MfxSid);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_SYSTEM);
   MrCs2SetSystemSubcmd(CanMsg, MR_CS2_SUBCMD_SYSTEM_MFX_FAST_READ);
   MrCs2SetDlc(CanMsg, 6);
}
