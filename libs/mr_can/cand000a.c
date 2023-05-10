/**********************************************************************\
* Dateiname: cand000a.c
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
* Funktionsname: MrCs2DecSysOverload
*
* Kurzbeschreibung:
* decode function for CAN system command overload message
*
* Parameter:
* CanMsg   : data, which hold the encoded CAN data
* Uid      : holds the decoded uid
* Channnel : holds the decoded channel
*
\**********************************************************************/
void MrCs2DecSysOverload(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                         int *Channel)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
   *Channel = CanMsg->Data[5];
}
