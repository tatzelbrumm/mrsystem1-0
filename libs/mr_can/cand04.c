/**********************************************************************\
* Dateiname: cand04.c
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
* Funktionsname: MrCs2DecSpeed4
*
* Kurzbeschreibung:
* decode function for CAN loc speed command messages
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* LocId  : holds the decoded loc id
*
\**********************************************************************/
void MrCs2DecSpeed4(MrCs2CanDataType *CanMsg, unsigned long *LocId)
{
   *LocId = GetLongFromByteArray((char *)CanMsg->Data);
}

/**********************************************************************\
* Funktionsname: MrCs2DecSpeed6
*
* Kurzbeschreibung:
* decode function for CAN loc speed command messages
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* LocId  : holds the decoded loc id
* Speed  : holds the decoded speed
*
\**********************************************************************/
void MrCs2DecSpeed6(MrCs2CanDataType *CanMsg, unsigned long *LocId, int *Speed)
{
   *LocId = GetLongFromByteArray((char *)CanMsg->Data);
   *Speed = GetIntFromByteArray((char *)&(CanMsg->Data[4]));
}
