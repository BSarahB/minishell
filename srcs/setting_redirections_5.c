/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setting_redirections_5.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 19:06:24 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/09/05 19:06:45 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_builtin_pwd(t_settings *set, t_param_sr para)
{
	if (para.cmd->simplecmds[set->i]->is_builtin == 1 && \
								para.cmd->simplecmds[set->i]->builtin == 2)
	{
		if (para.cmd->simplecmds[set->i]->pwd != NULL)
		{
			ft_putstr_fd(para.cmd->simplecmds[set->i]->pwd, STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		ft_free_and_exit_child(set, &para);
		exit(0);
	}
}

void	ft_builtin_echo(t_settings *set, t_param_sr para)
{
	if (para.cmd->simplecmds[set->i]->is_builtin == 1 && \
							para.cmd->simplecmds[set->i]->builtin == 0)
	{
		ft_echo(para.cmd->simplecmds[set->i]);
		ft_free_and_exit_child(set, &para);
		exit(0);
	}
}

void	ft_builtin_exit(t_settings *set, t_param_sr para)
{
	int	exit_code;

	exit_code = 0;
	if (para.cmd->simplecmds[set->i]->is_builtin == 1 && \
								para.cmd->simplecmds[set->i]->builtin == 6)
	{
		exit_code = para.cmd->simplecmds[set->i]->exit_code;
		ft_free_and_exit_child(set, &para);
		exit(exit_code);
	}
}

void	ft_builtin_unset(t_settings *set, t_param_sr para)
{
	if (para.cmd->simplecmds[set->i]->is_builtin == 1 && \
								para.cmd->simplecmds[set->i]->builtin == 4)
	{
		ft_free_and_exit_child(set, &para);
		exit(0);
	}
}
