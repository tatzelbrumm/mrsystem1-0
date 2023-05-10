/**********************************************************************\
* Dateiname: set_int.c
* Autor    : Michael Bernstein
* Projekt  : bytestream
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: write int to array of char
*
\**********************************************************************/

/*--- #includes der Form <...> ---------------------------------------*/
#include <bitmask.h>
/*--- #includes der Form "..." ---------------------------------------*/
#include "bytestream.h"

/**********************************************************************\
* Funktionsname: SetIntToByteArray
*
* Kurzbeschreibung:
* write a int to a array of bytes in big endian
*
* Parameter:
* Data  : array of bytes, from which should read
* Value : value to write to array of char
*
\**********************************************************************/
void SetIntToByteArray(char *Data, unsigned int Value)
{
   Data[0] = (Value >> SHIFT_1_BYTE) & MASK_LOW_BYTE;
   Data[1] = (Value >> SHIFT_0_BYTE) & MASK_LOW_BYTE;
}
