#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "formulaParser.h"
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

typedef struct STACKTYPE STACKTYPE;
struct STACKTYPE
{
    BCD *bcd;
    NodeKind nodeKind; // todo おそらく使う必要はないので削除する
};

// グローバル変数
/*******************************************/

// 計算用スタック
STACKTYPE *stack[CALCULATE_STACK_MAX] = {};
unsigned short stackIndex = 0;
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
    stackIndex--;
    STACKTYPE *st = NULL;
    st = stack[stackIndex];
    stack[stackIndex] = NULL;
    return st;
}

// プリント関数
/*******************************************/


void printStack()
{
    char str[BCD_MAX_DIGITS + 1]; // ex: +0000010000
    STACKTYPE *st = NULL;
    bool isFirstRender = true;

    for (int i = stackIndex; i >= 0; i--)
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
            convertBCDtoSTR(st->bcd, str, true, true);
            printf(" bcd:%s", str);
            // memset(str, 0, sizeof str);
            //     convertBCDtoSTR(st->bcd, str, true, false);
            //     printf(" bcd:%s", str);
            // memset(str, 0, sizeof str);
            //     convertBCDtoSTR(st->bcd, str, false, false);
            //     printf(" bcd:%s", str);
            // memset(str, 0, sizeof str);
        }
        else
        {
            printf("                ");
        }
        printf("|\n");
        printf("|__________________________|\n");

        memset(str, 0, sizeof str);
    }
}

/*******************************************/

/**
 * NodeTreeを再帰的に降下し、プッシュダウンオートマトンとしての計算を実行する
 */
void calculate(Node *node)
{

    if (node->kind == ND_NUM)
    {
        BCD *bcd = convertDECtoBCD(node->val, false);
        push(bcd, ND_NUM);
        return;
    }

    calculate(node->lhs);
    calculate(node->rhs);

    STACKTYPE *left = pop();
    STACKTYPE *right = pop();
    BCD *tmp = NULL;

    switch (node->kind)
    {
    case ND_ADD:
        tmp = addOrSubForSignedBCD(left->bcd, right->bcd, OP_ADD);
        break;
    case ND_SUB:
        tmp = addOrSubForSignedBCD(left->bcd, right->bcd, ND_SUB);
        break;
    case ND_MUL:
        // push(NULL, ND_MUL);
        break;
    case ND_DIV:
        // push(NULL, ND_DIV);
        break;
    }

    free(left->bcd);
    free(right->bcd);
    free(left);
    free(right);
    push(tmp, ND_NUM);
}

/**
 *
 * @param argv ex: "500 + 400"
 *
 */
int main(int argc, char **argv)
{
    debug_printf("\n argc is %d,\n", argc);
    debug_printf("input param is \"%s\"\n", argv[1]);
    if (argc != 2)
    {
        fprintf(stderr, "引数の個数が正しくありません\n");
        exit(1);
    }

    // トークナイズしてパースする
    Node *node = formulaParser(argc, argv[1]);
    calculate(node);

#ifdef DEBUG
    printStack();
#endif
    char result[BCD_MAX_DIGITS + 1];
    convertBCDtoSTR(pop()->bcd, result, false, false);
    printf("%s", result);
    return 0;
}