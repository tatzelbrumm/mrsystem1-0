/**********************************************************************\
* Dateiname: cane0006.c
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
* Funktionsname: MrCs2EncSysDecoderTimeing
*
* Kurzbeschreibung:
* encode function for CAN system command encoder timing message
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* Uid    : Uid of receiver
* Time   : new timeing
*
\**********************************************************************/
void MrCs2EncSysDecoderTimeing(MrCs2CanDataType *CanMsg, unsigned long Uid,
                               int Time)
{
   SetLongToByteArray((char *)CanMsg->Data, Uid);
   SetIntToByteArray((char *)&(CanMsg->Data[5]), Time);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_SYSTEM);
   MrCs2SetSystemSubcmd(CanMsg, MR_CS2_SUBCMD_SYSTEM_DECODER_TIMEING);
   MrCs2SetDlc(CanMsg, 6);
}
