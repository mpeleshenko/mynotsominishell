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
	
	term_vi(); /* make cursor invisible */
	if (!my_strcmp(c, ENTER)) /* Handle ENTER */
	{
		moveend();
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
				deletechar();
			}
			else if (c[i] == CTRL_K) /* Handle CTRL-K */
			{
				/* cut from cursor position to end of line */
				cut_cmd();
			}
			else if (c[i] == CTRL_Y) /* Handle CTRL-Y */
			{
				/* paste what was cut */
				paste_cmd();
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
				addchar(c[i]);
			}
		}
	}
	term_ve(); /* make cursor visible */
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


	/* save command in history */
	save_command();

	/* reset curr_cmd pointer */
	gl_env.curr_cmd = &(gl_env.cmd_buffer);

	/* clear command buffer */
	gl_env.cmd_buffer.elem[0] = '\0';
	gl_env.cmd_buffer.size = 0;

	/* reset position */
	gl_env.pos = 0;
}

/* pre : character, index to place character in
* post : adds a character to the current command at the cursor
*/
char add_character(char c, unsigned int index)
{
	unsigned int i;
	char result = FALSE; /* whether character was added successfully */
	if (gl_env.curr_cmd->size < (BUF_SZ - 1) && index <= gl_env.curr_cmd->size) /* if buffer has room */
	{
		result = TRUE;
		for (i = gl_env.curr_cmd->size; i > index; i--) /* push back characters */
		{
			gl_env.curr_cmd->elem[i] = gl_env.curr_cmd->elem[i - 1];
		}
		gl_env.curr_cmd->elem[index] = c;
		gl_env.curr_cmd->size++;
		gl_env.curr_cmd->elem[gl_env.curr_cmd->size] = '\0'; /* terminate command string */
	}
	return result;
}

/* pre : index of character in current command
* post : removes character at given index, starting from 0
*/
void remove_character(unsigned int index)
{
	unsigned int i;
	if (gl_env.curr_cmd->size > 0 && index < gl_env.curr_cmd->size) /* if buffer is not empty */
	{
		/* remove character at index and fill gap with next characters */
		for (i = index + 1; i <= gl_env.curr_cmd->size; i++)
		{
			gl_env.curr_cmd->elem[i - 1] = gl_env.curr_cmd->elem[i];
		}
		gl_env.curr_cmd->size--;
	}
}