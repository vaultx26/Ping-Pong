#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#define _DEFAULT_SOURCE
void draw_logo();
void menu(FILE*);
void start_game(FILE*);
int main(int argc , char* argv[]) {
    srand(time(NULL));
    initscr();
    noecho();
    start_color();
    curs_set(0);
    keypad(stdscr , true);
    FILE* fp = fopen(argv[1],"w");
    draw_logo();
    menu(fp);
    endwin();
    fclose(fp);
    return 0;
}

void draw_logo() {                  // Stiahol som to z curses-example.c
    clear();
    int usleep();
    char logo[8][66] = {
    {"                                                                 "},
    {" _______ ___ __    _ _______     _______ _______ __    _ _______ "},
    {"|       |   |  |  | |       |   |       |       |  |  | |       |"},
    {"|    _  |   |   |_| |    ___|___|    _  |   _   |   |_| |    ___|"},
    {"|   |_| |   |       |   | _|____|   |_| |  | |  |       |   | __ "},
    {"|    ___|   |  _    |   ||  |   |    ___|  |_|  |  _    |   ||  |"},
    {"|   |   |   | | |   |   |_| |   |   |   |       | | |   |   |_| |"},
    {"|___|   |___|_|  |__|_______|   |___|   |_______|_|  |__|_______|"}
    };
    int center = COLS/2 - strlen(logo[0])/2;
    int target = 15;
    for( int row_count = 7; row_count >= 1; row_count-- ){

        for( int y = 1; y <= target; y++ ){
            move(y, center);
            printw("%s", logo[row_count]);
            refresh();
            usleep(3 * 10000 );
            move(y,center);
            printw(logo[0]);
        }
        move(target,center);
        printw(logo[row_count]);
        target--;
    }
    sleep(3);
    clear();
}

void menu(FILE* fp) {
    int chose;
    
    mvprintw(6, 70 , " 1. Start game ") ;
    mvprintw(8, 70 , " 2. Instruction for game") ;
    mvprintw(10, 70, " Your selection: ");
    refresh();
    do{
        scanw("%d", &chose);
        refresh();
    }while(chose < 1 || chose > 2);
    switch (chose) {
        case 1 : start_game(fp); break;
        case 2 : clear();
        mvprintw(6, 70 , " To control left player use 'w' and 's' ") ;
        mvprintw(8, 70 , " To control right player use 'o' and 'l' ") ;
        mvprintw(10, 70, " Have fun! ");
        mvprintw(12, 70, "\t\tPress any key to back");
        getch();
        clear();
        menu(fp);
    }
}
void start_game(FILE* fp) {
    clear();
    init_pair(1,COLOR_BLUE,COLOR_BLACK);
    int usleep();
    typedef struct {
        int x , y ;
    } wind;
    typedef struct {
        int x , y , count ;
    } ping;
    wind console;
    getmaxyx(stdscr , console.y , console.x);
    ping player = {0, console.y/2, 0};
    ping player2 = {console.x-1, console.y/2, 0};
    ping ball = {console.x/2 , console.y/2 , 0};
    bool finish = false;
    int max_y = 0 , max_x = 0;
    int next_y = 0, next_x = 0;
    int direction_y = 1, direction_x = 1;
    int timer = 70000;
    getmaxyx(stdscr, max_y, max_x);
    while(finish != true) {
        nodelay(stdscr,1);
        mvprintw(ball.y,ball.x, "O");
        refresh();
        
        next_x = ball.x + direction_x;
        next_y = ball.y + direction_y;
        if (next_x > max_x || next_x < 0) {
            direction_x *= -1;
        }
        if (next_y > max_y || next_y < 0) {
            direction_y *= -1;
        }
        usleep(timer);
        timer -= 10;
        ball.x += direction_x;
        ball.y += direction_y;
        
        if(ball.x == max_x) {
            if(ball.y != player2.y && ball.y != player2.y+1 && ball.y != player2.y-1) {
                player.count++;
                ball.x = console.x/2;
                ball.y = console.y/2;
            } 
        } else if(ball.x == 0) {
            if(ball.y != player.y && ball.y != player.y+1 && ball.y != player.y-1) {
                player2.count++;
                ball.x = console.x/2;
                ball.y = console.y/2;
            } 
        }

        erase();
        mvprintw(2,console.x-5,"%d",player2.count);
        mvprintw(2 , 5 , "%d ",player.count);
        mvvline(0,console.x/2,'|',console.y);
        attron(COLOR_PAIR(1));
        for(int i = -1 ; i < 2 ; i++) {
            mvprintw(player.y+i,player.x,"|");
            mvprintw(player2.y+i,player2.x,"|");
        }
        attroff(COLOR_PAIR(1));
            switch (getch()) {
                case 'w': player.y--; break;
                case 's': player.y++; break;
                case 'o': player2.y--; break;
                case 'l': player2.y++; break;
                case 0x1B: finish = true; break;
            }
            if(player.y <= 1) {
                player.y = console.y-2;
            }
            if(player.y >= console.y-1) {
                player.y = 2;
            }
            if(player2.y<=1) {
                player2.y=console.y-2;
            }
            if(player2.y>=console.y-1) {
                player2.y=2;
            }
            if(finish == true) {
                fprintf(fp,"Player 1 = %d\nPlayer 2 = %d ",player.count , player2.count);
                
            }
        }
} 