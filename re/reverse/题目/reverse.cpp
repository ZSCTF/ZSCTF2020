#include <cstdio>
#include <cctype>
#include <cassert>
#include <cstring>
#include <cstdlib>

using namespace std;

#ifdef WEGFAN
#define MAKE
#endif

const int FLAG_LENGTH = 28;
const int INPUT_LENGTH = FLAG_LENGTH * 2;

char input[INPUT_LENGTH + 20];
char ans[INPUT_LENGTH + 1] = "294726410:9<9><9(\"$'\"&U/*/\"(+[X!T.Q,R+((N7I4?201E>F=@:48";
char flag_hex[INPUT_LENGTH + 1];
char str[] = "0123456789ABCDEF";

int main()
{
    printf("please input: ");
    scanf("%70s", input);

#ifdef MAKE
    char flag[FLAG_LENGTH + 1] = {0};

    strcpy(flag, "zsctf{reverse_ǝsɹǝʌǝɹ}");
    for (int i = 0; flag[i] != '\0'; i++)
    {
        sprintf(flag_hex + 2 * i, "%02X", flag[i] & 0xff);
    }
    strcpy(input, flag_hex);
    puts(input);

    assert(strlen(flag) == FLAG_LENGTH);
    assert(strlen(input) == INPUT_LENGTH);
#endif

    int len = strlen(input);
    if (len != INPUT_LENGTH)
    {
        puts("wrong input");
        return 0;
    }

    for (int i = 0; i < INPUT_LENGTH; i++)
    {
        if (!(input[i] >= '0' && input[i] <= '9') && !(input[i] >= 'A' && input[i] <= 'F'))
        {
            puts("wrong input");
            return 0;
        }
    }

    strcpy(flag_hex, input);

    for (int i = 0; i < INPUT_LENGTH; i++)
    {
        int pos = strchr(str, input[i]) - str;
        input[i] = str[15 - pos];
    }

#ifdef MAKE
    puts(input);
    for (int i = 0; input[i] != '\0'; i++)
    {
        printf("%02X ", input[i] & 0xff);
    }
    puts("");
#endif

    for (int i = 0; i < INPUT_LENGTH / 2; i++)
    {
        char t = input[INPUT_LENGTH - i - 1];
        input[INPUT_LENGTH - i - 1] = input[i] ^ i;
        input[i] = t ^ i;
    }

#ifdef MAKE
    puts(input);
    for (int i = 0; input[i] != '\0'; i++)
    {
        printf("%02X ", input[i] & 0xff);
    }
    puts("");
#endif

    if (strcmp(input, ans) == 0)
    {
        FILE *file = fopen("reverse_flag.txt", "wb");
        if (file == NULL)
        {
            puts("sorry, failed creating flag file");
            return 0;
        }
        for (int i = 0; i < INPUT_LENGTH; i += 2)
        {
            int byte;
            sscanf(flag_hex + i, "%02X", &byte);
            fputc(byte, file);
        }
        fclose(file);
        puts("correct, the flag is in reverse_flag.txt");
    }
    else
    {
        puts("wrong input");
        return 0;
    }

    return 0;
}