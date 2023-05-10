/**********************************************************************\
* Dateiname: cane11.c
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
* Funktionsname: MrCs2EncS88Event4
*
* Kurzbeschreibung:
* encode function for CAN S88 event messages
*
* Parameter:
* CanMsg  : data, which hold the encoded CAN data
* Device  : device for reciever
* Contact : contact for receiver
*
\**********************************************************************/
void MrCs2EncS88Event4(MrCs2CanDataType *CanMsg, int Device, int Contact)
{
   SetIntToByteArray((char *)CanMsg->Data, Device);
   SetIntToByteArray((char *)&(CanMsg->Data[2]), Contact);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_S88_EVENT);
   MrCs2SetDlc(CanMsg, 4);
}

/**********************************************************************\
* Funktionsname: MrCs2EncS88Event5
*
* Kurzbeschreibung:
* encode function for CAN S88 event messages
*
* Parameter:
* CanMsg  : data, which hold the encoded CAN data
* Device  : device for receiver
* Contact : contact for receiver
* Param   : param for receiver
*
\**********************************************************************/
void MrCs2EncS88Event5(MrCs2CanDataType *CanMsg, int Device, int Contact,
                       int Param)
{
   SetIntToByteArray((char *)CanMsg->Data, Device);
   SetIntToByteArray((char *)&(CanMsg->Data[2]), Contact);
   CanMsg->Data[4] = Param;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_S88_EVENT);
   MrCs2SetDlc(CanMsg, 5);
}

/**********************************************************************\
* Funktionsname: MrCs2EncS88Event8
*
* Kurzbeschreibung:
* encode function for CAN S88 event messages
*
* Parameter:
* CanMsg   : data, which hold the encoded CAN data
* Device   : device id for receiver
* Contact  : contact for reciever
* StateOld : old state for receiver
* StateNew : new state for receiver
* Time     : time for receiver
*
\**********************************************************************/
void MrCs2EncS88Event8(MrCs2CanDataType *CanMsg, int Device, int Contact,
                       int StateOld, int StateNew, int Time)
{
   SetIntToByteArray((char *)CanMsg->Data, Device);
   SetIntToByteArray((char *)&(CanMsg->Data[2]), Contact);
   CanMsg->Data[4] = StateOld;
   CanMsg->Data[5] = StateNew;
   SetIntToByteArray((char *)&(CanMsg->Data[6]), Time);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_S88_EVENT);
   MrCs2SetDlc(CanMsg, 8);
}
