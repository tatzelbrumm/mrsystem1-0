/**********************************************************************\
* Dateiname: cand0008.c
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
* Funktionsname: MrCs2DecSysTrackProtocoll
*
* Kurzbeschreibung:
* decode function for CAN system command track protocoll message
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* Uid    : holds the decoded uid
* Proto  : holds the decoded protocoll
*
\**********************************************************************/
void MrCs2DecSysTrackProtocoll(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                               int *Proto)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
   *Proto = CanMsg->Data[5];
}
