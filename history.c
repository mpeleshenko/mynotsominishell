#include "mynotsominishell.h"

/* pre : nothing
* post : loads commands from the .history file, if it exists, and sets nbelems in gl_env
*/
void load_history()
{
	unsigned int i, j;
	int fd;
	int n;
	static char buffer[BUF_SZ];

	if ((fd = open(".history", O_RDONLY)) > 0) /* if file could be opened */
	{
		for(i = 0; i < HISTORY_LIMIT; i++)
		{
			for (j = 0; j < (BUF_SZ - 1);)
			{
				n = read(fd, &buffer[j], 1);
				if (n <= 0) /* if reached end of file */
				{
					if (j > 0) /* if something was read before */
					{
						buffer[j] = '\0';
						j++;
					}
					break;
				}
				else
				{
					if (buffer[j] == '\r' || buffer[j] == '\n') /* if found carriage return or newline */
					{
						if (j != 0) /* if characters were read before, save as command */
						{
							buffer[j] = '\0';
							j++;
							break;
						}
					}
					else
					{
						j++;
					}
				}
			}
			if (j && buffer[j - 1] != '\0')
			{
				buffer[j] = '\0';
				j++;
			}
			if (j && buffer[j - 1] != '\r' && buffer[j - 1] != '\n')
			{
				gl_env.elements[i].elem = my_strdup(buffer);
				gl_env.elements[i].size = (j - 1);
				gl_env.nbelems++;
			}
			else
			{
				break;
			}
		}
		close(fd);
	}
}

/* pre : nothing
* post : saves command history in gl_env.elements to .history file
*/
void save_history()
{
	unsigned int i;
	unsigned int elems = gl_env.nbelems;
	int fd;
	int n;

	if ((fd = open(".history", O_WRONLY | O_TRUNC)) > 0) /* open file for writing and truncate it */
	{
		for(i = gl_env.elem_last; elems > 0; elems--, i = ++i % gl_env.nbelems)
		{
			if ((n = write(fd, gl_env.elements[i].elem, gl_env.elements[i].size)) > 0)
			{
				if ((n = write(fd, "\n", 1)) < 0)
				{
					my_str("ERROR: Failed to write new line.\n");
				}
			}
			else
			{
				my_str("ERROR: Failed to write command to file.\n");
			}
		}
		close(fd);
	}
	else
	{
		my_str("ERROR: Could not save command history to file.\n");
	}
}


/* pre : nothing
* post : saves current command to history if it is not the same as the most recent one
*/
void save_command()
{
	if (my_strcmp(gl_env.curr_cmd->elem, gl_env.elements[gl_env.elem_first].elem)) /* if not most recent command was selected */
	{
		gl_env.elem_first = ++gl_env.elem_first % HISTORY_LIMIT; /* increment most recent command index */
		if (gl_env.nbelems == HISTORY_LIMIT) /* if history at capacity */
		{
			gl_env.elem_last = ++gl_env.elem_last % HISTORY_LIMIT; /* increment last command pointer */

			/* replace most recent command with current command */
			free(gl_env.elements[gl_env.elem_first].elem);
			gl_env.elements[gl_env.elem_first].elem = my_strdup(gl_env.curr_cmd->elem);
			gl_env.elements[gl_env.elem_first].size = gl_env.curr_cmd->size;
		}
		else /* if history not at capacity */
		{
			/* replace most recent command with current command */
			gl_env.elements[gl_env.elem_first].elem = my_strdup(gl_env.curr_cmd->elem);
			gl_env.elements[gl_env.elem_first].size = gl_env.curr_cmd->size;
			gl_env.nbelems++;
		}
	}
}