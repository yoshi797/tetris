#define GLOBAL_VALUE_DEFINE
#include "tetris.h"

int BLOCK_F[19][4][4]={
    /* 
        ＊＊＊＊ 
    */
    {{2,2,2,2},{0,0,0,0},{0,0,0,0},{0,0,0,0}},  
    {{2,0,0,0},{2,0,0,0},{2,0,0,0},{2,0,0,0}}, 
    /*
        ＊
        ＊＊＊
    */
    {{2,0,0,0},{2,2,2,0},{0,0,0,0},{0,0,0,0}},
    {{2,2,0,0},{2,0,0,0},{2,0,0,0},{0,0,0,0}},
    {{2,2,2,0},{0,0,2,0},{0,0,0,0},{0,0,0,0}},
    {{0,2,0,0},{0,2,0,0},{2,2,0,0},{0,0,0,0}},
    /*
        　　＊
        ＊＊＊
    */
    {{0,0,2,0},{2,2,2,0},{0,0,0,0},{0,0,0,0}},
    {{2,0,0,0},{2,0,0,0},{2,2,0,0},{0,0,0,0}},
    {{2,2,2,0},{2,0,0,0},{0,0,0,0},{0,0,0,0}},
    {{2,2,0,0},{0,2,0,0},{0,2,0,0},{0,0,0,0}},
    /*
        　＊
        ＊＊＊
    */
    {{0,2,0,0},{2,2,2,0},{0,0,0,0},{0,0,0,0}},
    {{2,0,0,0},{2,2,0,0},{2,0,0,0},{0,0,0,0}},
    {{2,2,2,0},{0,2,0,0},{0,0,0,0},{0,0,0,0}},
    {{0,2,0,0},{2,2,0,0},{0,2,0,0},{0,0,0,0}},
    /*
        ＊
        ＊＊
        　＊
    */
    {{2,0,0,0},{2,2,0,0},{0,2,0,0},{0,0,0,0}},
    {{0,2,2,0},{2,2,0,0},{0,0,0,0},{0,0,0,0}},
    /*
        　＊
        ＊＊
        ＊
    */
    {{0,2,0,0},{2,2,0,0},{2,0,0,0},{0,0,0,0}},
    {{2,2,0,0},{0,2,2,0},{0,0,0,0},{0,0,0,0}},
    /*
        ＊＊
        ＊＊
    */
    {{2,2,0,0},{2,2,0,0},{0,0,0,0},{0,0,0,0}}};
int ranking[5] ={0,0,0,0,0};
char cmd_list[] = {
    START,
    RANK,
    QUIT
};


int main()
{
    char command;

    setlocale(LC_ALL,""); // for UTF-8 setting
    initscr();          //画面初期化
    clear();     
    initcolor();
    curs_set(0);   //カーソル非表示
    //矢印キーの有効化
    noecho(); 
    cbreak();      
    keypad(stdscr, TRUE);

    init_buff();
    field_print(); //ゲーム画面の表示

    score_load();

   /*コマンドの入力と実行*/
    while(( command = input_cmd() ) != QUIT){
        init_buff();
        exec_cmd(command);
    }

    score_save();
    /*後始末する*/
    clear();
    endwin();
    return 0;
}

/*input_cmd*/
/*------------------------------------------------------------------------*/
/*
    書式:char input_cmd()

    機能:住所録のコマンドを入力し、妥当なコマンドであれば、入力されたコマンド
         を返す。もしも、不適当なコマンドであれば、再入力を要求する。

    返値:入力されたコマンド
*/
char input_cmd()
{
    int cmd;
    int match = FALSE;
    int i;
    
    /*コマンドの入力*/
    for(;;){


        move(12,SCORE_X);
        cmd = getch();
        for(i=0;i<CMD_MAX;i++){ /*妥当なコマンドか?*/
            if(cmd == cmd_list[i]){
                match = TRUE;
                break;
            }
        }
        if(!match){ /*再入力*/
            move(9,35);
            printw("push button again !!");
        }else{
            break;
        }
    }
    /*後始末*/
    del_lines(10,12,SCORE_X);
    return(cmd);
}

/*exec_cmd*/
/*------------------------------------------------------------------*/
/*
    書式:void exec_cmd(char cmd)

    機能:変数cmdで、指定されたコマンドを実行する。

    返値:なし
*/
void exec_cmd(
    char cmd /*コマンド*/
)
{
    switch(cmd){
        case START:
                game(); /*追加*/
                renew_rank();
                score = 0;
                break;
        case RANK:
                rank();
                break;
        default:
                break;
    }
}

