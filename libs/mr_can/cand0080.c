/**********************************************************************\
* Dateiname: cand0080.c
* Autor    : Michael Bernstein
* Projekt  : mr_can
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: decode of CS2/MS2 CAN commands
*
\**********************************************************************/

/*--- #includes der Form <...> ---------------------------------------*/
#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
/*--- #includes der Form "..." ---------------------------------------*/
#include "mr_can.h"

/**********************************************************************\
* Funktionsname: MrCs2DecSysSystemReset
*
* Kurzbeschreibung:
* decode function for CAN system command system reset message
*
* Parameter:
* CanMsg      : data, which hold the encoded CAN data
* Uid         : holds the decoded uid
* ResetTarget : holds the decoded reset target
*
\**********************************************************************/
void MrCs2DecSysSystemReset(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                            int *ResetTarget)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
   *ResetTarget = CanMsg->Data[5];
}
