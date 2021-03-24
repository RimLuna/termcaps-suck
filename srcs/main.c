/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbougssi <rbougssi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/01 12:23:09 by arraji            #+#    #+#             */
/*   Updated: 2021/03/24 15:56:15 by rbougssi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
# include <termcap.h>
# include <term.h>
void	init(t_all *all)
{
	if (g_env == NULL)
	{
		g_total_env = 0;
		g_all = all;
		init_env();
		change_variables(NULL, -1);
	}
	g_pid = 0;
	all->parser.bits = 64;
	ft_end((void **)&(all->parser.line), NULL, 1);
	all->pipe = NULL;
	all->curs_pos = 0;
}

void	clear(t_all *all)
{
	t_command	*curr;
	void		*save;

	while (all->pipe)
	{
		curr = all->pipe->cmd_head;
		while (curr)
		{
			free_tab(curr->argv, ft_tablen(curr->argv));
			ft_end((void**)&(curr->full_path), (void**)&(curr->file), 1);
			while (curr->list_args)
				curr->list_args =
				(t_args *)ft_lstdelone((t_list *)curr->list_args, NULL);
			while (curr->all_files)
				curr->all_files =
				(t_files *)ft_lstdelone((t_list *)curr->all_files, NULL);
			save = (void *)curr;
			curr = curr->next;
			free(save);
		}
		save = (void *)all->pipe;
		all->pipe = all->pipe->next;
		free(save);
	}
}

static void init_history(t_hist *history)
{
	history->cmd = NULL;
	history->current = NULL;
	history->prev = NULL;
	history->end = NULL;
	history->next = NULL;
}

void			canonical_no()
{
	struct termios term;

	tcgetattr(0, &term);
	term.c_lflag |= (ICANON | ECHO);
	tcsetattr(0, TCSADRAIN, &term);
}

static void  clear_history(t_hist *head)
{
	t_hist *tmp;
	t_hist *prev;
	if (head->cmd)
		free(head->cmd);
	tmp = head->next;
	{
		while (tmp)
		{
			prev = tmp;
			tmp = tmp->next;
			free(prev->cmd);
			free(prev);
		}
	}
}

int		main(int argc, char **argv, char **env)
{
	t_all	all;
	t_hist	history;

	(void)argc;
	(void)argv;
	g_environ = env;
	all.exit_status = 0;
	signal(SIGINT, handler);
	signal(SIGQUIT, handler);
	all.parser.line = NULL;
	init_history(&history);
	while (1)
	{
		init(&all);
		if (get_data(&all, &history) == FALSE || here_we_go(&all) == FALSE || 1)
			clear(&all);
	}
	clear_history(&history);
	canonical_no();
}
