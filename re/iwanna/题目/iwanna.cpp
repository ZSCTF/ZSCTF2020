#include <cstdio>
#include <cctype>
#include <cstring>
#include <cassert>

using namespace std;

#ifdef WEGFAN
#define MAKE
#endif

const int FLAG_LENGTH = 41;
const int EQUATION_COUNT = 100;
const int ITEM_PER_EQUATION = 5;

struct Equation
{
    int coef[ITEM_PER_EQUATION];
    int index[ITEM_PER_EQUATION];
    int result;

#ifdef MAKE
    void print()
    {
        for (int i = 0; i < ITEM_PER_EQUATION; i++)
        {
            printf("%s%d * a[%d]", i == 0 ? "" : " + ", coef[i], index[i]);
        }
        printf(" = %d\n", result);
    }
#endif
};

Equation equation[EQUATION_COUNT];
char flag[FLAG_LENGTH + 20];

int result[EQUATION_COUNT] = {
    54878522, 66327491, 29992773, 70866551, 78456666, 63672236, 68384547, 67173520, 79353842, 41938727,
    42375893, 64318238, 68987817, 89879962, 48234173, 63619653, 34854719, 78883489, 72204110, 43043049,
    67581325, 70541094, 27882227, 51591592, 58717340, 73350988, 60620730, 50128309, 60296298, 57688059,
    37550463, 74467721, 71361899, 58296856, 59030105, 86601404, 64423967, 34023068, 66025424, 29086211,
    75957843, 59385315, 65546477, 53461965, 46872401, 61093714, 59501914, 61288944, 72844690, 70570137,
    52672729, 78296672, 34103361, 38136313, 54498657, 58061731, 59632265, 51882955, 54872978, 70300318,
    53691560, 69945096, 39481048, 50576127, 51604567, 63304822, 52908076, 60381842, 30415177, 93440343,
    65197173, 36063646, 71474241, 94430532, 43536030, 42452481, 34628536, 48835270, 83186926, 62316545,
    43978803, 64563885, 51584176, 49213073, 42395088, 57281585, 54756516, 56575445, 64599494, 80775504,
    60509174, 78527425, 52425560, 35662362, 49732449, 43981200, 44826738, 64000807, 53117631, 74699661
    // f**k clang format
};

#ifdef MAKE
int count[FLAG_LENGTH];
#endif

int randint(int a, int b)
{
    static unsigned int r = 998244353u;
    r = (r * 114514 + 1919810) ^ (r >> 4);
    r &= 0x7fffffff;
    return r % (b - a + 1) + a;
}

int main()
{
    printf("please input: ");
    scanf("%50s", flag);

#ifdef MAKE
    strcpy(flag, "zsctf{WoW_You_Can_REALLY_SoLve_EquaTions}");
    assert(strlen(flag) == FLAG_LENGTH);
    puts(flag);
#endif

    int len = strlen(flag);
    if (len != FLAG_LENGTH)
    {
        puts("wrong input");
        return 0;
    }

    for (int i = 0; i < FLAG_LENGTH; i++)
    {
        if (!isprint(flag[i]))
        {
            puts("wrong input");
            return 0;
        }
    }

    for (int i = 0; i < EQUATION_COUNT; i++)
    {
        Equation &cur = equation[i];
        for (int j = 0; j < ITEM_PER_EQUATION; j++)
        {
            cur.coef[j] = randint(0, 233333);
            cur.index[j] = randint(0, FLAG_LENGTH - 1);
            cur.result += cur.coef[j] * flag[cur.index[j]];
#ifdef MAKE
            count[cur.index[j]]++;
#endif
        }
#ifdef MAKE
        result[i] = cur.result;
        cur.print();
#endif
        if (cur.result != result[i])
        {
            puts("wrong input");
            return 0;
        }
    }

#ifdef MAKE
    printf("result = {");
    for (int i = 0; i < sizeof(result) / sizeof(*result); i++)
    {
        printf("%s%d", i == 0 ? "" : ", ", result[i]);
    }
    printf("};\n");
    printf("count = {");
    for (int i = 0; i < sizeof(count) / sizeof(*count); i++)
    {
        printf("%s%d", i == 0 ? "" : ", ", count[i]);
    }
    printf("};\n");
#endif

    printf("correct! your flag is %s\n", flag);
    return 0;
}