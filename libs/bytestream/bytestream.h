/**********************************************************************\
* Dateiname: bytestream.h
* Autor    : Michael Bernstein
* Projekt  :
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: extract/write ints to stream of bytes
*
\**********************************************************************/

#ifndef BYTESTRAEM_H
#define BYTESTREAM_H

/*--- Prototypen globaler Funktionen ---------------------------------*/
unsigned long GetLongFromByteArray(char *Data);
unsigned int GetIntFromByteArray(char *Data);
void SetLongToByteArray(char *Data, unsigned long Value);
void SetIntToByteArray(char *Data, unsigned int Value);

#endif /* BYTESTREAM_H */
