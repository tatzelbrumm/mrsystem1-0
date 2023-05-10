/**********************************************************************\
* Dateiname: cand0006.c
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
* Funktionsname: MrCs2DecSysDecoderTimeing
*
* Kurzbeschreibung:
* decode function for CAN system command decoder timing message
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* Uid    : holds the decoded uid
* Time   : holds the decoded timing
*
\**********************************************************************/
void MrCs2DecSysDecoderTimeing(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                               int *Time)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
   *Time = GetIntFromByteArray((char *)&(CanMsg->Data[5]));
}
