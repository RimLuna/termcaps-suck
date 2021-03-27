#include "minishell.h"
#include <termcap.h>
#include <term.h>

void    ft_cannonical_mod(void)
{
    struct termios term;
    tcgetattr(0, &term);
    term.c_lflag &= ~(ICANON);
    term.c_lflag &= ~(ECHO);
    term.c_lflag |= (ISIG);
    term.c_cc[VMIN] = 1;
    term.c_cc[VTIME] = 0;
    tcsetattr(0, TCSADRAIN, &term);
}

void    ft_setterm(void)
{
    char    *termtype;
    char    term_buffer[2048];
    int     success;
    termtype = getenv("TERM");
    if (termtype == 0)
    {
        ft_putstr_fd("Specify a terminal type with `setenv TERM <yourtype>'.\n", 1);
        exit(0);
    }
    success = tgetent(term_buffer, termtype);
    if (success < 0)
    {
        ft_putstr_fd("Could not access the termcap data base.\n", 1);
        exit(0);
    }
    if (success == 0)
    {
        ft_putstr_fd("Terminal type `%s' is not defined.\n", 1);
        exit(0);
    }
    ft_cannonical_mod();
}

int     h_putchar(int c)
{
    write(1, &c, 1);
    return (0);
}

void    ft_do_termcap(char *termcap)
{
    tputs(tgetstr(termcap, NULL), 1, &h_putchar);
}
