#include <cstdio>
#include <cctype>
#include <cassert>
#include <cstring>
#include <cstdlib>

using namespace std;

const int FLAG_LENGTH = 28;
const int INPUT_LENGTH = FLAG_LENGTH * 2;

char input[INPUT_LENGTH + 20];
char ans[INPUT_LENGTH + 1] = "294726410:9<9><9(\"$'\"&U/*/\"(+[X!T.Q,R+((N7I4?201E>F=@:48";
char flag_hex[INPUT_LENGTH + 1];
char str[] = "0123456789ABCDEF";

void solve(char *result)
{
    strcpy(result, ans);

    for (int i = 0; i < INPUT_LENGTH / 2; i++)
    {
        char t = result[INPUT_LENGTH - i - 1];
        result[INPUT_LENGTH - i - 1] = result[i] ^ i;
        result[i] = t ^ i;
    }

    for (int i = 0; i < INPUT_LENGTH; i++)
    {
        int pos = strchr(str, result[i]) - str;
        result[i] = str[15 - pos];
    }
}

int main()
{
    printf("please input: ");

    solve(input);
    puts(input); // 7A736374667B726576657273655FC79D73C9B9C79DCA8CC79DC9B97D

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

    for (int i = 0; i < INPUT_LENGTH / 2; i++)
    {
        char t = input[INPUT_LENGTH - i - 1];
        input[INPUT_LENGTH - i - 1] = input[i] ^ i;
        input[i] = t ^ i;
    }

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