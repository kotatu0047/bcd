#include <stdio.h>
#include <stdbool.h>

#define BCD_SIZE 12

// 実装がリトルエンディアンであることに依存している
typedef union
{
    unsigned char value[BCD_SIZE]; // 96bit (0〜4294967295までの値を入れること)
    struct _tag                    // 32+40 =72bit
    {
        unsigned int : 0; // 余った領域は使わせない
        unsigned int binary : 32; // 2^32 = 4294967296 //
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
    } bitFiled;
} BCD;

void dump_hex(char *ptr, size_t length)
{
    printf("0x");
    for (size_t i = 0; i < length; ++i)
    {
        printf("%01x%01x", (ptr[i] >> 4) & 0x0F, ptr[i] & 0x0F);
    }
    printf("\n");
}

int main()
{
    BCD bcd;
    // unsigned char *p = NULL;
    for (size_t i = 0; i <= BCD_SIZE - 1; i++)
    {
        if (i <= 3)
        {
            bcd.value[i] = 0xff;
        }
        else
        {
            bcd.value[i] = 0x00;
        }
    }

    dump_hex(bcd.value, 12);
    printf("0x%024x, 0x%01x  0x%01x 0x%01x 0x%01x 0x%01x 0x%01x 0x%01x 0x%01x 0x%01x 0x%01x \n", bcd.bitFiled.binary,
           bcd.bitFiled.BCD_10, bcd.bitFiled.BCD_9, bcd.bitFiled.BCD_8, bcd.bitFiled.BCD_7, bcd.bitFiled.BCD_6, bcd.bitFiled.BCD_5, bcd.bitFiled.BCD_4, bcd.bitFiled.BCD_3, bcd.bitFiled.BCD_2, bcd.bitFiled.BCD_1);

    for (size_t i = 0; i < 32; i++)
    {
        if (bcd.bitFiled.BCD_1 >= 5)
            bcd.bitFiled.BCD_1 += 3;
        if (bcd.bitFiled.BCD_2 >= 5)
            bcd.bitFiled.BCD_2 += 3;
        if (bcd.bitFiled.BCD_3 >= 5)
            bcd.bitFiled.BCD_3 += 3;
        if (bcd.bitFiled.BCD_4 >= 5)
            bcd.bitFiled.BCD_4 += 3;
        if (bcd.bitFiled.BCD_5 >= 5)
            bcd.bitFiled.BCD_5 += 3;
        if (bcd.bitFiled.BCD_6 >= 5)
            bcd.bitFiled.BCD_6 += 3;
        if (bcd.bitFiled.BCD_7 >= 5)
            bcd.bitFiled.BCD_7 += 3;
        if (bcd.bitFiled.BCD_8 >= 5)
            bcd.bitFiled.BCD_8 += 3;
        if (bcd.bitFiled.BCD_9 >= 5)
            bcd.bitFiled.BCD_9 += 3;
        if (bcd.bitFiled.BCD_10 >= 5)
            bcd.bitFiled.BCD_10 += 3;
        // 1bit左シフト
        bool tmp_head = false;
        for (size_t j = 0; j <= BCD_SIZE - 1; j++)
        {
            bool isAddEnd = tmp_head; //前回シフトした時に溢れていれば、その分1bit目を1にする
            tmp_head = bcd.value[j] & 0x80;
            bcd.value[j] <<= 1;
            if (j != 0 && isAddEnd)
            {
               bcd.value[j] = bcd.value[j] | 0x01;
            }
        }

        dump_hex(bcd.value, 12);
        printf("0x%024x, 0x%01x  0x%01x 0x%01x 0x%01x 0x%01x 0x%01x 0x%01x 0x%01x 0x%01x 0x%01x \n", bcd.bitFiled.binary,
               bcd.bitFiled.BCD_10, bcd.bitFiled.BCD_9, bcd.bitFiled.BCD_8, bcd.bitFiled.BCD_7, bcd.bitFiled.BCD_6, bcd.bitFiled.BCD_5, bcd.bitFiled.BCD_4, bcd.bitFiled.BCD_3, bcd.bitFiled.BCD_2, bcd.bitFiled.BCD_1);
    }

    dump_hex(bcd.value, 12);
    printf("0x%024x, 0x%01x  0x%01x 0x%01x 0x%01x 0x%01x 0x%01x 0x%01x 0x%01x 0x%01x 0x%01x \n", bcd.bitFiled.binary,
           bcd.bitFiled.BCD_10, bcd.bitFiled.BCD_9, bcd.bitFiled.BCD_8, bcd.bitFiled.BCD_7, bcd.bitFiled.BCD_6, bcd.bitFiled.BCD_5, bcd.bitFiled.BCD_4, bcd.bitFiled.BCD_3, bcd.bitFiled.BCD_2, bcd.bitFiled.BCD_1);

    printf("%p \n", &bcd);
    printf("%p \n", &bcd.value);
    printf("%p \n", &bcd.bitFiled);
    for (size_t i = 0; i <= 11; i++)
    {
        printf("%p ", &bcd.value[i]);
    }
    printf("%lu \n", sizeof(bcd));
    printf("%lu \n", sizeof(bcd.value));
    printf("%lu \n", sizeof(bcd.bitFiled));

    return 0;
}