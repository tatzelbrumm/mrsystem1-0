/**********************************************************************\
* Dateiname: cane01.c
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
* Funktionsname: MrCs2EncDiscovery0
*
* Kurzbeschreibung:
* encode function for CAN discovery command messages
*
* Parameter:
* CanMsg   : data, which hold the encoded CAN data
*
\**********************************************************************/
void MrCs2EncDiscovery0(MrCs2CanDataType *CanMsg)
{
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_DISCOVERY);
   MrCs2SetDlc(CanMsg, 0);
}

/**********************************************************************\
* Funktionsname: MrCs2EncDiscovery1
*
* Kurzbeschreibung:
* encode function for CAN discovery command messages
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* Proto  : proto for receiver
*
\**********************************************************************/
void MrCs2EncDiscovery1(MrCs2CanDataType *CanMsg, int Proto)
{
   CanMsg->Data[0] = Proto;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_DISCOVERY);
   MrCs2SetDlc(CanMsg, 1);
}

/**********************************************************************\
* Funktionsname: MrCs2EncDiscovery5
*
* Kurzbeschreibung:
* encode function for CAN discovery command messages
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* Uid    : uid of receiver
* Range  : range for receiver
*
\**********************************************************************/
void MrCs2EncDiscovery5(MrCs2CanDataType *CanMsg, unsigned long Uid,
                        int Range)
{
   SetLongToByteArray((char *)CanMsg->Data, Uid);
   CanMsg->Data[4] = Range;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_DISCOVERY);
   MrCs2SetDlc(CanMsg, 5);
}

/**********************************************************************\
* Funktionsname: MrCs2EncDiscovery6
*
* Kurzbeschreibung:
* encode function for CAN discovery command messages
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* Uid    : uid of receiver
* Range  : range for receiver
* Ask    : ask for receiver
*
\**********************************************************************/
void MrCs2EncDiscovery6(MrCs2CanDataType *CanMsg, unsigned long Uid,
                        int Range, int Ask)
{
   SetLongToByteArray((char *)CanMsg->Data, Uid);
   CanMsg->Data[4] = Range;
   CanMsg->Data[5] = Ask;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_DISCOVERY);
   MrCs2SetDlc(CanMsg, 6);
}
