#ifndef SCANNER_H
#define SCANNER_H

#define TOKEN_ERROR            0x0000
#define TOKEN_EOL              0x0001
#define TOKEN_NL               0x0002
#define TOKEN_START_PARAGRAPH  0x0003
#define TOKEN_END_PARAGRAPH    0x0004
#define TOKEN_LEVEL            0x0005
#define TOKEN_EQUAL            0x0006
#define TOKEN_VALUE            0x0007
#define TOKEN_INTEGER          0x0008

typedef struct {
   char *KeyWord;
   int Token;
} KeyWordEntry;

#define ScannerKeyWordsGetKeyWord(Data) (Data)->KeyWord
#define ScannerKeyWordsGetToken(Data)   (Data)->Token

#define MAX_STR_LEN 80

typedef struct {
   char *InputLine;
   int PosInInputLine;
   int ValueInt;
   char ValueString[MAX_STR_LEN];
   KeyWordEntry *KeyWords;
} ScannerStruct;

ScannerStruct *ScannerCreate(void);
void ScannerDestroy(ScannerStruct *Data);
void ScannerInit(ScannerStruct *Data, char *InputLine);
int ScannerScan(ScannerStruct *Data);
int ScannerScanEol(ScannerStruct *Data);

#define ScannerSetInputString(Data, Str)      (Data)->InputLine=Str
#define ScannerSetPosInInputString(Data, Pos) (Data)->PosInInputLine=Pos
#define ScannerSetValueInt(Data, Val)         (Data)->ValueInt=Val
#define ScannerSetValueString(Data, Str)      strcpy((Data)->ValueString, Str)
#define ScannerSetKeyWords(Data, Keys)        (Data)->KeyWords=Keys

#define ScannerGetInputString(Data)      (Data)->InputLine
#define ScannerGetPosInInputString(Data) (Data)->PosInInputLine
#define ScannerGetValueInt(Data)         (Data)->ValueInt
#define ScannerGetValueString(Data)      (Data)->ValueString
#define ScannerGetKeyWords(Data)         (Data)->KeyWords
#define ScannerGetKeyWordsIdx(Data,Idx)  &((Data)->KeyWords[Idx])

#endif
