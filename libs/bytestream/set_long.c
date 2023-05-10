/**********************************************************************\
* Dateiname: set_long.c
* Autor    : Michael Bernstein
* Projekt  : bytestream
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: write long to array of char
*
\**********************************************************************/

/*--- #includes der Form <...> ---------------------------------------*/
#include <bitmask.h>
/*--- #includes der Form "..." ---------------------------------------*/
#include "bytestream.h"

/**********************************************************************\
* Funktionsname: SetLongToByteArray
*
* Kurzbeschreibung:
* write a long to a array of bytes in big endian
*
* Parameter:
* Data  : array of bytes, from which should read
* Value : value to write to array of char
*
\**********************************************************************/
void SetLongToByteArray(char *Data, unsigned long Value)
{
   Data[0] = (Value >> SHIFT_3_BYTE) & MASK_LOW_BYTE;
   Data[1] = (Value >> SHIFT_2_BYTE) & MASK_LOW_BYTE;
   Data[2] = (Value >> SHIFT_1_BYTE) & MASK_LOW_BYTE;
   Data[3] = (Value >> SHIFT_0_BYTE) & MASK_LOW_BYTE;
}
