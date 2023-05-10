#ifndef PARSER_H
#define PARSER_H


#include <boolean.h>
#include "scanner.h"


#define PARSER_ERROR     0x00
#define PARSER_EOF       0x01
#define PARSER_PARAGRAPH 0x02
#define PARSER_VALUE     0x03

#define PARSER_PARAGRAPH_LOK        0x01
#define PARSER_PARAGRAPH_NUMLOKS    0x02
#define PARSER_PARAGRAPH_LOKOMOTIVE 0x04

#define PARSER_VALUE_LOK      0x01
#define PARSER_VALUE_NAME     0x02
#define PARSER_VALUE_WERT     0x03
#define PARSER_VALUE_UID      0x04
#define PARSER_VALUE_ADRESSE  0x05
#define PARSER_VALUE_TYP      0x06
#define PARSER_VALUE_MFXUID   0x07
#define PARSER_VALUE_SYMBOL   0x08
#define PARSER_VALUE_AV       0x09
#define PARSER_VALUE_BV       0x0a
#define PARSER_VALUE_VOLUME   0x0b
#define PARSER_VALUE_VELOCITY 0x0c
#define PARSER_VALUE_RICHTUNG 0x0d
#define PARSER_VALUE_VMAX     0x0e
#define PARSER_VALUE_VMIN     0x0f
#define PARSER_VALUE_FKT      0x10
#define PARSER_VALUE_DAUER    0x11

#define PARSER_TYPE_LOKNAMEN 0x01
#define PARSER_TYPE_LOKINFO  0x02


typedef struct {
   BOOL Verbose;
   ScannerStruct *Scanner;
   int Type;
   int SubType;
   int Level;
   char Name[20];
   char Value[80];
} ParserStruct;

ParserStruct *ParserCreate(void);
void ParserDestroy(ParserStruct *Data);
void ParserInit(ParserStruct *Data, int Type, char *InputLine);
int ParserParse(ParserStruct *Data);

#define ParserSetVerbose(Data, Val) (Data)->Verbose=Val
#define ParserSetScanner(Data, Scn) (Data)->Scanner=Scn
#define ParserSetType(Data, Val)    (Data)->Type=Val
#define ParserSetSubType(Data, Val) (Data)->SubType=Val
#define ParserSetLevel(Data, Val)   (Data)->Level=Val
#define ParserSetName(Data, Str)    strcpy((Data)->Name, Str)
#define ParserSetValue(Data, Str)   strcpy((Data)->Value, Str)

#define ParserGetVerbose(Data) (Data)->Verbose
#define ParserGetScanner(Data) (Data)->Scanner
#define ParserGetType(Data)    (Data)->Type
#define ParserGetSubType(Data) (Data)->SubType
#define ParserGetLevel(Data)   (Data)->Level
#define ParserGetName(Data)    (Data)->Name
#define ParserGetValue(Data)   (Data)->Value

#endif