/* random_block()*/
/*-------------------------------------------------------------*/
/*
    書式: int random_block(void)

    機能: 落下するブロックをランダムで選ぶ

    返値: ブロックの番号
*/
int random_block(){
    int r1;
    srand(time(NULL));

    //乱数の生成
    r1 = 0 + rand() % 7;

    switch(r1){ //ブロックの要素番号を返す
        case 0: return rand() % 2;
                break;
        case 1: return 2 + rand() % 4;
                break;
        case 2: return 6 + rand() % 4;
                break;
        case 3: return 10 + rand() % 4;
                break;
        case 4: return 14 + rand() % 4;
                break;
        case 5: return 16 + rand() % 2;
                break;
        default:return 18;
                break;
    }
}


/* game*/
/*-------------------------------------------------------------*/
/*
    書式: void game(void)

    機能: ゲームの処理と表示を行う

    返値: なし
*/
void game(){
    int c,r,tmp;
    int i;//ブロック移動用の変数
    int COUNT = 1;
    int count = 0;
    int p=3,q=0;

    r_h = -1;
    r_n = -1;

    
    time(&start_time); //ゲーム開始時刻の測定
    end_time = start_time + PLAY_TIME; //ゲーム終了時刻
    time(&now_time); 
    

    game_print();
    r = random_block();

    while(end_time - now_time >= 0){
        cell_copy_a();

        if(COUNT==1){ /*ブロックを初めて表示するとき*/
            cell_copy_c(r);
            r_n = random_block() ;
            //ブロックデータの代入
            for(int  y=0;y<4;y++){
                for(int x=0;x<4;x++){
                    if(cell[0][x] == 1 || cell[y+q][x+p] == 1){ //ゲームオーバー判定
                        move(10,SCORE_X);    
                        printw("GAME OVER!!!");
                        return;
                    }
                    if(BLOCK[y][x] == 2) cell[y+q][x+p] = BLOCK[y][x];
                }
            }
        }
        for(int  y=0;y<4;y++){
            for(int x=0;x<4;x++){
                if(BLOCK[y][x] == 2) cell[y+q][x+p] = BLOCK[y][x];
            }
        }

        //ゲーム画面の表示
        game_print(); 
        
        //入力と移動
        move(22,SCORE_X);
        c = getch();
        if(c == QUIT){ /*終了*/
        field_print();
            break;
        }else if(c == HOLD){ /*ホールド*/
            q=0;
            p=3;
            if(r_h == -1){
                r_h = r; 
                r = r_n;
                r_n = random_block();
                cell_copy_c(r);
            }else{
                //ホールドのブロックと交換
                tmp = r;
                r = r_h;
                r_h = tmp;
                cell_copy_c(r);
            }
            cell_copy_b();
            COUNT = 0;

        }else if(c == LEFT){ /*左回転*/
            
            l_revolution();
            cell_copy_b();
            //壁との当たり判定
            if(p+4 > GAME_X-1) p = GAME_X - 5;
            if(q+4 > GAME_Y-1) q = GAME_Y - 5;
            COUNT = 0;

        }else if(c == RIGHT){ /*右回転*/
            
            r_revolution();
            cell_copy_b();
            //壁との当たり判定
            if(p+4 > GAME_X-1) p = GAME_X - 4;
            if(q+4 > GAME_Y-1) q = GAME_Y - 5;
            COUNT = 0;

        }else{ 
            /*左右下の移動*/
            switch(c){
                case KEY_DOWN :/*下*/
                                //壁との当たり判定
                                for(i=0;i<4;i++){
                                    if(cell[GAME_Y-1][p+i] == 2){
                                        count=1;
                                        break;
                                    }
                                }
                                //ブロックが下まで来たとき
                                if((COUNT = block_pile(p,q)) == 1){ 
                                    q=0;
                                    p=3;
                                    del_block(); //消去できるラインがあるか判定
                                    cell_copy_b();
                                    r = r_n;
                                    break;
                                }
                                //壁にぶつからないとき
                                if(count == 0) q++; 
                                cell_copy_b();
                                break;
                                
                        
                case KEY_RIGHT :/*右*/
                                //壁との当たり判定
                                for(i=0;i<4;i++){
                                    if(cell[q+i][GAME_X-1] == 2){
                                        count=1;break;
                                    }
                                }
                                //他のブロックとの当たり判定
                                for(int  y=0;y<4;y++){
                                    for(int x=0;x<4;x++){
                                        if(BLOCK[y][x] == 2 && cell[y+q][x+p+1] == 1){
                                        count=1;break;
                                        }
                                    }
                                }
                                COUNT = 0;
                                if(count == 0) p++; //ぶつからないとき
                                cell_copy_b();
                                break;

                case KEY_LEFT :/*左*/
                                //壁との当たり判定
                                for(i=0;i<4;i++){
                                    if(cell[q+i][0] == 2){
                                        count=1;break;
                                    }
                                }
                                //他のブロックとの当たり判定
                                for(int  y=0;y<4;y++){
                                    for(int x=0;x<4;x++){
                                        if(BLOCK[y][x] == 2 && cell[y+q][x+p-1] == 1){
                                        count=1;break;
                                        }
                                    }
                                }
                                COUNT = 0;
                                if(count == 0) p--; //ぶつからないとき
                                cell_copy_b();
                                break;

                default: /*その他の入力*/
                                COUNT = 0;
                                cell_copy_b();
                                break;
            }
        }
        count = 0;

    }
}

