#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <ftw.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#define VERSION "1.0.0"

char* str = NULL;
int len = 0;

static int findString(const char* fPath, const struct stat* sb, int tFlag, struct FTW* ftwBuf)
{

    if(tFlag == FTW_F)
    {
        char buf[64] = {0};
        char cmd[64] = {0};
        FILE* pf;
        snprintf(cmd, 64, "%s %s", "file", fPath);
        if((pf = popen(cmd, "r")))
        {
            if(fgets(buf, 64, pf))
            {
                pclose(pf);
                if(strstr(buf, "ASCII"))
                {
                    FILE* file = fopen(fPath, "r");
                    if(str != NULL && len > 0)
                    {
                        fseek(file, 0, SEEK_END);
                        long size = ftell(file);
                        fseek(file, 0, SEEK_SET);
                        char* strBuf = malloc(size + 1);
                        fread(strBuf, 1, size, file);
                        char* found = strstr(strBuf, str);
                        while(found)
                        {
                            printf("Found in %s at %d\n", fPath, (int)(found - strBuf));
                            found = strstr(found+1, str);
                        }
                        free(strBuf);
                    }
                    fclose(file);
                }
            }
        }
    }
    return 0;
}

void printHelp()
{
    printf("String Finder\nVersion: %s\nUsage:\tstringfinder path stringToFind\n", VERSION);
}

int main(int argc, char *argv[])
{
    switch(argc)
    {
        case 1:
            printHelp();
            break;
        case 2:
            if(strcmp(argv[1], "help") == 0)
            {
                printHelp();
            }
            break;
        case 3:
        {
            char* path = argv[1];
            str = argv[2];
            len = strlen(str);
            nftw(path, findString, 20, 0);
        }
            break;
        default:
            return EX_USAGE;
            break;
    }
    return 0;
}
