/**********************************************************************\
* Dateiname: cane0b.c
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
* Funktionsname: MrCs2EncAccSwitch6
*
* Kurzbeschreibung:
* encode function for CAN acc switch command messages
*
* Parameter:
* CanMsg   : data, which hold the encoded CAN data
* LocId    : loc id for receiver
* Position : position for receiver
* Current  : current for receiver
*
\**********************************************************************/
void MrCs2EncAccSwitch6(MrCs2CanDataType *CanMsg, unsigned long LocId,
                        int Position, int Current)
{
   SetLongToByteArray((char *)CanMsg->Data, LocId);
   CanMsg->Data[4] = Position;
   CanMsg->Data[5] = Current;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_ACC_SWITCH);
   MrCs2SetDlc(CanMsg, 6);
}

/**********************************************************************\
* Funktionsname: MrCs2EncAccSwitch8
*
* Kurzbeschreibung:
* encode function for CAN acc switch command messages
*
* Parameter:
* CanMsg   : data, which hold the encoded CAN data
* LocId    : loc id for receiver
* Position : position for receiver 
* Current  : current for receiver
* TimeFkt  : time fkt for receiver
*
\**********************************************************************/
void MrCs2EncAccSwitch8(MrCs2CanDataType *CanMsg, unsigned long LocId,
                        int Position, int Current, int TimeFkt)
{
   SetLongToByteArray((char *)CanMsg->Data, LocId);
   CanMsg->Data[4] = Position;
   CanMsg->Data[5] = Current;
   SetIntToByteArray((char *)&(CanMsg->Data[6]), TimeFkt);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_ACC_SWITCH);
   MrCs2SetDlc(CanMsg, 8);
}