/*blocktall_check*/
/*---------------------------------------------------*/
/*
  書式:void blocktall_check()

  機能:山が各列でどの高さまであるか判定する
  
  返り値:なし
*/
void blocktall_check(){
    int x,y,k;
    for(k=0;k<GAME_X;k++){
        block_tall[k] = 0;
    }
    for(y=GAME_Y-1;y>=0;y--){
        for(x=GAME_X-1;x>=0;x--){
            if(cell[y][x] == 1){
                block_tall[x] = GAME_Y-y;
            }
        }
    }
}

/*block_check*/
/*---------------------------------------------------*/
/*
  書式:void block_check()
  
  機能:落下するブロックの最下点がどの位置にあるか判定する
  
  返り値:なし
*/
void block_check(){
    int x,y,k;
    for(k=0;k<GAME_X;k++){
        block_bottom[k] = 0;
    }
    for(x=0;x<GAME_X;x++){
        for(y=0;y<GAME_Y;y++){
            if(cell[y][x] == 2 && cell[y+1][x] !=2){
                block_bottom[x] = y;
            }
        }
    }
}

/*block_pile*/
/*---------------------------------------------------*/
/*
  書式:void block_pile(int p,int q)
  
  機能:落下するブロックを落下後の山のブロックに変換する
  
  返り値:0,1
*/
int block_pile(int p,int q){
    int x;

    blocktall_check();
    block_check();

    for(x=0;x<GAME_X;x++){
        if(block_tall[x] + block_bottom[x] == GAME_Y-1){ //落下し終わったとき
            for(int  y=0;y<4;y++){
                for(int x=0;x<4;x++){
                    if(BLOCK[y][x] == 2) cell[y+q][x+p] = BLOCK[y][x] - 1;
                }
            }
            return 1;   
        }
    }
    return 0;
}


/*del_block*/
/*---------------------------------------------------*/
/*
  書式:del_block(void)
  機能:ラインが出来た部分を消去し、山を詰める
  返り値:なし
*/
void del_block(){
    int count=0; //同時に消えるラインの本数
    int x,y,l=0;
    int lines=0;

    //データの初期化
    for(int i=0;i<GAME_Y;i++){
                for(int j=0;j<GAME_X;j++){
                    cell_t[i][j] = 0;
                }
            }

    //消去できるラインの判定
    for(y=GAME_Y-1;y>=0;y--){
        for(x=0;x<GAME_X;x++){
            if(cell[y][x] != 0){
                lines ++;
            }
        }
        if(lines == 0){ //ブロックが１つもないとき
            score = game_score(count);
            return;
        }else if(lines < GAME_X){ //ラインができていないとき
            for(int k=0;k<GAME_X;k++){
                cell_t[GAME_Y-1-l][k] = cell[y][k];

            }
            l++;
        }else{ //ラインが出来ているとき
            count = count +1;
        }
        lines = 0;
    }
}

/*game_score*/
/*---------------------------------------------------*/
/*
  書式:game_score(int count)
  機能:ラインを消した時のスコアを更新する
  返り値:score
*/
int game_score(int count){
    
    //スコアの更新
    switch(count){
        case 1:  return score + 40;
                 break;
        case 2:  return score + 100;
                 break;
        case 3:  return score + 300;
                 break;
        case 4:  return score + 1200;
                 break;
        default: return score; 
                 break;
        }
}

