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
* post : shows the elements on the terminal
*        prints a message if pompt and command cannot fit in the terminal
*/
void show_prompt()
{
	unsigned int max, i, x, y;

	get_win_size();
	gl_env.flag = 0;
	term_clear();

	/* Print each element */
	for(i = x = y = max = 0; i < gl_env.nbelems; i++, y++)
	{
		if (y >= gl_env.win.ws_row) /* if all rows filled, move to next column */
		{
			y = 0;
			x += max + 3;
			max = 0;
		}
		if (max < gl_env.elements[i].size) /* if found new max length element, set new max length */
			max = gl_env.elements[i].size;
		if (max + x > gl_env.win.ws_col) /* if current element cannot fit in terminal */
		{
			term_clear();
			my_str("Please enlarge the window");
			gl_env.flag = 1;
			return;
		}

		/* store coordinates and print element */
		gl_env.elements[i].x = x;
		gl_env.elements[i].y = y;
		refreshout(i);
	}
	refreshin(); /* underline previously selected element */
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
* post : Moves to the item on same row, but previous column
*        Does not wrap around
*/
// void moveleft()
// {
// 	refreshout(gl_env.pos);
// 	/* if underline is currently in first column, go to first element */
// 	if (gl_env.elements[gl_env.pos].x == 0)
// 		term_move_to_item(0);
// 	else
// 		term_move_to_item(gl_env.pos - gl_env.win.ws_row);
// 	refreshin();
// }

/* pre : nothing
* post : Moves to the item on the same row, but next column
*        Does not wrap around
*/
// void moveright()
// {
// 	unsigned int next_pos = gl_env.pos + gl_env.win.ws_row;
// 	refreshout(gl_env.pos);
// 	/* if underline is currently in last column or there is no element directly to the right, go to last element */
// 	if ((gl_env.elements[gl_env.pos].x == gl_env.elements[gl_env.nbelems - 1].x) || (next_pos > (gl_env.nbelems - 1)))
// 		term_move_to_item(gl_env.nbelems - 1);
// 	else
// 		term_move_to_item(next_pos);
// 	refreshin();
// }

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