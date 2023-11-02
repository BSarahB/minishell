/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setting_redirections_4.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 19:06:24 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/09/05 19:06:45 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_cmd_not_found(int exec_return, t_settings *set, t_param_sr para)
{
	if (exec_return == -1 && (errno == 2 || errno == 13))
	{
		if ((set->i < para.cmd->nb_of_simplecmds) \
			&& (set->i != (para.cmd->nb_of_simplecmds) - 1))
			close(set->pip[0]);
		close(set->savein);
		close(set->saveout);
		ft_free_struct_t_settings(&set);
		ft_free_in_child(para.cmd, para.data, para.line);
		ft_free_struct_t_cmd_only(&(para.cmd));
		if (para.envp_t != NULL)
			ft_free_tab(&(para.envp_t));
		if (para.data_env != NULL)
			ft_free_struct_t_data_env(&(para.data_env));
		exit(127);
	}
}

void	ft_builtin_export_w_option(t_settings *set, t_param_sr para)
{
	int	exit_code;

	if (para.cmd->simplecmds[set->i]->is_builtin == 1 && \
					para.cmd->simplecmds[set->i]->export_no_option == 0 && \
							para.cmd->simplecmds[set->i]->builtin == 3)
	{
		exit_code = para.cmd->simplecmds[set->i]->exit_code;
		ft_free_and_exit_child(set, &para);
		exit(exit_code);
	}
}

void	ft_builtin_cd(t_settings *set, t_param_sr para)
{
	int	exit_code;

	if (para.cmd->simplecmds[set->i]->is_builtin == 1 && \
							para.cmd->simplecmds[set->i]->builtin == 1)
	{
		if (para.cmd->simplecmds[set->i]->oldpwd != NULL)
		{
			ft_putstr_fd(para.cmd->simplecmds[set->i]->oldpwd, STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		exit_code = para.cmd->simplecmds[set->i]->exit_code;
		ft_free_and_exit_child(set, &para);
		exit(exit_code);
	}
}

void	ft_builtin_env(t_settings *set, t_param_sr para)
{
	t_listenvp	*tmp2;
	int			exit_code;

	tmp2 = NULL;
	if (para.cmd->simplecmds[set->i]->is_builtin == 1 && \
						para.cmd->simplecmds[set->i]->builtin == 5)
	{
		exit_code = para.cmd->simplecmds[set->i]->exit_code;
		if (exit_code != 127)
		{
			tmp2 = para.data_env->lst_envp;
			while (tmp2)
			{
				if (ft_strncmp(tmp2->key_value, "?=", 2) != 0)
				{
					ft_putstr_fd(tmp2->key_value, STDOUT_FILENO);
					ft_putstr_fd("\n", STDOUT_FILENO);
				}
				tmp2 = tmp2->next;
			}
		}
		ft_free_and_exit_child(set, &para);
		exit(exit_code);
	}
}

void	ft_builtin_export_declare_x(t_settings *set, t_param_sr para)
{
	t_listenvp	*tmp;

	tmp = NULL;
	para.data_env->lst_envp_d = \
		ft_add_double_quote_to_envp_d(para.data_env->lst_envp_d);
	tmp = para.data_env->lst_envp_d;
	ft_aff_export(tmp, para, set);
	ft_free_and_exit_child(set, &para);
	exit(0);
}
