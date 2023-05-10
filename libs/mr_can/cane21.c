/**********************************************************************\
* Dateiname: cane21.c
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
* Funktionsname: MrCs2EncCfgdatStream6
*
* Kurzbeschreibung:
* encode function for CAN config data command messages
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* Length : length to receiver
* Crc    : crc to receiver
*
\**********************************************************************/
void MrCs2EncCfgdatStream6(MrCs2CanDataType *CanMsg, unsigned long Length,
                           int Crc)
{
   SetLongToByteArray((char *)CanMsg->Data, Length);
   SetIntToByteArray((char *)&(CanMsg->Data[4]), Crc);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_CFGDAT_STREAM);
   MrCs2SetDlc(CanMsg, 6);
}

/**********************************************************************\
* Funktionsname: MrCs2EncCfgdatStream7
*
* Kurzbeschreibung:
* encode function for CAN config data command messages
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* Length : length to receiver
* Crc    : crc to receiver
* Byte6  : byte 6 to receiver
*
\**********************************************************************/
void MrCs2EncCfgdatStream7(MrCs2CanDataType *CanMsg, unsigned long Length,
                           int Crc, char Byte6)
{
   SetLongToByteArray((char *)CanMsg->Data, Length);
   SetIntToByteArray((char *)&(CanMsg->Data[4]), Crc);
   CanMsg->Data[6] = Byte6;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_CFGDAT_STREAM);
   MrCs2SetDlc(CanMsg, 7);
}

/**********************************************************************\
* Funktionsname: MrCs2EncCfgdatStream8
*
* Kurzbeschreibung:
* encode function for CAN config data command messages
*
* Parameter:
* CanMsg : data, which hold the encoded CAN data
* Bytes  : data bytes to receiver
*
\**********************************************************************/
void MrCs2EncCfgdatStream8(MrCs2CanDataType *CanMsg, char *Bytes)
{  int i;

   for (i = 0; i < 8; i++)
      CanMsg->Data[i] = Bytes[i];
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_CFGDAT_STREAM);
   MrCs2SetDlc(CanMsg, 8);
}
