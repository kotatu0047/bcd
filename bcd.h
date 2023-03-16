#ifndef BCD_HEADER
#define BCD_HEADER

#define BCD_STRUCT_SIZE 12 // bcd構造体のサイズ
#define BCD_9_INDEX 8
#define BCD_1_INDEX 4
#define BCD_MAX_DIGITS 10 // bcdの有効桁数

#define CALCULATE_STACK_MAX 20

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
    LEFT,
    RIGHT,
    EQUAL,
} COMPEAR_RESULT;

typedef enum
{
    OP_ADD, // +
    OP_SUB, // -
    OP_MUL, // *
    OP_DIV, // /
} OperatorKind;

// 実装がリトルエンディアンであることに依存している
typedef union
{
    unsigned char value[BCD_STRUCT_SIZE]; // 96bit (0〜4294967295までの値を入れること)
    struct _tag                           // 32+40+1 =73bit
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

/**
 * convertBCDtoSTR
 *
 * @param bcd
 * @param str  変換した文字列の格納先 BCD_MAX_DIGITS + 1 以上のサイズが必要
 * @param isDisplaySign 0以上の値でも符号を表示する   ex: "+10000"
 * @param isPaddingZero 0埋めを行うか   ex: "0000010000"
 *
 */
void convertBCDtoSTR(BCD *bcd, char *str, bool isDisplaySign, bool isPaddingZero);



    BCD *convertDECtoBCD(unsigned long dec, bool _sign);
    COMPEAR_RESULT absCompear(BCD *a, BCD *b);
    BCD *unsignedAdd(BCD *a, BCD *b);
    BCD *unsignedSub(BCD *a, BCD *b);
    BCD *addOrSubForSignedBCD(BCD *a, BCD *b, OperatorKind op);

#ifdef __cplusplus
};
#endif

#endif