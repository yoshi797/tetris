#ifndef INCLUDED_TETRIS
#define INCLUDED_TETRIS

#ifdef GLOBAL_VALUE_DEFINE
    #define GLOBAL
    #define GLOBAL_VAL(v) = (v)
#else
    #define GLOBAL extern
    #define GLOBAL_VAL(v)
#endif


#include<stdio.h>
#include<ctype.h>
#include<ncurses.h>
#include<locale.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

/*----------------------------*/
/*システム定数の宣言*/
/*----------------------------*/
#define BUFFER_LEN 80 /*入力バッファの文字数*/

#define GAME_UP_Y 3 /*ゲームの上端*/
#define GAME_SIDE_X 5  /*ゲーム画面の左端*/
#define GAME_X 10 /*テトリスの横*/
#define GAME_Y 20 /*テトリスの縦*/
#define SCORE_X 35/*スコア画面の横*/

/*コマンドの定義*/

#define CMD_MAX 3/*コマンド入力の文字数*/
#define START 's' /*ゲームの開始*/
#define RANK  'k' /*ランキング*/
#define QUIT  'q'  /*ゲームの終了*/ 
#define PLAY_TIME 120 /*制限時間*/

/*ゲームに関する定義*/
#define RIGHT 'r'/*右回転*/
#define LEFT 'l'/*左回転*/
#define HOLD 'h'/*ホールド*/

/*----------------------------*/
/*グローバル変数の宣言*/
/*----------------------------*/
GLOBAL char cmd_list[]; //コマンドリスト

GLOBAL int BLOCK[4][4];//落下するブロック(テトリミノ)
GLOBAL int BLOCK_H[4][4]; //ホールド用
GLOBAL int BLOCK_F[19][4][4];//落下するブロック全19パターン

GLOBAL int score  GLOBAL_VAL(0);
GLOBAL int ranking[5];  //ゲームスコア

GLOBAL int r_h GLOBAL_VAL(0); //ホールド用
GLOBAL int r_n GLOBAL_VAL(0); //次ブロック用
GLOBAL int cell[GAME_Y][GAME_X]; //0はブロック無し、1はブロックあり
GLOBAL int cell_t[GAME_Y][GAME_X];//ブロックの配置の一時保存場所
GLOBAL int block_tall[GAME_X]; //ゲーム画面の各列のブロックの個数
GLOBAL int block_bottom[GAME_X];//落下ブロックの最底辺
GLOBAL int line[GAME_Y];//列の消去

GLOBAL time_t start_time, end_time, now_time;

/*----------------------------*/
/*使用する関数の宣言*/
/*----------------------------*/

/*コマンド処理用部品*/
char input_cmd(void); 
void exec_cmd(char);

/*ゲーム用部品*/
void game(void); 
int random_block(void); 
void r_revolution(void); 
void l_revolution(void); 

void blocktall_check(void);
void block_check(void);
int block_pile(int,int);
int game_score(int);
void del_block(void);
void pack_line(int);

void cell_copy_a(void); 
void cell_copy_b(void);
void cell_copy_c(int );
void init_buff(void);
int cell_check(int);

#include "display.h"

#endif 
