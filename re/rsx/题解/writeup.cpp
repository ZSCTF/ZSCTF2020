#include <cstdio>
#include <cctype>
#include <cstring>
#include <algorithm>
#include <stack>

using namespace std;

#ifdef WEGFAN
#define MAKE
#endif

const int FLAG_LENGTH = 50;
const int KEY_LENGTH = 70;

char key[KEY_LENGTH + 1] = "the_sha256_of_the_flag_is_not_Ne44roib3Ko6rpDnErL7UzoTbfIlHqsvFCHJczdU";
char ans[FLAG_LENGTH + 1] = "\x09\x1f\x4d\x03\x2f\x00\x53\x14\x3c\x07\x52\x3a\x1a\x70\x0c\x51\x64\x13\x63\x1b\x46\x73\x52\x6a\x79\x6f\x5c\x4f\x3f\x02\x43\x63\x32\x7b\x3c\x7e\x2f\x4b\x16\x31\x7e\x00\x3b\x33\x14\x16\x6a\x5c\x51\x3e";
char flag[FLAG_LENGTH + 10];

unsigned int r = 998244353u;

int randint(int a, int b)
{
    r = (r * 114514 + 1919810) ^ (r >> 4);
    return r % (b - a + 1) + a;
}

void seed(char *str, int len)
{
    unsigned int hash1 = 5381;
    unsigned int hash2 = hash1;

    for (int i = 0; i < len; i += 2)
    {
        hash1 = ((hash1 << 5) + hash1) ^ str[i];
        if (i == len - 1)
        {
            break;
        }
        hash2 = ((hash2 << 5) + hash2) ^ str[i + 1];
    }

    r = hash1 + hash2 * 1566083941;
}

void encrypt(char *str, int str_len, char *key, int key_len, char *result)
{
    memcpy(result, str, str_len);

    seed(key, key_len);
    for (int i = 0; i < str_len; i++)
    {
        result[i] ^= key[randint(0, key_len - 1)];
        if (i + 1 <= str_len - 1)
        {
            int index = randint(i + 1, str_len - 1);
            swap(result[i], result[index]);
            result[i] ^= key[randint(0, key_len - 1)];
        }
        if (i - 1 >= 0)
        {
            int index = randint(0, i - 1);
            swap(result[i], result[index]);
            result[i] ^= key[randint(0, key_len - 1)];
            result[i] ^= result[i - 1];
        }
    }
}

void solve(char *str, int str_len, char *key, int key_len, char *result)
{
    memcpy(result, str, str_len);

    seed(key, key_len);

    stack<int> index_list;
    for (int i = 0; i < str_len; i++)
    {
        index_list.push(randint(0, key_len - 1));
        if (i + 1 <= str_len - 1)
        {
            index_list.push(randint(i + 1, str_len - 1));
            index_list.push(randint(0, key_len - 1));
        }
        if (i - 1 >= 0)
        {
            index_list.push(randint(0, i - 1));
            index_list.push(randint(0, key_len - 1));
        }
    }

    for (int i = str_len - 1; i >= 0; i--)
    {
        if (i - 1 >= 0)
        {
            result[i] ^= result[i - 1];
            result[i] ^= key[index_list.top()];
            index_list.pop();
            int index = index_list.top();
            index_list.pop();
            swap(result[i], result[index]);
        }
        if (i + 1 <= str_len - 1)
        {
            result[i] ^= key[index_list.top()];
            index_list.pop();
            int index = index_list.top();
            index_list.pop();
            swap(result[i], result[index]);
        }
        result[i] ^= key[index_list.top()];
        index_list.pop();
    }
}

int main()
{
    printf("input the flag: ");

    solve(ans, FLAG_LENGTH, key, KEY_LENGTH, flag);
    puts(flag); // zsctf{RanDom_SwaP_XoR_kT4W3T5GtTH3WhD0EihGt2exmkz}

    if (strlen(flag) != FLAG_LENGTH)
    {
        puts("wrong");
        return 0;
    }

    for (int i = 0; i < FLAG_LENGTH; i++)
    {
        if (!isprint(flag[i]))
        {
            puts("wrong");
            return 0;
        }
    }

    char result[FLAG_LENGTH + 10] = {0};
    encrypt(flag, FLAG_LENGTH, key, KEY_LENGTH, result);

    if (memcmp(result, ans, FLAG_LENGTH) == 0)
    {
        printf("correct, your flag is %s\n", flag);
    }
    else
    {
        puts("wrong");
    }
    return 0;
}