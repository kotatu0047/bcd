#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "bcd.h"

// デバッグ用   https://hotnews8.net/programming/tricky-code/macro03
/*******************************************/
// #define DEBUG //makeの引数で、有効,無効を切り替えるので、ここは触らないように
#ifdef DEBUG
#define debug_printf printf
#else
#define debug_printf 1 ? (void)0 : printf
#endif
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

void convertBCDtoSTR(BCD *bcd, char *str, bool isDisplaySign, bool isPaddingZero)
{
    if (bcd->bitFiled.sign)
    {
        str[0] = '-';
    }
    else
    {
        if (isDisplaySign)
            str[0] = '+';
    }

    char low_buf[3];
    char high_buf[3];
    bool hasGoneFirstDigits = false; // 先頭桁を変換するとtrueになる
    size_t strIndex = str[0] ? 1 : 0;
    if (isPaddingZero)
    {
        for (int i = BCD_9_INDEX; i >= BCD_1_INDEX; i--)
        {
            short high = (bcd->value[i] >> 4) & 0x0f;
            snprintf(high_buf, 3, "%d", high);
            str[strIndex] = high_buf[0];
            memset(high_buf, 0, sizeof high_buf);
            strIndex++;

            short low = bcd->value[i] & 0x0f;
            snprintf(low_buf, 3, "%d", low);
            str[strIndex] = low_buf[0];
            memset(low_buf, 0, sizeof low_buf);
            strIndex++;
        }
    }
    else
    {
        for (int i = BCD_9_INDEX; i >= BCD_1_INDEX; i--)
        {
            short high = (bcd->value[i] >> 4) & 0x0f;
            if (hasGoneFirstDigits || high > 0)
            {
                snprintf(high_buf, 3, "%d", high);
                str[strIndex] = high_buf[0];
                memset(high_buf, 0, sizeof high_buf);
                strIndex++;
                hasGoneFirstDigits = true;
            }

            short low = bcd->value[i] & 0x0f;
            if (hasGoneFirstDigits || low > 0)
            {
                snprintf(low_buf, 3, "%d", low);
                str[strIndex] = low_buf[0];
                memset(low_buf, 0, sizeof low_buf);
                strIndex++;
                hasGoneFirstDigits = true;
            }
        }
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
COMPEAR_RESULT absCompear(BCD *a, BCD *b)
{
    BCD *bcd = calloc(1, sizeof(BCD));
    bcd->bitFiled = a->bitFiled; // たぶんここバグってる
    // bcd->bitFiled.sign = a->bitFiled.sign;

    debug_printf("dump_hex(bcd->value, 12);   ");
#ifdef DEBUG
    dump_hex(bcd->value, 12);
#endif

    // 2桁(1byt)ずつ上位ケタから比較していく
    for (int i = BCD_9_INDEX; i >= BCD_1_INDEX; i--)
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
 *
 * 符号部は評価せずに、単純に|a| + |b| を返す
 * 戻り値の符号部は必ず+になる
 *
 */
BCD *unsignedAdd(BCD *a, BCD *b)
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

#ifdef DEBUG
    dump_hex(result->value, 12);
#endif
    debug_printf("0x%024x, 0x%01x  0x%01x 0x%01x 0x%01x 0x%01x 0x%01x 0x%01x 0x%01x 0x%01x 0x%01x \n", result->bitFiled.binary,
                 result->bitFiled.BCD_10, result->bitFiled.BCD_9, result->bitFiled.BCD_8, result->bitFiled.BCD_7, result->bitFiled.BCD_6,
                 result->bitFiled.BCD_5, result->bitFiled.BCD_4, result->bitFiled.BCD_3, result->bitFiled.BCD_2, result->bitFiled.BCD_1);

    return result;
}

/**
 *
 * 符号部は評価せずに、単純に|a| - |b| を返す
 * 戻り値が-になることもある
 *
 */
BCD *unsignedSub(BCD *a, BCD *b)
{
    BCD *result = calloc(1, sizeof(BCD));
    result->bitFiled.sign = 0;

    BCD *left = NULL;
    BCD *right = NULL;

    // a >= b となるようにする
    COMPEAR_RESULT cmp = absCompear(a, b);
    if (cmp == LEFT || cmp == EQUAL)
    {
        left = a;
        right = b;
    }
    else
    {
        left = b;
        right = a;
        result->bitFiled.sign = 1; //   |a| < |b| であらば計算結果は負になる
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
 * 符号付きBCDの加減算を行う
 *
 *
   https://drive.google.com/file/d/1aO7k4OFiw5WWtb6ED3IHUUGSofpUDABQ/view?usp=share_link
 *
*/
BCD *addOrSubForSignedBCD(BCD *a, BCD *b, OperatorKind op)
{
    BCD *result = NULL;
    bool aIsNegative = a->bitFiled.sign;
    bool bIsNegative = b->bitFiled.sign;

    if (!(op == OP_ADD || op == OP_SUB))
    {
        fprintf(stderr, "引数が不正です:addOrSubForSignedBCD\n");
        exit(1);
    }

    if ((!aIsNegative && !bIsNegative && op == OP_ADD) || (!aIsNegative && bIsNegative && op == OP_SUB))
    {
        result = unsignedAdd(a, b); // |a| + |b|
    }
    else if ((!aIsNegative && !bIsNegative && op == OP_SUB) || (!aIsNegative && bIsNegative && op == OP_ADD))
    {
        result = unsignedSub(a, b); // |a| - |b|
    }
    else if ((aIsNegative && !bIsNegative && op == OP_ADD) || (aIsNegative && bIsNegative && op == OP_SUB))
    {
        result = unsignedSub(b, a); // |b| - |a|
    }
    else if ((aIsNegative && !bIsNegative && op == OP_SUB) || (aIsNegative && bIsNegative && op == OP_ADD))
    {
        result = unsignedAdd(a, b); // -(|a|＋|b|)
        result->bitFiled.sign = 1;
    }

    return result;
}
