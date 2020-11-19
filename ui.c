#include<ncurses.h> 
 
#define WORLD_WIDTH 30
#define WORLD_HEIGHT 10
 
int main(int argc, char *argv[]) {
    char msg[100]; 
    WINDOW *snakeys_world;
    int offsetx, offsety;
 
    initscr();
    refresh();
 
    offsetx = (COLS - WORLD_WIDTH) / 2;
    offsety = (LINES - WORLD_HEIGHT) / 2;
 
    snakeys_world = newwin(WORLD_HEIGHT,
                           WORLD_WIDTH,
                           offsety,
                           offsetx);
 
    box(snakeys_world, 0 , 0);
    
     

    wrefresh(snakeys_world);
 mvprintw(1,1,"hello");
    getstr(msg); 
 
    getch();
 
    delwin(snakeys_world);
 
    endwin();
 
    return 0;
 
}
