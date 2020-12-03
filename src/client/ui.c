#include <form.h>
#include<ncurses.h>

#define WIDTH 30
#define HEIGHT 10


int max_y,max_x;
int cur_y,cur_x;
int main()
{
	FIELD *field[3];
	FORM  *my_form;
	int ch;
	
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	
	getmaxyx(stdscr,max_y,max_x);
	
	field[0] = new_field(1, 10, (max_y-HEIGHT)/2,(max_x-WIDTH)/2, 0, 0);
	field[1] = new_field(1, 10,((max_y-HEIGHT)/2)-1,(max_x-WIDTH)/2, 0, 0);
	field[2] = NULL;

	set_field_back(field[0], A_UNDERLINE);
	field_opts_off(field[0], O_AUTOSKIP);  
	set_field_back(field[1], A_UNDERLINE); 
	field_opts_off(field[1], O_AUTOSKIP);

	my_form = new_form(field);
	post_form(my_form);
	refresh();
	
	mvprintw(((max_y-HEIGHT)/2)-1, ((max_x-WIDTH)/2)-10, "ID:");
	mvprintw(((max_y-HEIGHT)/2),((max_x-WIDTH)/2)-10, "PASSWORD:");
	refresh();

	while((ch = getch()) != '\n')
	{	switch(ch)
		{	case KEY_DOWN:
				form_driver(my_form, REQ_NEXT_FIELD);
				form_driver(my_form, REQ_END_LINE);
				break;
			case KEY_UP:
				form_driver(my_form, REQ_PREV_FIELD);
				form_driver(my_form, REQ_END_LINE);
				break;
			case KEY_BACKSPACE:
				form_driver(my_form,REQ_DEL_PREV);
				break;
				
			default:
				form_driver(my_form, ch);
				break;
		}
	}

	mvprintw(1,1,"%s",field[0]);
	unpost_form(my_form);
	free_form(my_form);
	getch();
	free_field(field[0]);
	free_field(field[1]); 

	endwin();
	return 0;
}
