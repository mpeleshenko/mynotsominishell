#include "mynotsominishell.h"

/* pre : keyboard key as a string
* post : Calls the corresponding function for the provided key
*/
char check_character(char *c)
{
	if (!my_strcmp(c, ENTER)) /* Handle ENTER */
	{
		// restore_terminal();
		// getout(0);
	}
	else if (!gl_env.flag)
	{
		if (!my_strncmp(c, SPACEBAR, 1)) /* Handle SPACEBAR */
		{
			// doselect();
		}
		else if (!my_strcmp(c, KU)) /* Handle UP arrow */
		{
			// moveup();
		}
		else if (!my_strcmp(c, KD)) /* Handle DOWN arrow */
		{
			// movedown();
		}
		else if (!my_strcmp(c, KL)) /* Handle LEFT arrow */
		{
			// moveleft();
		}
		else if (!my_strcmp(c, KR)) /* Handle RIGHT arrow */
		{
			// moveright();
		}
	}
}

/* pre : string
* post : converts the string to a vector, checks, and runs the command
*/
char check_command(char *str)
{
	char **vect;

	vect = my_str2vect(str);
	if(!vect)
		my_str("It is null\n");

	/* Exit shell */
	if(!my_strcmp(vect[0], "exit"))
	{
		restore_terminal();
		my_str("Thank you for using my shell!\n");
		exit(0);
	}
	/* Process cd commands */
	else if(!my_strcmp(vect[0], "cd"))
	{
		if(chdir(vect[1]) < 0)
			perror("cd");
		else
		{
			free(gl_env.curr_dir);
			gl_env.curr_dir = getcwd(NULL, 0); /* get current directory */
			gl_env.curr_dir_len = my_strlen(gl_env.curr_dir); /* set current directory length */
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
}