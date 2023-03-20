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

char *ReadLineBuf = ":empty:";
#include "gb_string.h"

char *ReadLine(FILE * InputFile)
{
    if(InputFile == 0) {
        if(ReadLineBuf[0] == '\0') {
            return 0;
        }

        gbString currentLine = gb_make_string("");

        while(ReadLineBuf[0] != '\0') {
            char singleCh[2];
            singleCh[0] = ReadLineBuf[0];
            singleCh[1] = '\0';

            currentLine = gb_append_cstring(currentLine, singleCh);

            ReadLineBuf++;
            if(ReadLineBuf[0] == '\n') {
                ReadLineBuf++;
                break;
            }
        }

        return (char*)currentLine;
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
