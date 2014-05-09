#include "mynotsominishell.h"

/* pre : nothing
* post : Backs up the current terminal configuration and sets up
*        the terminal for noncanonical mode, and signals
*        and change fd 1 to TERM
*/
void init_terminal()
{
	struct termio line;

	int fd;
	char *name;

	ioctl(0, TCGETA, &line); /* Get current terminal configs */

	gl_env.line_backup = line; /* Backup terminal configs */

	line.c_cc[VMIN] = READMIN; 
	line.c_cc[VTIME] = READTIME;
	line.c_lflag &= ~(ICANON | ECHO | ISIG); /* Set terminal to noncanonical and disable echo and signals */
	ioctl(0, TCSETA, &line); /* apply changes to terminal config */

	/* Change STDOUT I/O to TERM */
	name = ttyname(0);
	fd = open(name, O_WRONLY);
	gl_env.stdio_backup = dup(1); /* Save STDOUT */
	dup2(fd, 1); /* 1 now points to TERM */

}

/* pre : terminal configs were backed up
* post : Restores backed up terminal configuration
*/
void restore_terminal()
{
	term_clear();
	term_ve(); /* make cursor visible */
	ioctl(0, TCSETA, &(gl_env.line_backup)); /* restore terminal configs */	
	dup2(gl_env.stdio_backup, 1);
}

/* pre : capability string
* post : Returns the actual terminal's capability string
*/
char *term_get_cap(char *cap)
{
	char *str;
	static char area[2048];

	if(!(str = tgetstr(cap, (char **)&area)))
	{
		restore_terminal();
		my_panic("Could not get term cap.\n");
	}
	return str;
}

/* pre : nothing
* post : Initializes terminal capabilities
*/
void init_caps()
{
	char *term;
	char bp[2048];

	if(!(term = getenv("TERM"))) // Get terminal capabilities
	{
		restore_terminal();
		my_panic("Could not get TERM environment variable.\n");
	}
	tgetent(bp, term);

	gl_env.clear = term_get_cap(CLEAR);
	gl_env.delete_char = term_get_cap(DEL_CHAR);
	gl_env.move_left = term_get_cap(MOVE_LEFT);
	gl_env.move_right = term_get_cap(MOVE_RIGHT);
	gl_env.move_down = term_get_cap(MOVE_DOWN);
	gl_env.move_start = term_get_cap(MOVE_START);
	gl_env.cursoroff = term_get_cap(CURSOROFF);
	gl_env.cursoron = term_get_cap(CURSORON);
}

/* pre : term caps have been configured
* post : clears the terminal
*/
void term_clear()
{
	tputs(gl_env.clear, 1, my_termprint);
}

/* pre : term caps have been configured
* post : deletes a character at the cursor
*/
void term_delete_char()
{
	tputs(gl_env.delete_char, 1, my_termprint);
}

/* pre : term caps have been configured
* post : moves cursor to the left
*/
void term_move_left()
{
	tputs(gl_env.move_left, 1, my_termprint);
}

/* pre : term caps have been configured
* post : moves cursor to the right
*/
void term_move_right()
{
	tputs(gl_env.move_right, 1, my_termprint);
}

/* pre : term caps have been configured
* post : moves cursor down
*/
void term_move_down()
{
	tputs(gl_env.move_down, 1, my_termprint);
}

/* pre : term caps have been configured
* post : moves cursor to the start of the current line
*/
void term_move_start()
{
	tputs(gl_env.move_start, 1, my_termprint);
}

/* pre : term caps have been configured
* post : turn cursor off in terminal
*/
void term_vi()
{
	tputs(gl_env.cursoroff, 1, my_termprint);
}

/* pre : term caps have been configured
* post : turn cursor on in terminal
*/
void term_ve()
{
	tputs(gl_env.cursoron, 1, my_termprint);
}

/* pre : terminal has been restored
* post : Prints a goodbye message and exits
*/
void getout()
{
	save_history(); /* save command history to file */
	my_str("Thank you for using my shell!\n");
	exit(0);
}

/* pre : nothing
* post : Initializes gl_env strings
*/
void setup_env()
{
	unsigned int i;
	int fd;

	gl_env.nbelems = 0; /* Set number of commands in history */
	gl_env.elem_last = 0;
	gl_env.elem_first = 0;
	gl_env.elements = (t_elem *) xmalloc(HISTORY_LIMIT * sizeof(*(gl_env.elements)));

	gl_env.pos = 0;

	set_curr_dir();

	/* init command buffer */
	gl_env.cmd_buffer.elem = (char *) xmalloc(BUF_SZ * sizeof(*(gl_env.cmd_buffer.elem))); /* allocate for command buffer */
	gl_env.cmd_buffer.size = 0; /* init command length */
	gl_env.cmd_buffer.elem[0] = '\0';
	gl_env.curr_cmd = &gl_env.cmd_buffer;

	/* init copy buffer */
	gl_env.cpy_buffer.elem = (char *) xmalloc(BUF_SZ * sizeof(*(gl_env.cpy_buffer.elem))); /* allocate for copy buffer */
	gl_env.cpy_buffer.size = 0; /* init copy command length */
	gl_env.cpy_buffer.elem[0] = '\0';

	/* init history buffer */
	gl_env.hst_buffer.elem = (char *) xmalloc(BUF_SZ * sizeof(*(gl_env.hst_buffer.elem))); /* allocate for history buffer */
	gl_env.hst_buffer.size = 0; /* init copy command length */
	gl_env.hst_buffer.elem[0] = '\0';

	/* Load history, if it exists */
	load_history();
	if (gl_env.nbelems > 0)
	{
		gl_env.elem_first = gl_env.nbelems - 1; /* set most recent command index */
	}
}

/* pre : nothing
* post : sets the current directory in gl_env
*/
void set_curr_dir()
{
	gl_env.curr_dir.elem = getcwd(NULL, 0); /* get current directory */
	gl_env.curr_dir.size = my_strlen(gl_env.curr_dir.elem); /* set current directory length */
}