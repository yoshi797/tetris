#include "tetris.h"



/* initcolor()*/
/*-------------------------------------------------------------*/
/*
    書式: void initcolor(void)

    機能: 色設定を初期化する．

    返値: なし
*/
void initcolor(){
    start_color();
    initcolorpair();
}

/* initcolorpair()*/
/*-------------------------------------------------------------*/
/*
    書式: void initcolorpair(void)

    機能: 色ペアを初期化する．

    返値: なし
*/
void initcolorpair(){
    init_pair(NONE_COLOR, COLOR_BLACK,COLOR_RED);
    init_pair(PILED_COLOR, COLOR_BLACK, COLOR_CYAN);
    init_pair(BLOCK_COLOR, COLOR_BLACK,COLOR_BLUE);
    init_pair(WALL_COLOR, COLOR_BLACK,COLOR_WHITE);
}


/* game_print*/
/*-------------------------------------------------------------*/
/*
    書式: void game_print(void)

    機能: ゲームのブロックの表示を行う。

    返値: なし
*/
void game_print(){
    
    /*ゲームフィールドなどの表示*/
    field_print();

    /*ブロックの表示*/
    for(int y=0;y<GAME_Y;y++){
        move(GAME_UP_Y+y,GAME_LEFT_X);
        
        for(int x=0;x<GAME_X;x++){
            
            if(cell[y][x] == NONE_COLOR){
                color_set(NONE_COLOR, NULL);
                printw("  ");
            }
            if(cell[y][x] == BLOCK_COLOR){
                color_set(BLOCK_COLOR, NULL);
                printw("  ");
            }
            if(cell[y][x] == PILED_COLOR){
                color_set(PILED_COLOR, NULL);
                printw("  ");
            }
        }
        
    }

    //ホールドしているブロックの表示
    move(GAME_UP_Y,SCORE_X);
    color_set(NONE_COLOR, NULL);
    printw("HOLD");
    if(r_h != -1){
        for(int y=0;y<4;y++){
            move(GAME_UP_Y+y+2,SCORE_X);
            for(int x=0;x<4;x++){
                if(BLOCK_F[r_h][y][x] == NONE_COLOR){
                    color_set(NONE_COLOR, NULL);
                    printw("  ");
                }
                if(BLOCK_F[r_h][y][x] == BLOCK_COLOR){
                    color_set(BLOCK_COLOR, NULL);
                    printw("  ");
                }

            }
        }
    }

    //次のブロックの表示
    move(GAME_UP_Y,SCORE_X+10);
    color_set(NONE_COLOR, NULL);
    printw("NEXT");
    if(r_n != -1){
        for(int y=0;y<4;y++){
            move(GAME_UP_Y+y+2,SCORE_X+10);
            for(int x=0;x<4;x++){
                if(BLOCK_F[r_n][y][x] == 0){
                    color_set(NONE_COLOR, NULL);
                    printw("  ");
                }
                if(BLOCK_F[r_n][y][x] == 2){
                    color_set(BLOCK_COLOR, NULL);
                    printw("  ");
                }
            }
        }
    }
}

/* field_print*/
/*-------------------------------------------------------------*/
/*
    書式: void game_print(void)

    機能: ゲームフィールド、スコアの表示を行う。

    返値: なし
*/
void field_print(){
    /*前処理*/
    del_lines(GAME_UP_Y,GAME_Y+2,GAME_LEFT_X-1);
    
    /*ゲームフィールドの表示*/
    for(int y=0;y<GAME_Y;y++){
        move(GAME_UP_Y+y,GAME_LEFT_X-2);
        color_set(WALL_COLOR, NULL);
        printw("  ");
        move(GAME_UP_Y+y,GAME_RIGHT_X);
        color_set(WALL_COLOR, NULL);
        printw("  ");
    }
    move(GAME_Y+GAME_UP_Y,GAME_LEFT_X-2);
    for(int x=0;x<GAME_X+2;x++){
        color_set(WALL_COLOR, NULL);
        printw("  ");
    }


    color_set(NONE_COLOR, NULL);
    move(GAME_UP_Y+7,GAME_LEFT_X+3);
    printw("s : game start");
    move(GAME_UP_Y+9,GAME_LEFT_X+3);
    printw("k : ranking");
    move(GAME_UP_Y+11,GAME_LEFT_X+3);
    printw("q : quit");
    move(13,SCORE_X);
    printw("q : game end");
    move(14,SCORE_X);
    printw("r : rotate clockwise");
    move(15,SCORE_X);
    printw("l : rotate counter-clockwise");
    move(16,SCORE_X);
    printw("→: move right");
    move(17,SCORE_X);
    printw("←: move left");
    move(18,SCORE_X);
    printw("↓: move down");

    


    /*残り時間、スコアの表示*/
    time(&now_time); //時刻を取得
    move(20,SCORE_X);
    color_set(NONE_COLOR,NULL);
    
    if(end_time - now_time < 0){ //終了時刻を過ぎたとき
        printw("time: 0s");
    }else{
        printw("time:%2ds",end_time - now_time);
    }
    move(21,SCORE_X);
    printw("SCORE:%d",score);
}

/* rank */
/*--------------------------------------------------------*/
/*
    書式: void rank()

    機能: ランキングを表示する

    返値: なし
*/
void rank(){
    move(GAME_UP_Y+3,GAME_LEFT_X+3);
    printw("<Ranking>");
    for(int i=0;i<5;i++){
        move(GAME_UP_Y+5+2*i,GAME_LEFT_X+2);
        printw("rank:%d score:%d",i+1,ranking[i]);
    }
}

/* renew_rank */
/*--------------------------------------------------------*/
/*
    書式: void renew_rank()

    機能: ランキングの更新をする

    返値: なし
*/
void renew_rank(){
    int i,j;

    for(i=0;i<5;i++){
        if(score >= ranking[i]){
            for(j=4;j>i;j--){
                ranking[j] = ranking[j-1];
            }
            ranking[i] = score;
            break;
        }
    }
}

/* score_load */
/*--------------------------------------------------------*/
/*
    書式: void score_load()

    機能: スコアデータのロード

    返値: なし
*/
void score_load(void)
{
    int i;
    FILE *fp;

    /*データファイルからの読み込み*/
    if( (fp=fopen( "score.txt", "r" )) == NULL ){
        score_save();
        return ;
    }        

    for(i=0;i<5;i++){
        fscanf(fp,"%d",&(ranking[i]));
    }

    /*後始末*/
    fclose( fp );
}

/* score_save */
/*--------------------------------------------------------*/
/*
    書式: void score_save(void)

    機能: スコアデータをセーブする

    返値: なし
*/
void score_save(void)
{
    int i;
    FILE *fp;

    /*データファイルへの書き出し*/
    if( ( fp=fopen( "score.txt", "w" ) ) == NULL ){
        printw("ERROR!");
        return;
    }
    for(i=0;i<5;i++){
        fprintf( fp, "%d\n",ranking[i]);
    }

    /*後始末*/
    fclose(fp);
}

/* del_lines*/
/*-------------------------------------------------------------*/
/*
    書式: void del_lines(int,int,int)

    機能: 第3引数から行末までを、第1引数から第2引数までの行数分行う.

    返値: なし
*/
void del_lines(int start,int end ,int x)
{
    int i;
    move(start,x);
    for(i=start;i<=end;i++){
        clrtoeol();
        addch('\n');
    }
}