#include <cstdio>
#include <cctype>
#include <cstring>
#include <cassert>

using namespace std;

#ifdef WEGFAN
#define MAKE
#endif

const int FLAG_LENGTH = 35;
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
    60657615, 81731116, 36796762, 82049032, 89427462, 63787070, 85474185, 68438291, 77261047, 60370156,
    52794808, 62057629, 71962401, 96891178, 61457743, 64907948, 53919536, 77292259, 82079880, 57301988,
    63303607, 82802689, 32801886, 64402704, 66867017, 86689416, 71523593, 60665322, 59028575, 59074588,
    47256534, 83061919, 88020913, 52215007, 72469242, 95356536, 65990971, 35173763, 85047505, 29426200,
    100858426, 66234561, 67755982, 56325419, 61405858, 69454810, 61576242, 68646192, 87531855, 83217228,
    52817938, 87259370, 37121570, 41622934, 52358233, 65774262, 69631065, 57754318, 65870832, 64263900,
    80703633, 84745485, 50716094, 59521830, 71656256, 65886510, 51405737, 75952365, 31474114, 92828458,
    73483353, 51399064, 74463232, 114239250, 45601045, 56136480, 41144281, 58001731, 97325996, 65754244,
    52942305, 53693264, 51161992, 76597799, 48024607, 57558967, 60727545, 53376399, 63524077, 79432490,
    66250769, 93270713, 58181829, 42820345, 59052585, 55042479, 45672149, 77538501, 53904077, 79308029 // f**k clang format
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
    strcpy(flag, "zsctf{import_sympy_or_z3_is_easier}");
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