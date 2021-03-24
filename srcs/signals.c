/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbougssi <rbougssi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/04 00:11:18 by arraji            #+#    #+#             */
/*   Updated: 2021/03/24 15:01:20 by rbougssi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handler(int sig)
{
	if (sig == SIGINT && g_pid == 0)
	{
		bzero(buff, ft_strlen(buff));
		ft_fprintf(1, "\n" BOLD PRINT_RED PS RESET);
		g_all->exit_status = 128 + sig;
		get_next_line(-5, NULL);
	}
	if (sig == SIGQUIT)
	{
		if (g_pid == 0)
			write(1, "\b \b\b \b", 6);
	}
}
