#include "mynotsominishell.h"

int main(int argc, char *argv[])
{
	unsigned int i, j, k;
	pid_t pid;
	int n;
	char buffer[BUF_SZ];
	char **vect;
	char *curr_dir = getcwd(NULL, 0);

	while(1)
	{
		/* Print shell prompt with current directory */
		my_str(curr_dir);
		my_str("$> ");

		/* Read input */
		n = read(0, buffer, BUF_SZ - 1);
		buffer[n - 1] = '\0';
		vect = my_str2vect(buffer);
		if(!vect)
			my_str("It is null\n");

		/* Exit shell */
		if(!my_strcmp(vect[0], "exit"))
		{
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
				free(curr_dir);
				curr_dir = getcwd(NULL, 0);
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
}