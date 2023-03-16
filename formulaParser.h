// https://www.sigbus.info/compilerbook
// https://github.com/rui314/9cc
/**
 * MIT License

Copyright (c) 2018 Rui Ueyama

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 * 
 * 
*/


#ifndef FORMULA_PARSER
#define FORMULA_PARSER

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// トークンの種類
typedef enum
{
  TK_RESERVED, // 記号
  TK_NUM,      // 整数トークン
  TK_EOF,      // 入力の終わりを表すトークン
} TokenKind;

typedef struct Token Token;

struct Token
{
  TokenKind kind;
  Token *next;
  unsigned long val;
  char *str;
};

// 現在着目しているトークン
// Token *token;

typedef enum
{
  ND_ADD, // +
  ND_SUB, // -
  ND_MUL, // *
  ND_DIV, // /
  ND_NUM, // num
} NodeKind;

typedef struct Node Node;

struct Node
{
  NodeKind kind;
  Node *lhs;
  Node *rhs;
  unsigned long val;
};

// 入力プログラム
// char *user_input;

// void error_at(char *loc, char *fmt, ...);

// Node *new_node(NodeKind kind, Node *lhs, Node *rhs);

// Node *new_node_num(int val);

// bool consume(char op);

// void expect(char op);

// int expect_number();

// bool at_eof();

// Node *expr();

// Node *primary();

// Node *mul();

// Node *expr();

// void gen(Node *node);

// Token *new_token(TokenKind kind, Token *cur, char *str);

// Token *tokenize(char *p);

Node *formulaParser(int argc, char *argv);

//BCDプロジェクト向けに作成したもの
char getOperatorFromNNodeKind(NodeKind kind);

#endif
