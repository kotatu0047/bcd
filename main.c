#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "formulaParser.h"

// デバッグ用   https://hotnews8.net/programming/tricky-code/macro03
/*******************************************/
// #define DEBUG //makeの引数で、有効,無効を切り替えるので、ここは触らないように
#ifdef DEBUG
#define debug_printf printf
#else
#define debug_printf 1 ? (void)0 : printf
#endif
/*******************************************/

#define BCD_STRUCT_SIZE 12 // bcd構造体のサイズ
#define BCD_9_INDEX 8
#define BCD_1_INDEX 4
#define BCD_MAX_DIGITS 10 // bcdの有効桁数

#define CALCULATE_STACK_MAX 20

typedef enum
{
    LEFT,
    RIGHT,
    EQUAL,
} COMPEAR_RESULT;

// 実装がリトルエンディアンであることに依存している
typedef union
{
    unsigned char value[BCD_STRUCT_SIZE]; // 96bit (0〜4294967295までの値を入れること)
    struct _tag                           // 32+40 =72bit
    {
        unsigned int : 0;         // 余った領域は使わせない
        unsigned int binary : 32; // 2^32 = 4294967296 //  unsigned long int =  0 ～ 4294967295
        // 4*10bit
        unsigned int BCD_1 : 4;  // 0-9
        unsigned int BCD_2 : 4;  // 10-90
        unsigned int BCD_3 : 4;  // 100-900
        unsigned int BCD_4 : 4;  // 1000-9000
        unsigned int BCD_5 : 4;  // 10000-90000
        unsigned int BCD_6 : 4;  // 100000-900000
        unsigned int BCD_7 : 4;  // 1000000-9000000
        unsigned int BCD_8 : 4;  // 10000000-90000000
        unsigned int BCD_9 : 4;  // 100000000-900000000
        unsigned int BCD_10 : 4; // 1000000000-9000000000
                                 //            4294967295
                                 //            4294967295
        unsigned int sign : 1;   // 正負符号 true=負の数,false=正の数
    } bitFiled;
} BCD;

typedef struct STACKTYPE STACKTYPE;
struct STACKTYPE
{
    BCD *bcd;
    NodeKind nodeKind;
};

// グローバル変数
/*******************************************/

// 計算用スタック
STACKTYPE *stack[CALCULATE_STACK_MAX] = {};
unsigned short stackIndex = 0;

/*******************************************/

// プロトタイプ宣言
/*******************************************/
void convertBCDtoSTR(BCD *bcd, char *str);

/*******************************************/

// 計算用スタック操作
/*******************************************/

void push(BCD *bcd, NodeKind kind)
{
    if (stackIndex + 1 >= CALCULATE_STACK_MAX)
    {
        fprintf(stderr, "Calculation stack overflow\n");
        exit(1);
    }
    else
    {
        STACKTYPE *st = (STACKTYPE *)calloc(1, sizeof(STACKTYPE));
        st->nodeKind = kind;
        st->bcd = NULL;
        if (bcd != NULL)
            st->bcd = bcd;
        stack[stackIndex] = st;
        stackIndex++;
    }
}

STACKTYPE *pop()
{
    STACKTYPE *st = NULL;
    st = stack[stackIndex];
    stack[stackIndex] = NULL;
    stackIndex--;
    return st;
}

// プリント関数
/*******************************************/

void dump_hex(char *ptr, size_t length)
{
    printf("0x");
    for (size_t i = 0; i < length; ++i)
    {
        printf("%01x%01x", (ptr[i] >> 4) & 0x0F, ptr[i] & 0x0F);
    }
    printf("\n");
}

void printStack()
{
    char str[BCD_MAX_DIGITS + 1]; // ex: +0000010000
    STACKTYPE *st = NULL;
    bool isFirstRender = true;

    for (size_t i = stackIndex; i >= 0; i--)
    {
        st = stack[i];
        if (st == NULL)
            continue;

        if (isFirstRender)
        {
            printf("____________________________\n");
            isFirstRender = false;
        }
        else
        {
            printf("|__________________________|\n");
        }

        printf("|NodeKind:%c", getOperatorFromNNodeKind(st->nodeKind));
        if (st->bcd != NULL)
        {
            convertBCDtoSTR(st->bcd, str);
            printf(" bcd:%s", str);
        }
        else
        {
            printf("                ");
        }
        printf("|\n");
        printf("|__________________________|\n");

        memset(str, 0, sizeof str);
        //  snprintf(str, BCD_MAX_DIGITS + 1, " ");
        // str = "1111111111\0";
    }
}

/*******************************************/

/**
    ex: +0000010000
*/
void convertBCDtoSTR(BCD *bcd, char *str)
{
    if (bcd->bitFiled.sign)
    {
        str[0] = '-';
    }
    else
    {
        str[0] = '+';
    }

    char low_buf[3];
    char high_buf[3];
    size_t strIndex = 1;
    for (size_t i = BCD_9_INDEX; i >= BCD_1_INDEX; i--)
    {
        short high = (bcd->value[i] >> 4) & 0x0f;
        snprintf(high_buf, 3, "%d", high);
        str[strIndex] = high_buf[0];
        strIndex++;

        short low = bcd->value[i] & 0x0f;
        snprintf(low_buf, 3, "%d", low);
        str[strIndex] = low_buf[0];
        strIndex++;
        memset(high_buf, 0, sizeof high_buf);
        memset(low_buf, 0, sizeof low_buf);
    }
    str[strIndex] = '\0';
}

