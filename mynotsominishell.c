#include "mynotsominishell.h"

int main(int argc, char *argv[])
{
	unsigned int i, j, k;
	pid_t pid;
	int n;
	char buffer[READMIN + 1];
	char **vect;
	set_curr_dir();

	init_caps(); /* initialize termcaps */
	init_terminal(); /* setup termcaps */
	setup_env(); /* init gl_env */
	signal(SIGWINCH, show_prompt);

	// show_prompt();
	show_prompt();
	while(1)
	{
		/* Read input */
		if ((n = read(0, buffer, READMIN)) < 0)
			{
				restore_terminal();
				my_panic("Failed to read!\n");
			}
		buffer[n] = '\0';
		check_character(buffer);
	}
}