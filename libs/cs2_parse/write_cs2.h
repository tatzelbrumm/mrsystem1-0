#ifndef WRITE_CS2_H
#define WRITE_CS2_H


#include <stdio.h>


#define CS2_FILE_TYPE_LOKOMOTIVE 1

#define CS2_FILE_STRING_LOKOMOTIVE "lokomotive.cs2"

#define CS2_PARAGRAPH_TYPE_LOK        1
#define CS2_PARAGRAPH_TYPE_NUMLOKS    2
#define CS2_PARAGRAPH_TYPE_LOKOMOTIVE 3

#define CS2_PARAGRAPH_STRING_LOK        "lok"
#define CS2_PARAGRAPH_STRING_NUMLOKS    "numloks"
#define CS2_PARAGRAPH_STRING_LOKOMOTIVE "lokomotive"

#define CS2_TYP_TYPE_MM2_DIL8 1
#define CS2_TYP_TYPE_MM2_LOK  2

#define CS2_TYP_STRING_MM2_DIL8 "mm2_dil8"
#define CS2_TYP_STRING_MM2_LOK  "mm2_lok"


char *Cs2GetTypAsString(int Typ);
FILE *Cs2OpenByName(char *Name);
FILE *Cs2OpenByType(int Cs2FileType);
void Cs2Close(FILE *Stream);
void Cs2WriteParagraphByName(FILE *Stream, char *Paragraph);
void Cs2WriteParagraphByType(FILE *Stream, int Cs2ParagraphType);
void Cs2WriteTitleByName(FILE *Stream, char *Title, int Level);
void Cs2WriteStringValueByName(FILE *Stream, char *Title, char *Value, int Level);
void Cs2WriteIntValueByName(FILE *Stream, char *Title, int Value, int Level);
void Cs2WriteULongValueByName(FILE *Stream, char *Title, unsigned long Value, int Level);
void Cs2WriteHexLongValueByName(FILE *Stream, char *Title, unsigned long Value, int Level);

#endif
