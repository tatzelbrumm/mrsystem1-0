/**********************************************************************\
* Dateiname: cand07.c
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
* Funktionsname: MrCs2DecReadConfig6
*
* Kurzbeschreibung:
* decode function for CAN read config command messages
*
* Parameter:
* CanMsg  : data, which hold the encoded CAN data
* LocId   : holds the decoded loc id
* CvIndex : holds the decoded cv index
*
\**********************************************************************/
void MrCs2DecReadConfig6(MrCs2CanDataType *CanMsg, unsigned long *LocId,
                         int *CvIndex)
{
   *LocId = GetLongFromByteArray((char *)CanMsg->Data);
   *CvIndex = GetIntFromByteArray((char *)&(CanMsg->Data[4]));
}

/**********************************************************************\
* Funktionsname: MrCs2DecReadConfig7
*
* Kurzbeschreibung:
* decode function for CAN read config command messages
*
* Parameter:
* CanMsg  : data, which hold the encoded CAN data
* LocId   : holds the decoded loc id
* CvIndex : holds the decoded cv index
* Param   : holds the decoded parameter
*
\**********************************************************************/
void MrCs2DecReadConfig7(MrCs2CanDataType *CanMsg, unsigned long *LocId,
                         int *CvIndex, int *Param)
{
   *LocId = GetLongFromByteArray((char *)CanMsg->Data);
   *CvIndex = GetIntFromByteArray((char *)&(CanMsg->Data[4]));
   *Param = CanMsg->Data[6];
}
