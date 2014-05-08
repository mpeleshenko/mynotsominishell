#include "mynotsominishell.h"

/* pre : character
* post : Prints character to the terminal
*/
int	my_termprint(int c)
{
	return write(1, &c, 1);
}

/* pre : nothing
* post : gets the current window size and stores it in gl_env
*/
char get_win_size()
{
	ioctl(0, TIOCGWINSZ, &(gl_env.win));
}

/* pre : nothing
* post : shows the shell prompt and the last command
*/
void show_prompt()
{
	unsigned int max, i, x, y;

	get_win_size();
	gl_env.flag = 0;
	gl_env.cursor_col = ((gl_env.curr_dir.size + 4) - 1) % gl_env.win.ws_col;
	term_clear();

	print_prompt();
	print_cmd();
}

/* pre : nothing
* post : Reprints the currently selected element as it was with an underline
*/
// void refreshin()
// {
// 	term_move_to_item(gl_env.pos);
// 	term_underline(); /* turn on onderline */
// 	if (gl_env.elements[gl_env.pos].mode)  /* if element was highlighted */
// 		term_standout();
// 	my_str(gl_env.elements[gl_env.pos].elem); /* print element */
// 	term_standend();
// 	term_underend(); /* turn off underline */
// }

/* pre : element number
* post : Reprints the element as it was with the underline removed
*/
// void refreshout(int n)
// {
// 	term_move_to_item(n);
// 	if (gl_env.elements[n].mode)  /* if element was highlighted */
// 		term_standout();
// 	my_str(gl_env.elements[n].elem); /* print element */
// 	term_standend();
// }

/* pre : nothing
* post : Moves to the previous item, wraps around if necessary
*/
// void moveup()
// {
// 	refreshout(gl_env.pos);
// 	/* if underline is currently on first element, wrap around */
// 	if (gl_env.pos == 0)
// 		term_move_to_item(gl_env.nbelems - 1);
// 	else
// 		term_move_to_item(gl_env.pos - 1);
// 	refreshin();
// }

/* pre : nothing
* post : Moves to the next item, wraps around if necessary
*/
// void movedown()
// {
// 	refreshout(gl_env.pos);
// 	/* if underline is currently on last element, wrap around */
// 	if (gl_env.pos == gl_env.nbelems - 1)
// 		term_move_to_item(0);
// 	else
// 		term_move_to_item(gl_env.pos + 1);
// 	refreshin();
// }

/* pre : nothing
* post : Moves cursor to the left up to the start of the prompt
*/
void moveleft()
{
	if (gl_env.pos > 0)
	{
		if (gl_env.cursor_col == 0)
		{
			gl_env.cursor_col = gl_env.win.ws_col - 1;
		}
		else
		{
			gl_env.cursor_col--;
		}
		term_move_left(); /* move cursor left */
		gl_env.pos--;
	}
}

/* pre : nothing
* post : Moves cursor to the right up to the end of the command
*/
void moveright()
{
	if (gl_env.pos < gl_env.curr_cmd->size)
	{
		if (gl_env.cursor_col == (gl_env.win.ws_col - 1))
		{
			term_move_down();
			term_move_start();
			gl_env.cursor_col = 0;
		}
		else
		{
			term_move_right(); /* move cursor right */
			gl_env.cursor_col++;
		}
		gl_env.pos++;
	}
}

/* pre : nothing
* post : Highlights the currently selected item, if not highlighted
*        and moves to the next item, wrapping around if necessary
*        If the item is already highlighted, just removes the highlighting
*/
// void doselect()
// {
// 	if (gl_env.elements[gl_env.pos].mode) /* if selected element is already highlighted */
// 	{
// 		gl_env.elements[gl_env.pos].mode = 0;
// 		refreshin();
// 	}
// 	else
// 	{
// 		gl_env.elements[gl_env.pos].mode = 1;
// 		refreshin();
// 		movedown();
// 	}
// }

/* pre : nothing
* post : prints shell prompt with current directory
*/
void print_prompt()
{
	my_str(gl_env.curr_dir.elem);
	my_str("$> ");
}

/* pre : nothing
* post : prints the last command
*/
void print_cmd()
{
	my_str(gl_env.curr_cmd->elem);
}