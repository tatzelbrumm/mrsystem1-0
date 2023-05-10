/**********************************************************************\
* Dateiname: cane06.c
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
* Funktionsname: MrCs2EncFunction5
*
* Kurzbeschreibung:
* encode function for CAN loc function command messages
*
* Parameter:
* CanMsg   : data, which hold the encoded CAN data
* LocId    : loc id for receiver
* Function : function for receiver
*
\**********************************************************************/
void MrCs2EncFunction5(MrCs2CanDataType *CanMsg, unsigned long LocId,
                       int Function)
{
   SetLongToByteArray((char *)CanMsg->Data, LocId);
   CanMsg->Data[4] = Function;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_FUNCTION);
   MrCs2SetDlc(CanMsg, 5);
}

/**********************************************************************\
* Funktionsname: MrCs2EncFunction6
*
* Kurzbeschreibung:
* encode function for CAN loc function command messages
*
* Parameter:
* CanMsg   : data, which hold the encoded CAN data
* LocId    : loc id for receiver
* Function : function for receiver
* Value    : value for receiver
*
\**********************************************************************/
void MrCs2EncFunction6(MrCs2CanDataType *CanMsg, unsigned long LocId,
                       int Function, int Value)
{
   SetLongToByteArray((char *)CanMsg->Data, LocId);
   CanMsg->Data[4] = Function;
   CanMsg->Data[5] = Value;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_FUNCTION);
   MrCs2SetDlc(CanMsg, 6);
}

/**********************************************************************\
* Funktionsname: MrCs2EncFunction8
*
* Kurzbeschreibung:
* encode function for CAN loc function command messages
*
* Parameter:
* CanMsg   : data, which hold the encoded CAN data
* LocId    : loc id for receiver
* Function : function for receiver
* Value    : value for receiver
* FktVal   : fkt value for receiver
*
\**********************************************************************/
void MrCs2EncFunction8(MrCs2CanDataType *CanMsg, unsigned long LocId,
                       int Function, int Value, int FktVal)
{
   SetLongToByteArray((char *)CanMsg->Data, LocId);
   CanMsg->Data[4] = Function;
   CanMsg->Data[5] = Value;
   SetIntToByteArray((char *)&(CanMsg->Data[6]), FktVal);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_FUNCTION);
   MrCs2SetDlc(CanMsg, 8);
}
