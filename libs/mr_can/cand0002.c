/**********************************************************************\
* Dateiname: cand0002.c
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
* Funktionsname: MrCs2DecSysHalt
*
* Kurzbeschreibung:
* decode function for CAN system command halt message
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* Uid    : holds the decoded uid
*
\**********************************************************************/
void MrCs2DecSysHalt(MrCs2CanDataType *CanMsg, unsigned long *Uid)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
}