/*cell_copy_a*/
/*---------------------------------------------------*/
/*
  書式:cell_copy_a()
  機能:ゲーム画面を一時保存用に保管する
  返り値:なし
*/
void cell_copy_a()
{
    int x,y;
    for( y=0;y<GAME_Y;y++){
        for(x=0;x<GAME_X;x++){
            cell_t[y][x] = cell[y][x];
        }
    }
}

/*cell_copy_b*/
/*---------------------------------------------------*/
/*
  書式:cell_copy_()
  機能:ゲーム画面の配列を取り出す
  返り値:なし
*/
void cell_copy_b()
{
    int x,y;

    for( y=0;y<GAME_Y;y++){
        for(x=0;x<GAME_X;x++){
            cell[y][x] = cell_t[y][x] ;
        }
    }
}

/*cell_copy_c*/
/*---------------------------------------------------*/
/*
  書式:cell_copy_c(int a)
  機能:落下するブロックを代入する
  返り値:なし
*/
void cell_copy_c(int a){
    int x,y;

    for(y=0;y<4;y++){
        for(x=0;x<4;x++){
            BLOCK[y][x] = BLOCK_F[a][y][x];
        }
    }
}

/*cell_check*/
/*---------------------------------------------------*/
/*
  書式:cell_check(int a)
  機能:落下するブロックがどれか判定する
  返り値:0,1
*/
int cell_check(int a){
    int count =0;
    int x,y;

    for(y=0;y<4;y++){
        for(x=0;x<4;x++){
            if(BLOCK[y][x] != BLOCK_F[a][y][x]){ //ブロックが一致していないとき
                count = 1;break;
            }
            if(count == 1) break;
        }
    }
    if(count == 0)return 0;
    if(count == 1)return 1;      
}


/*r_revolution*/
/*---------------------------------------------------*/
/*
  書式:int r-revolution(int BLOCK)
  機能:落下するブロックを90度右回転させる
  返り値:BLOCK
*/
void r_revolution()
{
    int i=0;//for loop

    for(i=0;i<19;i++){
        if(cell_check(i) == 0){
            if(i==0){
                cell_copy_c(i+1);
                break;
            }else if(i==1){
                cell_copy_c(i-1);
                break;
            }else if(i==2||i==3||i==4){
                cell_copy_c(i+1);
                break;
            }else if(i==5){
                cell_copy_c(i-3);
                break;
            }else if(i==6||i==7||i==8){
                cell_copy_c(i+1);
                break;
            }else if(i==9){
                cell_copy_c(i-3);
                break;
            }else if(i==10||i==11||i==12){
                cell_copy_c(i+1);
                break;
            }else if(i==13){
                cell_copy_c(i-3);
                break;
            }else if(i==14||i==16){
                cell_copy_c(i+1);
                break;
            }else if(i==15||i==17){
                cell_copy_c(i-1);
                break;
            }
        }
    }
}

/*l_revolution*/
/*-------------------------------------*/
/*
書式:int l_revolution(int BLOCK)
機能:落下するブロックを90度左回転させる
返値:BLOCK
*/
void l_revolution()
{
    int i;//for loop

    for(i=0;i<19;i++){
        if(cell_check(i) == 0){
            if(i==0){
                cell_copy_c(i+1);
                break;
            }else if(i==1){
                cell_copy_c(i-1);
                break;
            }else if(i==2){
                cell_copy_c(i+3);
                break;
            }else if(i==3||i==4||i==5){
                cell_copy_c(i-1);
                break;
            }else if(i==6){
                cell_copy_c(i+3);
                break;
            }else if(i==7||i==8||i==9){
                cell_copy_c(i-1);
                break;
            }else if(i==10){
                cell_copy_c(i+3);
                break;
            }else if(i==11||i==12||i==13){
                cell_copy_c(i-1);
                break;
            }else if(i==14||i==16){
                cell_copy_c(i+1);
                break;
            }else if(i==15||i==17){
                cell_copy_c(i-1);
                break;
            }
        }
    }
}



/* init_buff */
/*--------------------------------------------------------*/
/*
    書式: void init_buff()

    機能: ブロックの配列を初期化する

    返値: なし
*/
void init_buff()
{
    int i,j,k;
    // ブロックの初期化
    for(i=0;i<GAME_Y;i++){
        for(j=0;j<GAME_X;j++){
            cell[i][j] = 0;
            cell_t[i][j] =0;
        }
    }
    //ブロックの高さの初期化
    for(k=0;k<GAME_X;k++){
        block_tall[k] = 0;
    }
}
