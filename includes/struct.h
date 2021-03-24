/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbougssi <rbougssi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 16:05:20 by arraji            #+#    #+#             */
/*   Updated: 2021/03/24 14:53:31 by rbougssi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_STRUCT_H
# define FT_STRUCT_H
# ifndef D_ERROR
#  error "include only dumbshell.h"
# endif

typedef	struct	s_files
{
	struct s_files	*next;
	char			*file;
	int				type;
}				t_files;

typedef	struct	s_parser
{
	int		bits;
	char	*line;
	int		rt;
}				t_parser;

typedef	struct	s_args
{
	struct s_args	*next;
	char			c;
	char			*str;
	int				type;
}				t_args;

typedef	struct	s_command
{
	struct s_command	*next;
	char				*cmd_name;
	char				**argv;
	struct s_args		*list_args;
	char				*full_path;
	int					fd;
	char				*file;
	t_files				*all_files;
	char				read_type;
	int					simple;
	int					relation;
}				t_command;

typedef struct		s_hist
{
	struct s_hist	*next;
	struct s_hist	*prev;
	struct s_hist	*end;
	struct s_hist	*current;
	char			*cmd;
}					t_hist;

typedef	struct	s_pipeline
{
	struct s_pipeline	*next;
	int					simple;
	struct s_command	*cmd_head;
}				t_pipeline;

typedef	struct	s_all
{
	unsigned char	exit_status;
	t_parser		parser;
	t_pipeline		*pipe;
	char			*pwd;
	int				curs_pos;
}				t_all;

typedef	struct	s_env
{
	struct s_env	*next;
	char			*key;
	char			*value;
	char			*full_var;
	int				type;
}				t_env;
#endif
