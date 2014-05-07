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
	line.c_lflag &= ~(ICANON | /*ECHO |*/ ISIG); /* Set terminal to noncanonical and disable echo and signals */
	ioctl(0, TCSETA, &line); /* apply changes to terminal config */

	/* Change STDOUT I/O to TERM */
	name = ttyname(0);
	fd = open(name, O_WRONLY);
	gl_env.stdio_backup = dup(1); /* Save STDOUT */
	dup2(fd, 1); /* 1 now points to TERM */

	// term_vi(); /* Make cursor invisible */
}

/* pre : terminal configs were backed up
* post : Restores backed up terminal configuration
*/
void restore_terminal()
{
	term_clear();
	// term_ve(); /* Make cursor visible */
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

	// gl_env.left = term_get_cap(LEFT);
	// gl_env.right = term_get_cap(RIGHT);
	// gl_env.up = term_get_cap(UP);
	// gl_env.down = term_get_cap(DOWN);
	// gl_env.esc = term_get_cap("\E");
	// gl_env.underline = term_get_cap(UNDERLINE);
	// gl_env.under_end = term_get_cap(UNDER_END);
	// gl_env.standout = term_get_cap(STANDOUT);
	// gl_env.stand_end = term_get_cap(STAND_END);
	gl_env.move = term_get_cap(MOVE);
	gl_env.clear = term_get_cap(CLEAR);
	// gl_env.cursoroff = term_get_cap(CURSOROFF);
	// gl_env.cursoron = term_get_cap(CURSORON);
}

/* pre : term caps have been configured
* post : clears the terminal
*/
void term_clear()
{
	tputs(gl_env.clear, 1, my_termprint);
}

/* pre : term caps have been configured
* post : turn cursor off in terminal
*/
// void term_vi()
// {
// 	tputs(gl_env.cursoroff, 1, my_termprint);
// }

/* pre : term caps have been configured
* post : turn cursor on in terminal
*/
// void term_ve()
// {
// 	tputs(gl_env.cursoron, 1, my_termprint);
// }

/* pre : term caps have been configured
* post : turns underline on in terminal
*/
// void term_underline()
// {
// 	tputs(gl_env.underline, 1, my_termprint);
// }

/* pre : term caps have been configured
* post : turns underline off in terminal
*/
// void term_underend()
// {
// 	tputs(gl_env.under_end, 1, my_termprint);
// }

/* pre : term caps have been configured
* post : turns highlight on in terminal
*/
// void term_standout()
// {
// 	tputs(gl_env.standout, 1, my_termprint);
// }

/* pre : term caps have been configured
* post : turns highlight off in terminal
*/
// void term_standend()
// {
// 	tputs(gl_env.stand_end, 1, my_termprint);
// }

/* pre : row and column in terminal
* post : moves cursor to column x and row y in terminal
*/
void term_move(int x, int y)
{
	tputs(tgoto(gl_env.move, x, y), 1, my_termprint);
}

/* pre : element number
* post : moves cursor to element in terminal
*/
// void term_move_to_item(int n)
// {
// 	term_move(gl_env.elements[n].x, gl_env.elements[n].y);
// 	gl_env.pos = n;
// }

/* pre : terminal has been restored
* post : Prints to STDOUT the list of all highlighted items with whitespaces inbetween and exits
*/
// void getout()
// {
// 	unsigned int i;

// 	for (i = 0; i < gl_env.nbelems; i++)
// 	{
// 		if (gl_env.elements[i].mode) /* If element was highlighted */
// 		{
// 			my_str(gl_env.elements[i].elem);
// 			break;
// 		}
// 	}

// 	for (i++; i < gl_env.nbelems; i++)
// 	{
// 		if (gl_env.elements[i].mode) /* If element was highlighted */
// 		{
// 			my_char(' ');
// 			my_str(gl_env.elements[i].elem);
// 		}
// 	}
// 	exit(0);
// }

/* pre : number of elements, vector of strings representing elements
* post : Initializes gl_env strings
*/
void setup_env()
{
	unsigned int i;

	// gl_env.nbelems = n; /* Set number of elements */
	gl_env.pos = 0; /* Set current element position */
	gl_env.flag = 0; /* Unset flag */

	gl_env.curr_dir = (char *) xmalloc(BUF_SZ * sizeof(char)); /* allocate current directory buffer */
	gl_env.curr_dir_len = 0; /* initialize current directory length */
	
	gl_env.elements = (t_elem *) xmalloc(n * sizeof(*(gl_env.elements)));

	/* Fill array with elements */
	for (i = 0; i < n; i++)
	{
		gl_env.elements[i].elem = elems[i];
		gl_env.elements[i].size = my_strlen(gl_env.elements[i].elem);
		gl_env.elements[i].x = 0;
		gl_env.elements[i].y = 0;
		gl_env.elements[i].mode = 0;
	}
}