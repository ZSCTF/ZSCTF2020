#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char encode_characters[65] = "0123456789aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ+-";

unsigned int calc_hash(unsigned int x)
{
    return (~x * x * ~x * x * ~x << 5u) ^
           ((((x << 1) & 0xacacacacu)) ^ ((~x >> 1) & 0x55555555u)) ^
           ((x << 2u) ^ (x >> 2u));
}

unsigned int str_to_uint(char *str)
{
    return ((str[0] & 0xff) << 24) |
           ((str[1] & 0xff) << 16) |
           ((str[2] & 0xff) << 8) |
           (str[3] & 0xff);
}

int find_index(char arr[], char c)
{
    int len = strlen(arr);
    for (int i = 0; i < len; i++)
    {
        if (arr[i] == c)
        {
            return i;
        }
    }
    return -1;
}

void decrypt(char *src, char *dst)
{
    int len = strlen(src);
    for (int i = 0; i < len / 4; i++)
    {
        unsigned int tmp = (find_index(encode_characters, src[i * 4]) << 18) +
                           (find_index(encode_characters, src[i * 4 + 1]) << 12) +
                           (find_index(encode_characters, src[i * 4 + 2]) << 6) +
                           find_index(encode_characters, src[i * 4 + 3]);
        dst[i * 3] = (tmp >> 16) & 0xff;
        dst[i * 3 + 1] = (tmp >> 8) & 0xff;
        dst[i * 3 + 2] = tmp & 0xff;
    }
    dst[len / 4 * 3] = '\0';
}

int main()
{
    printf("welcome to zsctf2020 guess game\n");
    printf("please enter 16 letters or digits: ");

    char input[17] = {0},
         output[17] = {0};

    int len = 0;
    int last = 0;
    while (len < 16)
    {
        char c = _getch();

        if (c < 32 || c > 126)
        {
            return 0;
        }
        if (!isalnum(c))
        {
            continue;
        }

        last = last ^ find_index(encode_characters, c) ^ (len << 2);
        input[len] = c;
        output[len] = encode_characters[last];
        putchar(output[len]);
        len++;
    }

    printf("\n");

    char decrypted_str[13] = {0};
    decrypt(output, decrypted_str);

    unsigned int num1 = str_to_uint(decrypted_str),
                 num2 = str_to_uint(decrypted_str + 4),
                 num3 = str_to_uint(decrypted_str + 8);

    if (calc_hash(num1) == 555555555u &&
        calc_hash(num2) == 0x5555555u &&
        calc_hash(num3) == 05555555555u)
    {
        printf("correct, the flag is zsctf{%s}\n", input);
    }
    else
    {
        printf("wrong\n");
    }

    system("pause");
    return 0;
}
