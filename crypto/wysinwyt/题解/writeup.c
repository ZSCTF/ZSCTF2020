#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

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

void encrypt(char *src, char *dst)
{
    // 解密函数里把4个字节的低6位取出来，拼在一起变成3个字节
    // 所以加密函数要把3个字节以6位分组变成4个字节，这样每个字节表示的数在0~63之间，然后再去数组里找对应的字符
    int len = strlen(src);
    for (int i = 0; i < len / 3; i++)
    {
        unsigned int tmp = ((src[i * 3] & 0xff) << 16) +
                           ((src[i * 3 + 1] & 0xff) << 8) +
                           (src[i * 3 + 2] & 0xff);
        dst[i * 4] = encode_characters[(tmp >> 18) & 0b111111];
        dst[i * 4 + 1] = encode_characters[(tmp >> 12) & 0b111111];
        dst[i * 4 + 2] = encode_characters[(tmp >> 6) & 0b111111];
        dst[i * 4 + 3] = encode_characters[tmp & 0b111111];
    }
    dst[len / 3 * 4] = '\0';
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

void solve(char *result)
{
    // 暴力遍历获得hash前的数
    unsigned int pre_hash1, pre_hash2, pre_hash3;
    int count = 0;
    // 注意这里需要使用long long，否则会导致溢出
    for (unsigned long long i = 0x00000000LL; i <= 0xffffffffLL; i++)
    {
        if (calc_hash((unsigned int)i) == 555555555u)
        {
            pre_hash1 = i;
            count++;
        }
        if (calc_hash((unsigned int)i) == 0x5555555u)
        {
            pre_hash2 = i;
            count++;
        }
        if (calc_hash((unsigned int)i) == 05555555555u)
        {
            pre_hash3 = i;
            count++;
        }
    }
    assert(count == 3);
    // pre hash 1: 0x92481a0f hash: 555555555
    // pre hash 2: 0x8daff247 hash: 89478485
    // pre hash 3: 0x01e7ab76 hash: 766958445
    printf("pre hash 1: 0x%08x hash: %u\n", pre_hash1, calc_hash(pre_hash1));
    printf("pre hash 2: 0x%08x hash: %u\n", pre_hash2, calc_hash(pre_hash2));
    printf("pre hash 3: 0x%08x hash: %u\n", pre_hash3, calc_hash(pre_hash3));

    // 将三个数变成十六进制串并拼接
    char decrypted_str[13] = "\x92\x48\x1a\x0f\x8d\xaf\xf2\x47\x01\xe7\xab\x76";
    char encrypted_str[17];

    // 加密（实际上是类base64）
    encrypt(decrypted_str, encrypted_str);
    printf("encrypted: %s\n", encrypted_str);

    result[0] = encrypted_str[0];
    for (int i = 15; i >= 1; i--)
    {
        result[i] = encode_characters[find_index(encode_characters, encrypted_str[i - 1]) ^ find_index(encode_characters, encrypted_str[i]) ^ (i << 2)];
    }

    printf("your input: %s\n", result); // n4bw9Sg5UzdT8WSO
}

int main()
{
    char correct_input[17] = {0};
    solve(correct_input);

    printf("welcome to zsctf2020 broken keyboard game\n");
    printf("please enter 16 letters or digits: ");

    char input[17] = {0}, output[17] = {0};

    int len = 0;
    int last = 0;
    while (true)
    {
        char c = correct_input[len];
        if (len >= 16)
        {
            break;
        }
        last = last ^ find_index(encode_characters, c) ^ (len << 2);
        input[len] = c;
        output[len] = encode_characters[last];
        putchar(output[len]);
        len++;
    }

    printf("\n");
    printf("input: %s\noutput: %s\n", input, output);

    char decrypted_str[13] = {0};
    decrypt(output, decrypted_str);

    unsigned int num1 = str_to_uint(decrypted_str),
                 num2 = str_to_uint(decrypted_str + 4),
                 num3 = str_to_uint(decrypted_str + 8);

    if (calc_hash(num1) == 555555555u &&
        calc_hash(num2) == 0x5555555u &&
        calc_hash(num3) == 05555555555u)
    {
        // zsctf{n4bw9Sg5UzdT8WSO}
        printf("correct, the flag is zsctf{%s}\n", input);
    }
    else
    {
        printf("wrong\n");
    }

    system("pause");
    return 0;
}
