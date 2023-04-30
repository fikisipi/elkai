#include "LKH.h"

/*      
 * The ReadLine function reads the next input line from a file. The function
 * handles the problem that an input line may be terminated by a carriage
 * return, a newline, both, or EOF.
 */

static char *Buffer;
static int MaxBuffer;

static int EndOfLine(FILE * InputFile, int c)
{
    int EOL = (c == '\r' || c == '\n');
    if (c == '\r') {
        c = fgetc(InputFile);
        if (c != '\n' && c != EOF)
            ungetc(c, InputFile);
    }
    return EOL;
}

#include "gb_string.h"

char *ReadLineBuf = 0;
int ReadLinePtr = 0;
static gbString ReadLastLine = 0;

void WriteLine(gbString str) {
    if(ReadLineBuf == 0) {
        ReadLineBuf = gb_make_string("");
    }
    ReadLineBuf = gb_append_string(ReadLineBuf, str);
}

void ClearLines() {
    ReadLinePtr = 0;
    if(ReadLineBuf != 0) {
        gb_free_string(ReadLineBuf);
        ReadLineBuf = 0;
    }
    if(ReadLastLine != 0) {
        gb_free_string(ReadLastLine);
        ReadLastLine = 0;
    }
}

double ReadNumber() {
    if(ReadLinePtr == 0) return 0;
    char *k = ReadLineBuf + ReadLinePtr;
    double output = strtof(ReadLineBuf + ReadLinePtr, &k);
    ReadLinePtr += k - (ReadLineBuf + ReadLinePtr);
    return output;
}

int ReadNumberInt() {
    if(ReadLinePtr == 0) return 0;
    char *k = ReadLineBuf + ReadLinePtr;
    int output = strtol(ReadLineBuf + ReadLinePtr, &k, 10);
    ReadLinePtr += k - (ReadLineBuf + ReadLinePtr);
    return output;
}

char *ReadLine(FILE * InputFile)
{
    if(InputFile == 0) {
        if(ReadLineBuf[ReadLinePtr] == '\0') {
            return 0;
        }

        if(ReadLastLine != 0) {
            gb_free_string(ReadLastLine);
            ReadLastLine = 0;
        }

        gbString currentLine = gb_make_string("");

        while(ReadLineBuf[ReadLinePtr] != '\0') {
            char singleCh[2];
            singleCh[0] = ReadLineBuf[ReadLinePtr];
            singleCh[1] = '\0';

            currentLine = gb_append_cstring(currentLine, singleCh);

            ReadLinePtr++;
            if(ReadLineBuf[ReadLinePtr] == '\n') {
                ReadLinePtr++;
                break;
            }
        }

          ReadLastLine = currentLine;
          return currentLine;
    }

    int i, c;

    if (Buffer == 0)
        Buffer = (char *) malloc(MaxBuffer = 80);
    for (i = 0; (c = fgetc(InputFile)) != EOF && !EndOfLine(InputFile, c);
         i++) {
        if (i >= MaxBuffer - 1) {
            MaxBuffer *= 2;
            Buffer = (char *) realloc(Buffer, MaxBuffer);
        }
        Buffer[i] = (char) c;
    }
    Buffer[i] = '\0';
    if (!LastLine || (int) strlen(LastLine) < i) {
        free(LastLine);
        LastLine = (char *) malloc((i + 1) * sizeof(char));
    }
    strcpy(LastLine, Buffer);
    return c == EOF && i == 0 ? 0 : Buffer;
}
