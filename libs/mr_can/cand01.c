/**********************************************************************\
* Dateiname: cand01.c
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
* Funktionsname: MrCs2DecDiscovery0
*
* Kurzbeschreibung:
* decode function for CAN discovery command messages
*
* Parameter:
* CanMsg   : data, which hold the encoded CAN data
*
\**********************************************************************/
void MrCs2DecDiscovery0(MrCs2CanDataType *CanMsg)
{
}

/**********************************************************************\
* Funktionsname: MrCs2DecDiscovery1
*
* Kurzbeschreibung:
* decode function for CAN discovery command messages
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* Proto  : holds the decoded proto
*
\**********************************************************************/
void MrCs2DecDiscovery1(MrCs2CanDataType *CanMsg, int *Proto)
{
   *Proto = CanMsg->Data[0];
}

/**********************************************************************\
* Funktionsname: MrCs2DecDiscovery5
*
* Kurzbeschreibung:
* decode function for CAN discovery command messages
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* Uid    : holds the decoded uid
* Range  : holds the decoded range
*
\**********************************************************************/
void MrCs2DecDiscovery5(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                        int *Range)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
   *Range = CanMsg->Data[4];
}

/**********************************************************************\
* Funktionsname: MrCs2DecDiscovery6
*
* Kurzbeschreibung:
* decode function for CAN discovery command messages
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* Uid    : holds the decoded uid
* Range  : holds the decoded range
* Ask    : holds the decoded ask
*
\**********************************************************************/
void MrCs2DecDiscovery6(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                        int *Range, int *Ask)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
   *Range = CanMsg->Data[4];
   *Ask = CanMsg->Data[5];
}
