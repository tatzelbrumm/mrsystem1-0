/**********************************************************************\
* Dateiname: cand0b.c
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
* Funktionsname: MrCs2DecAccSwitch6
*
* Kurzbeschreibung:
* decode function for CAN acc switch command messages
*
* Parameter:
* CanMsg   : data, which hold the encoded CAN data
* LocId    : holds the decoded loc id
* Position : holds the decoded position
* Current  : holds the decoded current
*
\**********************************************************************/
void MrCs2DecAccSwitch6(MrCs2CanDataType *CanMsg, unsigned long *LocId,
                        int *Position, int *Current)
{
   *LocId = GetLongFromByteArray((char *)CanMsg->Data);
   *Position = CanMsg->Data[4];
   *Current = CanMsg->Data[5];
}

/**********************************************************************\
* Funktionsname: MrCs2DecAccSwitch8
*
* Kurzbeschreibung:
* decode function for CAN acc switch command messages
*
* Parameter:
* CanMsg   : data, which hold the encoded CAN data
* LocId    : holds the decoded loc id
* Position : holds the decoded position
* Current  : holds the decoded current
* TimeFkt  : holds the decoded time function
*
\**********************************************************************/
void MrCs2DecAccSwitch8(MrCs2CanDataType *CanMsg, unsigned long *LocId,
                        int *Position, int *Current, int *TimeFkt)
{
   *LocId = GetLongFromByteArray((char *)CanMsg->Data);
   *Position = CanMsg->Data[4];
   *Current = CanMsg->Data[5];
   *TimeFkt = GetIntFromByteArray((char *)&(CanMsg->Data[6]));
}
