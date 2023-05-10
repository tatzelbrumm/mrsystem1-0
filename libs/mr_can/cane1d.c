/**********************************************************************\
* Dateiname: cane1d.c
* Autor    : Michael Bernstein
* Projekt  : mr_can
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: encode of CS2/MS2 CAN commands
*
\**********************************************************************/

/*--- #includes der Form <...> ---------------------------------------*/
#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
/*--- #includes der Form "..." ---------------------------------------*/
#include "mr_can.h"

/**********************************************************************\
* Funktionsname: MrCs2EncStatus5
*
* Kurzbeschreibung:
* encode function for CAN status command messages
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* Uid    : uid of receiver
* Index  : index for receiver
*
\**********************************************************************/
void MrCs2EncStatus5(MrCs2CanDataType *CanMsg, unsigned long Uid, int Index)
{
   SetLongToByteArray((char *)CanMsg->Data, Uid);
   CanMsg->Data[4] = Index;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_STATUS);
   MrCs2SetDlc(CanMsg, 5);
}

/**********************************************************************\
* Funktionsname: MrCs2EncStatus6
*
* Kurzbeschreibung:
* encode function for CAN status command messages
*
* Parameter:
* CanMsg     : data, which hold the encoded CAN data
* Uid        : uid of receiver
* Index      : index for receiver
* NumPackets : number of packets for receiver
*
\**********************************************************************/
void MrCs2EncStatus6(MrCs2CanDataType *CanMsg, unsigned long Uid, int Index,
                     int NumPackets)
{
   SetLongToByteArray((char *)CanMsg->Data, Uid);
   CanMsg->Data[4] = Index;
   CanMsg->Data[5] = NumPackets;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_STATUS);
   MrCs2SetDlc(CanMsg, 6);
}
