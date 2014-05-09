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
	unsigned int i;
	unsigned int saved_pos;

	get_win_size();
	term_clear();

	print_prompt();

	saved_pos = gl_env.pos;
	gl_env.pos = 0;
	print_cmd(0);

	for (i = gl_env.pos; i > saved_pos; i--)
	{
		moveleft();
	}
}

/* pre : nothing
* post : prints current command on terminal from cursor position
*/
void refreshin()
{
	print_cmd(gl_env.pos);
}

/* pre : nothing
* post : clears current command on the terminal
*/
void refreshout()
{
	unsigned int i;
	moveend(); /* move to last character of command */
	for (i = 0; i < gl_env.curr_cmd->size; i++)
	{
		moveleft();
		term_delete_char();
	}
	gl_env.cursor_col = ((gl_env.curr_dir.size + 4) - 1) % gl_env.win.ws_col;
	gl_env.pos = 0;
}

/* pre : nothing
* post : Moves to the previous command in history, if it exists
*/
void moveup()
{
	if (gl_env.curr_cmd == &gl_env.cmd_buffer && gl_env.nbelems > 0) /* if history is not empty */
	{
		refreshout();
		gl_env.curr_elem = gl_env.elem_first;

		/* copy command from history int hst_buffer */
		my_strcpy(gl_env.hst_buffer.elem, gl_env.elements[gl_env.curr_elem].elem);
		gl_env.hst_buffer.size = gl_env.elements[gl_env.curr_elem].size;
		gl_env.curr_cmd = &(gl_env.hst_buffer);
		refreshin();
	}
	else /* currently in history */
	{
		if (gl_env.curr_elem != gl_env.elem_last) /* if not reached end of history */
		{
			if (gl_env.curr_elem == 0)
			{
				gl_env.curr_elem = gl_env.nbelems - 1;
			}
			else
			{
				gl_env.curr_elem--;
			}
			refreshout();
			/* copy command from history int hst_buffer */
			my_strcpy(gl_env.hst_buffer.elem, gl_env.elements[gl_env.curr_elem].elem);
			gl_env.hst_buffer.size = gl_env.elements[gl_env.curr_elem].size;
			refreshin();
		}
	}
}

/* pre : nothing
* post : Moves to more recent item in history up to the current command
*/
void movedown()
{
	if (gl_env.curr_cmd != &gl_env.cmd_buffer) /* if not at current command */
	{
		refreshout();
		if (gl_env.curr_elem == gl_env.elem_first) /* if at most recent command in history */
		{
			/* switch to command buffer */
			gl_env.curr_cmd = &(gl_env.cmd_buffer);
		}
		else
		{
			if (gl_env.curr_elem == gl_env.nbelems - 1) /* if at end of array */
			{
				gl_env.curr_elem = 0;
			}
			else
			{
				gl_env.curr_elem++;
			}
			/* copy command from history int hst_buffer */
			my_strcpy(gl_env.hst_buffer.elem, gl_env.elements[gl_env.curr_elem].elem);
			gl_env.hst_buffer.size = gl_env.elements[gl_env.curr_elem].size;
			// gl_env.curr_cmd = &(gl_env.hst_buffer);
			// gl_env.curr_cmd = &(gl_env.elements[gl_env.curr_elem]);
		}
		refreshin();
	}
}

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
		if (gl_env.cursor_col == (gl_env.win.ws_col - 1)) /* move to start of next line */
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
* post : Moves cursor to the first character of the command
*/
void movestart()
{
	unsigned int i;

	for (i = gl_env.pos; i > 0; i--)
	{
		moveleft();
	}
}

/* pre : nothing
* post : Moves cursor to the last character of the command
*/
void moveend()
{
	unsigned int i;

	for (i = gl_env.pos; i < gl_env.curr_cmd->size; i++)
	{
		moveright();
	}
}

/* pre : nothing
* post : inserts character at cursor and reprints command up to the end
*        also modifies current command buffer
*/
void addchar(char c)
{
	unsigned int i;
	unsigned int saved_pos;

	if (add_character(c, gl_env.pos)) /* add character to command buffer */
	{
		my_char(c);
		gl_env.pos++; /* increment position */
		gl_env.cursor_col = (gl_env.cursor_col + 1) % gl_env.win.ws_col; /* increment cursor column */
		if (gl_env.cursor_col == 0) /* move cursor to next row when needed */
		{
			term_move_down();
			term_move_start();
		}

		saved_pos = gl_env.pos;
		print_cmd(gl_env.pos);

		for (i = gl_env.pos - saved_pos; i > 0; i--) /* move cursor back to old position */
		{
			moveleft();
		}
	}
}

/* pre : nothing
* post : deletes character before cursor and reprints command up to the end, deleting the last character
*        also modifies current command buffer
*/
void deletechar()
{
	unsigned int i;
	unsigned int saved_pos;

	remove_character(gl_env.pos - 1); /* remove character before cursor from command buffer */
	moveleft();
	term_delete_char();
	saved_pos = gl_env.pos;
	print_cmd(gl_env.pos);
	term_delete_char();

	for (i = gl_env.pos - saved_pos; i > 0; i--) /* move cursor back to old position */
	{
		moveleft();
	}
}

/* pre : nothing
* post : copies characters from cursor into copy buffer
*        and deletes those characters from the current command buffer and terminal
*/
void cut_cmd()
{
	unsigned int i, j;

	if (gl_env.pos != gl_env.curr_cmd->size)
	{
		/* copy characters from cursor into copy buffer */
		gl_env.cpy_buffer.size = gl_env.curr_cmd->size - gl_env.pos;
		for (i = 0, j = gl_env.pos; i <= gl_env.cpy_buffer.size; i++, j++)
		{
			gl_env.cpy_buffer.elem[i] = gl_env.curr_cmd->elem[j];
		}
		// memcpy(gl_env.cpy_buffer.elem, &(gl_env.curr_cmd->elem[gl_env.pos]), gl_env.cpy_buffer.size + 1);

		/* update terminal display */
		moveend();
		for (i = 0; i < gl_env.cpy_buffer.size; i++)
		{
			deletechar();
		}

		/* update command that was cut from */
		gl_env.curr_cmd->elem[gl_env.curr_cmd->size] = '\0'; /* termintate command that was cut from */
	}
}

/* pre : nothing
* post : pastes cut command at cursor
*        and adds those characters to the current command buffer
*/
void paste_cmd()
{
	unsigned int i;

	if (gl_env.cpy_buffer.size) /* if copy buffer is not empty */
	{
		for (i = 0; i < gl_env.cpy_buffer.size; i++)
		{
			addchar(gl_env.cpy_buffer.elem[i]);
		}
	}
}

/* pre : nothing
* post : prints shell prompt with current directory
*/
void print_prompt()
{
	my_str(gl_env.curr_dir.elem);
	my_str("$> ");
	gl_env.cursor_col = ((gl_env.curr_dir.size + 4) - 1) % gl_env.win.ws_col;
}

/* pre : index
* post : prints the current command from the given index
*        and increments the cursor position accordingly
*/
void print_cmd(unsigned int index)
{
	if (index < gl_env.curr_cmd->size)
	{
		my_str(&(gl_env.curr_cmd->elem[index]));
		gl_env.cursor_col = (gl_env.cursor_col + (gl_env.curr_cmd->size - index)) % gl_env.win.ws_col;
		if (gl_env.cursor_col == 0) /* move cursor to next row when needed */
		{
			term_move_down();
			term_move_start();
		}	
		gl_env.pos = gl_env.pos + (gl_env.curr_cmd->size - index);
	}
}