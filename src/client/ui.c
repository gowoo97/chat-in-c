#include<ncurses.h> 
 
#define WORLD_WIDTH 30
#define WORLD_HEIGHT 10
 
int main(int argc, char *argv[]) {
    char msg[100]; 
    WINDOW *w;
    int offsetx, offsety;
 
    initscr();
    refresh();
 
    offsetx = (COLS - WORLD_WIDTH) / 2;
    offsety = (LINES - WORLD_HEIGHT) / 2;
 
    w =	newwin(WORLD_HEIGHT,
                           WORLD_WIDTH,
                           offsety,
                           offsetx);
 
    box(w, 0 , 0);
    
     

    wrefresh(w);
	
    getstr(msg); 
 
    getch();
 
    delwin(w);
 
    endwin();
 
    return 0;
 
}
