/**********************************************************************\
* Dateiname: cand1d.c
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
* Funktionsname: MrCs2DecStatus5
*
* Kurzbeschreibung:
* decode function for CAN status command messages
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* Uid    : holds the decoded uid
* Index  : holds the decoded index
*
\**********************************************************************/
void MrCs2DecStatus5(MrCs2CanDataType *CanMsg, unsigned long *Uid, int *Index)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
   *Index = CanMsg->Data[4];
}

/**********************************************************************\
* Funktionsname: MrCs2DecStatus6
*
* Kurzbeschreibung:
* decode function for CAN status command messages
*
* Parameter:
* CanMsg     : data, which hold the encoded CAN data
* Uid        : holds the decoded uid
* Index      : holds the decoded index
* NumPackets : holds the decoded number of packets
*
\**********************************************************************/
void MrCs2DecStatus6(MrCs2CanDataType *CanMsg, unsigned long *Uid, int *Index,
                     int *NumPackets)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
   *Index = CanMsg->Data[4];
   *NumPackets = CanMsg->Data[5];
}
