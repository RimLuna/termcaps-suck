/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_we_go.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbougssi <rbougssi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/01 13:59:41 by arraji            #+#    #+#             */
/*   Updated: 2021/03/24 15:05:45 by rbougssi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
# include <termcap.h>
# include <term.h>
static	void	set_return(void)
{
	int			ret;
	int			data;

	ret = 0;
	data = 0;
	while (ret != -1)
	{
		ret = wait(&data);
		if (ret == g_pid && WIFEXITED(data))
			g_all->exit_status = WEXITSTATUS(data);
		else if (ret == g_pid && WIFSIGNALED(data))
		{
			g_all->exit_status = 128 + WTERMSIG(data);
			WTERMSIG(data) == SIGQUIT ? write(1, "Quit\n", 5) : 1;
		}
	}
	g_pid = 0;
}

t_bool			here_we_go(t_all *all)
{
	t_pipeline	*pipe;
	t_command	*cmd;
	int			pipefd[2];
	int			savefd[2];

	pipe = all->pipe;
	fd_saving(savefd);
	while (pipe)
	{
		cmd = pipe->cmd_head;
		while (cmd)
		{
			cmd->simple = pipe->simple;
			reverse_args(&(cmd->list_args));
			link_argv(cmd);
			executing(cmd, pipefd, savefd);
			cmd = cmd->next;
		}
		set_return();
		pipe = pipe->next;
	}
	fd_saving(savefd);
	return (TRUE);
}

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

void clear_line()
{
	ft_do_termcap("cr");
	ft_do_termcap("cd");
	ft_fprintf(1, "%s%s%s%s", BOLD, PRINT_GR, PS, RESET);
}

void			up(t_hist *history, char *buff)
{
	clear_line();
}

void			down(t_hist *history, char *buff)
{
	clear_line();
}

void			del()
{
	// ft_do_termcap("le");
	// ft_do_termcap("dc");
	int		len;

	len = ft_strlen(buff);
	if (len == 0)
		return ;
	buff[len - 1] = '\0';
	clear_line();
	write(1, buff, ft_strlen(buff));
}

char            *readikhaane(t_hist *history)
{
    char c;
    int  index;

    ft_setterm();
	buff = (char *)malloc(1000);
    index = 0;
    while (1)
    {
        read(0, &c, 1);
        if (c == '\n')
        {
			write(1, "\n", 1);
            buff[index] = '\0';
            return (buff);
        }
        if (c == 65)
            up(history, buff);
        else if (c == 66)
            down(history, buff);
		else if (c == 127)
		{
			if (index != 0)
				index--;
			del();
		}
        else if (ft_isprint(c))
        {
            buff[index++] = c;
            write(1, &c, 1);
        }
    }
}

t_bool			get_data(t_all *all, t_hist *history)
{
	(all->exit_status == 0) ? ft_fprintf(1, "%s%s%s%s", BOLD, PRINT_GR, PS,
	RESET) : ft_fprintf(1, "%s%s%s%s", BOLD, PRINT_RED, PS, RESET);
/*	if ((all->parser.rt = get_next_line(1, &all->parser.line)) == -1)
		return (error(E_STANDARD, 1, NULL));*/
	all->parser.line = readikhaane(history);
	
	if (lexer(all->parser.line, &all->parser) == FALSE ||
	parser(all->parser.line, all) == FALSE)
		return (FALSE);
	return (TRUE);
}