/**
 * @param dec  0-4294967295
 * @param _sign 正負符号 true=負の数,false=正の数
 */
BCD *convertDECtoBCD(unsigned long dec, bool _sign)
{
    BCD *bcd = calloc(1, sizeof(BCD));
    bcd->bitFiled.binary = dec;
    bcd->bitFiled.sign = _sign;

    for (size_t i = 0; i < 32; i++)
    {
        if (bcd->bitFiled.BCD_1 >= 5)
            bcd->bitFiled.BCD_1 += 3;
        if (bcd->bitFiled.BCD_2 >= 5)
            bcd->bitFiled.BCD_2 += 3;
        if (bcd->bitFiled.BCD_3 >= 5)
            bcd->bitFiled.BCD_3 += 3;
        if (bcd->bitFiled.BCD_4 >= 5)
            bcd->bitFiled.BCD_4 += 3;
        if (bcd->bitFiled.BCD_5 >= 5)
            bcd->bitFiled.BCD_5 += 3;
        if (bcd->bitFiled.BCD_6 >= 5)
            bcd->bitFiled.BCD_6 += 3;
        if (bcd->bitFiled.BCD_7 >= 5)
            bcd->bitFiled.BCD_7 += 3;
        if (bcd->bitFiled.BCD_8 >= 5)
            bcd->bitFiled.BCD_8 += 3;
        if (bcd->bitFiled.BCD_9 >= 5)
            bcd->bitFiled.BCD_9 += 3;
        if (bcd->bitFiled.BCD_10 >= 5)
            bcd->bitFiled.BCD_10 += 3;
        // 1bit左シフト
        bool tmp_head = false;
        for (size_t j = 0; j <= BCD_9_INDEX; j++)
        {
            bool isAddEnd = tmp_head; // 前回シフトした時に溢れていれば、その分1bit目を1にする
            tmp_head = bcd->value[j] & 0x80;
            bcd->value[j] <<= 1;
            if (j != 0 && isAddEnd)
            {
                bcd->value[j] = bcd->value[j] | 0x01;
            }
        }
    }

#ifdef DEBUG
    dump_hex(bcd->value, 12);
#endif
    debug_printf("0x%024x, 0x%01x  0x%01x 0x%01x 0x%01x 0x%01x 0x%01x 0x%01x 0x%01x 0x%01x 0x%01x \n", bcd->bitFiled.binary,
                 bcd->bitFiled.BCD_10, bcd->bitFiled.BCD_9, bcd->bitFiled.BCD_8, bcd->bitFiled.BCD_7, bcd->bitFiled.BCD_6,
                 bcd->bitFiled.BCD_5, bcd->bitFiled.BCD_4, bcd->bitFiled.BCD_3, bcd->bitFiled.BCD_2, bcd->bitFiled.BCD_1);

    return bcd;
}

/**
 * 符号部は評価せず、絶対値が大きい方を判別する
 *
 */
COMPEAR_RESULT unsignedCompear(BCD *a, BCD *b)
{
    BCD *bcd = calloc(1, sizeof(BCD));
    bcd->bitFiled.binary = a->bitFiled.binary;
    bcd->bitFiled.sign = a->bitFiled.sign;

    // 2桁(1byt)ずつ上位ケタから比較していく
    for (size_t i = BCD_9_INDEX; i >= BCD_1_INDEX; i--)
    {
        unsigned short a_buf = a->value[i];
        unsigned short b_buf = b->value[i];
        if (a_buf > b_buf)
        {
            return LEFT;
        }
        else if (a_buf < b_buf)
        {
            return RIGHT;
        }
    }

    return EQUAL;
}

/**
 * これはまだunsignned
 *
 */
BCD *add(BCD *a, BCD *b)
{
    BCD *result = calloc(1, sizeof(BCD));
    result->bitFiled.sign = 0;

    // 2桁(1byt)ずつ計算していく
    unsigned short tmp_1 = 0;         // 下位bit計算結果
    unsigned short carryLowerBit = 0; // 下位bitキャリーフラグ
    unsigned short tmp_2 = 0;         // 上位bit計算結果
    unsigned short tmp_head = 0;      // 前回(下位byt)のキャリーフラグ

    for (size_t j = BCD_1_INDEX; j <= BCD_9_INDEX; j++)
    {
        // 下位bitの計算
        unsigned short a_1 = a->value[j] & 0x0f;
        unsigned short b_1 = b->value[j] & 0x0f;
        if (a_1 + b_1 + tmp_head >= 10)
        {
            carryLowerBit = 1;
        }
        else
        {
            carryLowerBit = 0;
        }
        tmp_1 = (unsigned short)((a_1 + b_1 + tmp_head) % 10);

        // 上位bitの計算
        unsigned short a_2 = (a->value[j] & 0xf0) >> 4;
        unsigned short b_2 = (b->value[j] & 0xf0) >> 4;
        // printf(" a_2 + b_2 + isOverFlow_1 = %d, \n" , a_2 + b_2 + isOverFlow_1);
        if (a_2 + b_2 + carryLowerBit >= 10)
        {
            tmp_head = 1;
        }
        else
        {
            tmp_head = 0;
        }
        tmp_2 = (unsigned short)((a_2 + b_2 + carryLowerBit) % 10);

        result->value[j] = tmp_1 | (tmp_2 << 4);
        tmp_1 = 0;
        tmp_2 = 0;
    }

    return result;
}

