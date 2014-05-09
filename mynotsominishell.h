#ifndef _MYNOTSOMINISHELL_H_
#define _MYNOTSOMINISHELL_H_

#include <sys/ioctl.h>
#include <stdio.h> /* for perror and things */
#include <fcntl.h>
#include <termios.h>
#include <sys/termios.h>
#include "../lib/my.h"
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <termcap.h>
#include <sys/wait.h> /* for wait */
#include <errno.h> /* for errno */
#include <string.h> /* for memcpy */

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define BUF_SZ 2097152 /* same buffer size as in BASH */
#define HISTORY_LIMIT 100 /* size of command history */

#define READTIME 1
/* used to return from read after 1/10th of second passed from the first byte read */

#define READMIN 3
/* used to return from read if 3 bytes have been read. Needed for things like characters which are \E[A B C or D */

#ifndef NULL
#define NULL (0)
#endif

#define SPACEBAR " "
#define ESC "\E"
#define BACKSPACE 127
#define ENTER "\n"
#define CTRL_A 1
#define CTRL_E 5
#define CTRL_K 11
#define CTRL_L 12
#define CTRL_Y 25
#define KU "\E[A"
#define KD "\E[B"
#define KR "\E[C"
#define KL "\E[D"
#define CLEAR "cl"
#define MOVE_LEFT "le"
#define MOVE_RIGHT "nd"
#define MOVE_DOWN "do"
#define MOVE_START "cr" /* move to start of line */
#define DEL_CHAR "dc"
#define CURSOROFF "vi"
#define CURSORON "ve"

typedef struct s_elem
{
	char *elem;
	unsigned int size;
}	t_elem;

typedef struct s_env
{
	char *move_left;
	char *move_right;
	char *move_down;
	char *move_start;
	char *clear;
	char *delete_char;
	char *cursoroff;
	char *cursoron;
	struct winsize win;
	struct termio line_backup;
	int	stdio_backup;
	int history_fd;
	
	unsigned int pos; /* position of cursor relative to command's first character */
	unsigned int cursor_col; /* column of cursor */
	t_elem curr_dir; /* current directory path */
	t_elem cmd_buffer; /* buffer for current command */
	t_elem hst_buffer; /* buffer for command in history for editing */
	t_elem cpy_buffer; /* buffer for copied command */
	t_elem *curr_cmd; /* pointer to command being displayed */

	t_elem *elements; /* represents command history */
	unsigned int nbelems; /* number of commands in history */
	unsigned int elem_last; /* index of last command in history */
	unsigned int elem_first; /* index of most recent command in history */
	unsigned int curr_elem; /* index of currently selected element in history */
} t_env;

t_env gl_env;
/* created as a structure to limit number of globals to 1 */

void init_terminal();
void restore_terminal();
char *term_get_cap(char*);
void init_caps();
int	my_termprint(int);
void term_clear();
void term_vi();
void term_ve();

void check_character(char *c);
void check_command();
char add_character(char, unsigned int);
void remove_character(unsigned int);

char get_win_size();
void show_prompt();
void refreshin();
void refreshout();

void moveup();
void movedown();
void moveleft();
void moveright();
void movestart();
void moveend();

void addchar(char c);
void deletechar();
void getout();

void setup_env();
void set_curr_dir();

void cut_cmd();
void paste_cmd();
void print_prompt();
void print_cmd(unsigned int);

void term_move_left();
void term_move_right();

void term_delete_char();

void load_history();
void save_history();
void save_command();
#endif
