#ifndef INCLUDED_DISPLAY
#define INCLUDED_DISPLAY


/*----------------------------*/
/*システム定数の宣言*/
/*----------------------------*/
#define BUFFER_LEN 80 /*入力バッファの文字数*/

#define GAME_UP_Y 3 /*ゲームの上端*/
#define GAME_LEFT_X 5  /*ゲーム画面の左端*/
#define GAME_RIGHT_X 25 /*ゲーム画面の右端*/
#define GAME_X 10 /*テトリスの横*/
#define GAME_Y 20 /*テトリスの縦*/
#define SCORE_X 35/*スコア画面の横*/

#define NONE_COLOR 0
#define PILED_COLOR 1
#define BLOCK_COLOR 2
#define WALL_COLOR 3
/*ランク処理用関数*/
void rank(void);
void renew_rank(void);
void score_load(void);
void score_save(void);
void initcolor(void);
void initcolorpair(void);

/*画面表示用部品*/
void game_print(void); 
void field_print(void); 
void del_lines(int,int,int); 


#endif 
