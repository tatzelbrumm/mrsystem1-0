/**********************************************************************\
* Dateiname: cand10.c
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
* Funktionsname: MrCs2DecS88Poll5
*
* Kurzbeschreibung:
* decode function for CAN S88 poll command messages
*
* Parameter:
* CanMsg    : data, which hold the encoded CAN data
* Uid       : holds the decoded uid
* NumModuls : holds the decoded number of modules
*
\**********************************************************************/
void MrCs2DecS88Poll5(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                      int *NumModuls)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
   *NumModuls = CanMsg->Data[4];
}

/**********************************************************************\
* Funktionsname: MrCs2DecS88Poll7
*
* Kurzbeschreibung:
* decode function for CAN S88 poll command messages
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* Uid    : holds the decoded uid
* Modul  : holds the decoded module
* State  : holds the decoded state
*
\**********************************************************************/
void MrCs2DecS88Poll7(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                      int *Modul, int *State)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
   *Modul = CanMsg->Data[4];
   *State = GetIntFromByteArray((char *)&(CanMsg->Data[5]));
}
