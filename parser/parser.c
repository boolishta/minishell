#include <unistd.h>
#include <termios.h>
#include <curses.h>
#include <term.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>

int	ft_putchar(int c)
{
	write(1, &c, 1);
	return (0);
}

int main (int argc, char const *argv[], char const *envp[])
{
	char str[2000];
	int l;
	struct termios term;
	char *term_name;
	int n;

	term_name = getenv("TERM");

	tcgetattr(0, &term);
	term.c_lflag &= ~(ECHO);
	term.c_lflag &= ~(ICANON);
	tcsetattr(0, TCSANOW, &term);

	tgetent(0, term_name); //TODO: проверить на взращаемое значение

	char *str2 = tgetstr("kr", 0);
	while (strcmp(str, "\4"))
	{
		tputs(save_cursor, l, ft_putchar);
		do
		{
			struct winsize win;

			// ioctl(0, FIOCGWINSZ, &win);
			// printf("%s\n", win);

			ioctl(0, FIONREAD, &n);
			l = read(0, str, 100);
			str[l] = 0;
			if (!strcmp(str, "\e[A"))
			{
				tputs(restore_cursor, 1, ft_putchar);
				tputs(tigetstr("ed"), 1, ft_putchar);
				write(1, "previous", 8);
			}
			else if (!strcmp(str, "\e[B"))
			{
				tputs(restore_cursor, 1, ft_putchar);
				tputs(tigetstr("ed"), 1, ft_putchar);
				write(1, "next", 4);
			}
			else if (!strcmp(str, key_backspace) && !strcmp(str, "\177")) // \177[c
			{
				// printf("backspace is %i\n", key_backspace[2]);
				tputs(cursor_left, 1, ft_putchar);
				tputs(tigetstr("ed"), 1, ft_putchar);
			}
			else
				write(1, str, l);

		} while (strcmp(str, "\n") && strcmp(str, "\4"));
	}
	write(1, "\n", 1);
	tcgetattr(0, &term);
	term.c_lflag |= ICANON;
	term.c_lflag |= ECHO;
	tcsetattr(0, TCSANOW, &term);

	return(0);
}