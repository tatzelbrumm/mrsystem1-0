/**********************************************************************\
* Dateiname: cane30.c
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
* Funktionsname: MrCs2EncAutomatic6
*
* Kurzbeschreibung:
* encode function for CAN automatic command messages
*
* Parameter:
* CanMsg   : data, which hold the encoded CAN data
* DeviceId : device id of receiver
* Func     : func of receiver
* Status   : status of receiver
* Param    : param of receiver
*
\**********************************************************************/
void MrCs2EncAutomatic6(MrCs2CanDataType *CanMsg, int DeviceId, int Func,
                        int Status, int Param)
{
   SetIntToByteArray((char *)CanMsg->Data, DeviceId);
   SetIntToByteArray((char *)&(CanMsg->Data[2]), Func);
   CanMsg->Data[4] = Status;
   CanMsg->Data[5] = Param;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_AUTOMATIC);
   MrCs2SetDlc(CanMsg, 6);
}

/**********************************************************************\
* Funktionsname: MrCs2EncAutomatic8
*
* Kurzbeschreibung:
* encode function for CAN automatic command messages
*
* Parameter:
* CanMsg   : data, which hold the encoded CAN data
* DeviceId : device id of receiver
* Func     : func of receiver
* LocId    : loc id of receiver
*
\**********************************************************************/
void MrCs2EncAutomatic8(MrCs2CanDataType *CanMsg, int DeviceId, int Func,
                        unsigned long LocId)
{
   SetIntToByteArray((char *)CanMsg->Data, DeviceId);
   SetIntToByteArray((char *)&(CanMsg->Data[2]), Func);
   SetLongToByteArray((char *)&(CanMsg->Data[4]), LocId);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_AUTOMATIC);
   MrCs2SetDlc(CanMsg, 8);
}
