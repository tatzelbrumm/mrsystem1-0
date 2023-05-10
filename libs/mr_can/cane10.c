/**********************************************************************\
* Dateiname: cane10.c
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
* Funktionsname: MrCs2EncS88Poll5
*
* Kurzbeschreibung:
* encode function for CAN S88 poll command messages
*
* Parameter:
* CanMsg    : data, which hold the encoded CAN data
* Uid       : uid of receiver
* NumModuls : num of modules for receiver
*
\**********************************************************************/
void MrCs2EncS88Poll5(MrCs2CanDataType *CanMsg, unsigned long Uid,
                      int NumModuls)
{
   SetLongToByteArray((char *)CanMsg->Data, Uid);
   CanMsg->Data[4] = NumModuls;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_S88_POLL);
   MrCs2SetDlc(CanMsg, 5);
}

/**********************************************************************\
* Funktionsname: MrCs2EncS88Poll7
*
* Kurzbeschreibung:
* encode function for CAN S88 poll command messages
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* Uid    : uid of reciver
* Modul  : modul for receiver
* State  : sate for receiver
*
\**********************************************************************/
void MrCs2EncS88Poll7(MrCs2CanDataType *CanMsg, unsigned long Uid,
                      int Modul, int State)
{
   SetLongToByteArray((char *)CanMsg->Data, Uid);
   CanMsg->Data[4] = Modul;
   SetIntToByteArray((char *)&(CanMsg->Data[5]), State);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_S88_POLL);
   MrCs2SetDlc(CanMsg, 7);
}
