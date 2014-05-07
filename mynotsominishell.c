#include "mynotsominishell.h"

int main(int argc, char *argv[])
{
	unsigned int i, j, k;
	pid_t pid;
	int n;
	char buffer[READMIN + 1]];
	char **vect;
	gl_env.curr_dir = getcwd(NULL, 0);
	gl_env.curr_dir_len = my_strlen(gl_env.curr_dir); /* set current directory length */

	init_caps(); /* initialize termcaps */
	init_terminal(); /* setup termcaps */
	// setup_elems(argc - 1, &argv[1]);
	// signal(SIGWINCH, show_prompt);

	// show_prompt();

	while(1)
	{
		/* Print shell prompt with current directory */
		my_str(curr_dir);
		my_str("$> ");

		/* Read input */
		n = read(0, buffer, READMIN);
		buffer[n - 1] = '\0';
		check_character(buffer);
	}
}