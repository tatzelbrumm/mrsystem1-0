/**********************************************************************\
* Dateiname: cand21.c
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
* Funktionsname: MrCs2DecCfgdatStream6
*
* Kurzbeschreibung:
* decode function for CAN cfg data stream command messages
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* Length : holds the decoded length
* Crc    : holds the decoded crc
*
\**********************************************************************/
void MrCs2DecCfgdatStream6(MrCs2CanDataType *CanMsg, unsigned long *Length,
                           int *Crc)
{
   *Length = GetLongFromByteArray((char *)CanMsg->Data);
   *Crc = GetIntFromByteArray((char *)&(CanMsg->Data[4]));
}

/**********************************************************************\
* Funktionsname: MrCs2DecCfgdatStream7
*
* Kurzbeschreibung:
* decode function for CAN cfg data stream command messages
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* Length : holds the decoded length
* Crc    : holds the decoded crc
* Byte6  : holds the decoded byte 6
*
\**********************************************************************/
void MrCs2DecCfgdatStream7(MrCs2CanDataType *CanMsg, unsigned long *Length,
                           int *Crc,char *Byte6)
{
   *Length = GetLongFromByteArray((char *)CanMsg->Data);
   *Crc = GetIntFromByteArray((char *)&(CanMsg->Data[4]));
   *Byte6 = CanMsg->Data[6];
}

/**********************************************************************\
* Funktionsname: MrCs2DecCfgdatStream8
*
* Kurzbeschreibung:
* decode function for CAN cfg data stream command messages
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* Bytes  : holds the decoded 8 bytes
*
\**********************************************************************/
void MrCs2DecCfgdatStream8(MrCs2CanDataType *CanMsg, char *Bytes)
{  int i;

   for (i = 0; i < 8; i++)
      Bytes[i] = CanMsg->Data[i];
}
