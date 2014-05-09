#include "mynotsominishell.h"

/* pre : nothing
* post : loads commands from the .history file, if it exists, and sets nbelems in gl_env
*/
void load_history()
{
	unsigned int i, j;
	int fd;
	int n;
	char buffer[BUF_SZ];

	if ((fd = open(".history", O_RDONLY)) > 0) /* if file could be opened */
	{
		for(i = 0; i < HISTORY_LIMIT; i++)
		{
			for (j = 0; j < BUF_SZ - 1;)
			{
				n = read(fd, &buffer[j], 1);
				if (n <= 0)
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
					j++;
					if (buffer[j - 1] == '\r') /* if found carriage return */
					{
						buffer[j - 1] = '\0';
						break;
					}
					if (buffer[j - 1] == '\n') /* if found new line */
					{
						if (j == 1) /* handles Windows line endings */
						{
							j--;
						}
						else /* reached end of line */
						{
							buffer[j - 1] = '\0';
							break;
						}
					}
				}
			}
			if (j && buffer[j - 1] != '\n')
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
	char buffer[BUF_SZ];

	if ((fd = open(".history", O_WRONLY)) > 0) /* if file could be opened */
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
* post : loads commands from the .history file, if it exists, and sets nbelems in gl_env
*/
// void save_command()
// {
// 	unsigned int i;
// 	int fd;
// 	int n;
// 	char buffer[BUF_SZ];

// 	if ((fd = open(".history", O_WRONLY)) > 0) /* if file could be opened */
// 	{
// 		for(i = 0; i < HISTORY_LIMIT && ((n = read(fd, buffer, BUF_SZ - 1)) > 0); i++)
// 		{
// 			buffer[n] = '\0';
// 			gl_env.elements[i].elem = my_strdup(buffer);
// 			gl_env.elements[i].size = (n - 1);
// 			gl_env.nbelems++;
// 		}
// 	}
// }