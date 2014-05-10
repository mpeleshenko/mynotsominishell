#mynotsominishell

A custom built shell that has the following features:

##Features:
- on window resize, reprint current prompt and typed command
- up/down arrows command history
- left/right arrows to move cursor
- backspace at cursor
- insert characters at cursor
- Ctrl-A - move cursor to first character
- Ctrl-E - move cursor to the end
- Ctrl-K - cut from cursor position to end of the command
- Ctrl-L - clear screen and reshow typed command
- Ctrl-Y - paste what was cut at cursor
- loads command history (up to 100 commands) from .history file on start (if it exists)
- saves command history (up to 100 commands) to .history file on exit
