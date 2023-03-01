#include <stdio.h>

// typedef enum {
//   EMPTY_SET_BUTTON,  // どのボタンでもない
//   CALC__NUMBUTTON__NUM00,
//   CALC__NUMBUTTON__NUM01,
//   CALC__NUMBUTTON__NUM02,
//   CALC__NUMBUTTON__NUM03,
//   CALC__NUMBUTTON__NUM04,
//   CALC__NUMBUTTON__NUM05,
//   CALC__NUMBUTTON__NUM06,
//   CALC__NUMBUTTON__NUM07,
//   CALC__NUMBUTTON__NUM08,
//   CALC__NUMBUTTON__NUM09,
// } ButtonKind;

// 実装がリトルエンディアンであることに依存している
typedef union
{
    // unsigned long int value; // 32bit (0〜8192までの値を入れること)
    struct _tag
    {
        // unsigned int binary : 12; // 2^12 = 8192
        unsigned int binary : 16; // 2^16 = 65536
        // 4*5bit
        unsigned int BCD_1 : 4; // 0-9
        unsigned int BCD_2 : 4; // 10-90
        unsigned int BCD_3 : 4; // 100-900
        unsigned int BCD_4 : 4; // 1000-9000
        unsigned int BCD_5 : 4; // 10000-90000
        // unsigned int BCD_6 : 4;
        // unsigned int BCD_7 : 4;
        // unsigned int BCD_8 : 4;
        // unsigned BCD_9 : 4;
    } bitFiled;
} BCD;

// typedef struct BCD BCD;

// typedef struct BCD BCD;

// struct BCD
// {
//     // unsigned binary : 32;
//     unsigned BCD_1 : 4;
//     unsigned BCD_2 : 4;
//     unsigned BCD_3 : 4;
//     unsigned BCD_4 : 4;
//     unsigned BCD_5 : 4;
//     unsigned BCD_6 : 4;
//     unsigned BCD_7 : 4;
//     unsigned BCD_8 : 4;
//     unsigned BCD_9 : 4;
// };

typedef union cif
{
    char c[4];
    char cc[2];
    int i;
    float f;
} UCIF, *Ucif;

void prnum(UCIF *up)
{
    int i;

    printf("i = 0x%08x\n", up->i);
    // printf("f = 0x%08x\n", up->f);
    printf("c = ");
    for (i = 0; i < 4; i++)
    {
        if (i)
            printf(", ");
        printf("0x%02x", up->c[i] & 0xff);
    }
    printf("\n");
    printf("cc = ");
    for (i = 0; i < 2; i++)
    {
        if (i)
            printf(", ");
        printf("0x%02x", up->cc[i] & 0xff);
    }
    printf("\n");
    printf("i = %d\n", up->i);
    printf("f = %f\n", up->f);
}

/**
 *   main関数
 * @return    int [0: 成功, -1:失敗]
 */
int main()
{
    BCD bcd;
    // bcd.value = 0;
    bcd.bitFiled.binary = 65535;

            bcd.bitFiled.BCD_1 = 0x0;
            bcd.bitFiled.BCD_2 = 0x0;
            bcd.bitFiled.BCD_3 = 0x0;
            bcd.bitFiled.BCD_4 = 0x0;
            bcd.bitFiled.BCD_5 = 0x0;

        printf("%u, %u %u %u %u %u\n", bcd.bitFiled.binary,
           bcd.bitFiled.BCD_5, bcd.bitFiled.BCD_4, bcd.bitFiled.BCD_3, bcd.bitFiled.BCD_2, bcd.bitFiled.BCD_1);
    printf("0x%04x,  0x%01x  0x%01x 0x%01x 0x%01x 0x%01x \n", bcd.bitFiled.binary,
           bcd.bitFiled.BCD_5, bcd.bitFiled.BCD_4, bcd.bitFiled.BCD_3, bcd.bitFiled.BCD_2, bcd.bitFiled.BCD_1);


    for (unsigned short i = 0; i < 16; i++)
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
        // if (bcd.bitFiled.BCD_6 >= 5)
        //     bcd.bitFiled.BCD_6 += 3;
        // if (bcd.bitFiled.BCD_7 >= 5)
        //     bcd.bitFiled.BCD_7 += 3;
        // if (bcd.bitFiled.BCD_8 >= 5)
        //     bcd.bitFiled.BCD_8 += 3;
        bcd.bitFiled.binary <<= 1;
    }
    // printf("%lu \n", sizeof(bcd));


    printf("%u, %u %u %u %u %u\n", bcd.bitFiled.binary,
           bcd.bitFiled.BCD_5, bcd.bitFiled.BCD_4, bcd.bitFiled.BCD_3, bcd.bitFiled.BCD_2, bcd.bitFiled.BCD_1);
    printf("0x%04x,  0x%01x  0x%01x 0x%01x 0x%01x 0x%01x \n", bcd.bitFiled.binary,
           bcd.bitFiled.BCD_5, bcd.bitFiled.BCD_4, bcd.bitFiled.BCD_3, bcd.bitFiled.BCD_2, bcd.bitFiled.BCD_1);
    return 0;
}