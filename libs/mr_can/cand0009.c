/**********************************************************************\
* Dateiname: cand0009.c
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
* Funktionsname: MrCs2DecSysResetMfxCounter
*
* Kurzbeschreibung:
* decode function for CAN system command reset mfx counter message
*
* Parameter:
* CanMsg  : data, which hold the encoded CAN data
* Uid     : holds the decoded uid
* Counter : holds the decoded counter
*
\**********************************************************************/
void MrCs2DecSysResetMfxCounter(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                                int *Counter)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
   *Counter = GetIntFromByteArray((char *)&(CanMsg->Data[5]));
}
