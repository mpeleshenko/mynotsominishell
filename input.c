#include "mynotsominishell.h"

/* pre : keyboard key as a string
* post : Calls the corresponding function for the provided key
*/
void check_character(char *c)
{
	unsigned int i;
	if (!my_strcmp(c, ESC)) /* Handle ESC */
	{
		restore_terminal();
		getout();
	}
	if (!gl_env.flag) /* if terminal does not need resizing */
	{
		if (!my_strcmp(c, ENTER)) /* Handle ENTER */
		{
			my_str(c);
			check_command();
			print_prompt();
		}
		else if (!my_strcmp(c, KU)) /* Handle UP arrow */
		{
			moveup();
		}
		else if (!my_strcmp(c, KD)) /* Handle DOWN arrow */
		{
			movedown();
		}
		else if (!my_strcmp(c, KL)) /* Handle LEFT arrow */
		{
			moveleft();
		}
		else if (!my_strcmp(c, KR)) /* Handle RIGHT arrow */
		{
			moveright();
		}
		else /* Handle single character input */
		{
			for(i = 0; i < READMIN && c[i]; i++) /* iterate through buffer to get all possible characters */
			{
				if (c[i] == BACKSPACE) /* Handle BACKSPACE */
				{
					/* NOT DONE */
					remove_character(gl_env.curr_cmd->size - 1);
					moveleft();
					term_delete_char();
				}
				else if (c[i] == CTRL_K) /* Handle CTRL-K */
				{
					/* cut from cursor position to end of line */
				}
				else if (c[i] == CTRL_Y) /* Handle CTRL-Y */
				{
					/* paste what was cut */
				}
				else if (c[i] == CTRL_A) /* Handle CTRL-A */
				{
					/* move cursor to first character */
					movestart();
				}
				else if (c[i] == CTRL_E) /* Handle CTRL-E */
				{
					/* move cursor to the end */
					moveend();
				}
				else if (c[i] == CTRL_L) /* Handle CTRL-L */
				{
					/* clear screen and reshow line at top of terminal */
					show_prompt();
				}
				else if (c[i] >= ' ' && c[i] <= '~') /* Handle character input */
				{
					add_character(c[i]); /* add character to command buffer */
					my_char(c[i]);
					gl_env.pos++; /* increment position */
					gl_env.cursor_col = (gl_env.cursor_col + 1) % gl_env.win.ws_col; /* increment cursor column */
				}
			}
		}
	}
}

/* pre : nothing
* post : converts the command buffer to a vector, checks, and runs the command
*/
void check_command()
{
	unsigned int i;
	pid_t pid;
	char **vect;

	vect = my_str2vect(gl_env.curr_cmd->elem);
	if(!vect)
		my_str("It is null\n");

	/* Exit shell */
	if(!my_strcmp(vect[0], "exit"))
	{
		restore_terminal();
		getout();
	}
	/* Process cd commands */
	else if(!my_strcmp(vect[0], "cd"))
	{
		if(chdir(vect[1]) < 0)
			perror("cd");
		else
		{
			free(gl_env.curr_dir.elem);
			gl_env.curr_dir.elem = getcwd(NULL, 0); /* get current directory */
			gl_env.curr_dir.size = my_strlen(gl_env.curr_dir.elem); /* set current directory length */
		}
	}
	else
	{
		if((pid = fork()) < 0)
			perror("Fork Error");
		if(pid > 0) /* Parent */
		{
			wait(NULL);
		}
		else /* Child */
		{
			/* Attempt to execute command */
			if(execvp(vect[0], vect) < 0)
			{
				if(errno == ENOENT)
				{
					my_str(vect[0]);
					my_str(": Command not found\n");
				}
				else
				{
					perror("Command Error");
				}
			}
			exit(0);
		}
	}

	/* free vect */
	for(i = 0; vect[i]; i++)
	{
		free(vect[i]);
	}
	free(vect[i]);
	vect = NULL;


	/* save command in history NOT DONE*/

	/* reset curr_cmd pointer */
	gl_env.curr_cmd = &(gl_env.cmd_buffer);

	/* clear command buffer */
	gl_env.cmd_buffer.elem[0] = '\0';
	gl_env.cmd_buffer.size = 0;

	/* reset position */
	gl_env.pos = 0;
}

/* pre : character
* post : adds a character to the current command
*/
void add_character(char c)
{
	if (gl_env.curr_cmd->size < BUF_SZ - 1) /* if buffer has room */
	{
		gl_env.curr_cmd->elem[gl_env.curr_cmd->size] = c;
		gl_env.curr_cmd->size++;
		gl_env.curr_cmd->elem[gl_env.curr_cmd->size] = '\0'; /* terminate command string */
	}
}

/* pre : index of character in current command
* post : removes character at given index, starting from 0
*/
void remove_character(unsigned int index)
{
	if (gl_env.curr_cmd->size > 0 && index < gl_env.curr_cmd->size) /* if buffer is not empty */
	{
		if (index != gl_env.curr_cmd->size - 1) /* remove character at index */
		{
			/* need to implement this */
		}
		gl_env.curr_cmd->size--;
		gl_env.curr_cmd->elem[gl_env.curr_cmd->size] = '\0'; /* terminate command string */

	}
}