/**
 *
 * 符号部は評価せずに、単純に|a| - |b| を返す
 * 戻り値が-になることもある
 *
 * 作りかけ
 *
 */
BCD *unsignedSub(BCD *a, BCD *b)
{
    BCD *result = calloc(1, sizeof(BCD));
    result->bitFiled.sign = 0;

    BCD *left = NULL;
    BCD *right = NULL;

    // a >= b となるようにする
    COMPEAR_RESULT cmp = unsignedCompear(a, b);
    if (cmp == LEFT || cmp == EQUAL)
    {
        left = a;
        right = b;
    }
    else
    {
        left = b;
        right = a;
    }

    // 2桁(1byt)ずつ計算していく
    unsigned short tmp_1 = 0;         // 下位bit計算結果
    unsigned short carryLowerBit = 0; // 下位bitキャリーフラグ
    unsigned short tmp_2 = 0;         // 上位bit計算結果
    unsigned short tmp_head = 0;      // 前回(下位byt)のキャリーフラグ
    for (size_t j = BCD_1_INDEX; j <= BCD_9_INDEX; j++)
    {
        // 下位bitの計算
        unsigned short a_1 = left->value[j] & 0x0f;
        unsigned short b_1 = right->value[j] & 0x0f;
        if ((short)(a_1 - b_1 - tmp_head) < 0)
        {
            carryLowerBit = 1;
            tmp_1 = 10 + a_1 - b_1 - tmp_head;
        }
        else
        {
            carryLowerBit = 0;
            tmp_1 = a_1 - b_1 - tmp_head;
        }

        // 上位bitの計算
        unsigned short a_2 = (left->value[j] & 0xf0) >> 4;
        unsigned short b_2 = (right->value[j] & 0xf0) >> 4;
        if ((short)(a_2 - b_2 - carryLowerBit) < 0)
        {
            tmp_head = 1;
            tmp_2 = 10 + a_2 - b_2 - carryLowerBit;
        }
        else
        {
            tmp_head = 0;
            tmp_2 = a_2 - b_2 - carryLowerBit;
        }

        result->value[j] = tmp_1 | (tmp_2 << 4);
        tmp_1 = 0;
        tmp_2 = 0;
    }

    return result;
}

/**
 * NodeTreeを再帰的に降下し、プッシュダウンオートマトン用の
 * スタックを作成する
 */
void initStack(Node *node)
{
    if (node->kind == ND_NUM)
    {
        BCD *bcd = convertDECtoBCD(node->val, false);
        push(bcd, ND_NUM);
        return;
    }

    initStack(node->lhs);
    initStack(node->rhs);

    switch (node->kind)
    {
    case ND_ADD:
        push(NULL, ND_ADD);
        break;
    case ND_SUB:
        push(NULL, ND_SUB);
        break;
    case ND_MUL:
        push(NULL, ND_MUL);
        break;
    case ND_DIV:
        push(NULL, ND_DIV);
        break;
    }
}

// void calculation()
// {
// }

/**
 *
 * @param argv ex: "500 + 400"
 *
 */
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "引数の個数が正しくありません\n");
        exit(1);
    }

    // トークナイズしてパースする
    Node *node = formulaParser(argc, argv[1]);
    initStack(node);
    debug_printf("input param is \"%s\"\n", argv[1]);

#ifdef DEBUG
    printStack();
#endif

    BCD *bcd500 = convertDECtoBCD(8294967295, false);
    debug_printf("\n\n\n");
    BCD *bcd295 = convertDECtoBCD(4294967295, false);
    debug_printf("\n\n\n");

    BCD *addResult = add(bcd500, bcd295);
    debug_printf("\n\n\n");

#ifdef DEBUG
    dump_hex(addResult->value, 12);
#endif
    debug_printf("0x%024x, 0x%01x  0x%01x 0x%01x 0x%01x 0x%01x 0x%01x 0x%01x 0x%01x 0x%01x 0x%01x \n", addResult->bitFiled.binary,
                 addResult->bitFiled.BCD_10, addResult->bitFiled.BCD_9, addResult->bitFiled.BCD_8, addResult->bitFiled.BCD_7, addResult->bitFiled.BCD_6,
                 addResult->bitFiled.BCD_5, addResult->bitFiled.BCD_4, addResult->bitFiled.BCD_3, addResult->bitFiled.BCD_2, addResult->bitFiled.BCD_1);

    // BCD *bcd0 = convertDECtoBCD(0, true);
    // printf("\n\n\n");

    return 0;
}