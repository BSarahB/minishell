/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setting_redirections.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 00:06:40 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/31 00:06:43 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_signal;

void	ft_child_process(t_settings *set, t_param_sr para)
{
	int	exec_return;

	exec_return = 0;
	g_signal = IN_S_CMD;
	signal(SIGQUIT, handler_sigquit);
	if (para.cmd->simplecmds[set->i]->is_builtin == 0)
		exec_return = ft_execute_cmd(para.cmd, (int)set->i, para.envp_t, set);
	ft_cmd_not_found(exec_return, set, para);
	ft_builtin_unset(set, para);
	ft_builtin_exit(set, para);
	ft_builtin_echo(set, para);
	ft_builtin_pwd(set, para);
	ft_builtin_env(set, para);
	ft_builtin_cd(set, para);
	ft_builtin_export_w_option(set, para);
	ft_builtin_export_declare_x(set, para);
}

void	ft_check_fork_error(t_settings *set, t_param_sr *para)
{
	(void)para;
	if (set->ret == -1)
	{
		perror (" pb fork ");
		ft_free_struct_t_settings(&set);
		ft_free_in_child(para->cmd, para->data, para->line);
		ft_free_struct_t_cmd_only(&(para->cmd));
		if (para->envp_t != NULL)
			ft_free_tab(&(para->envp_t));
		if (para->data_env != NULL)
			ft_free_struct_t_data_env(&para->data_env);
		exit(EXIT_FAILURE);
	}
}

void	ft_go_through_each_process(t_settings *set, t_cmd *cmd, t_param_sr para)
{
	while (set->i < cmd->nb_of_simplecmds)
	{
		ft_redirect_input(set, cmd);
		if (set->i == (cmd->nb_of_simplecmds) - 1)
			ft_last_simplecmd(set, cmd);
		else
			ft_regular_simplecmd(set, cmd);
		if (cmd->simplecmds[set->i]->nofile != 1)
		{
			if (cmd->simplecmds[set->i] != NULL && \
							cmd->simplecmds[set->i]->cmd_and_args != NULL)
			{
				ft_redirect_output(set);
				g_signal = IN_S_CMD;
				signal(SIGQUIT, handler_sigquit);
				set->ret = fork();
				ft_check_fork_error(set, &para);
				if (set->ret == 0)
					ft_child_process(set, para);
			}
		}
		(set->i)++;
	}
}

int	ft_set_exit_status_after_fork(t_cmd *cmd, t_settings *set, int exit_status)
{
	if (set->i >= 1 && cmd->simplecmds[set->i -1]->nofile == 1)
		exit_status = cmd->simplecmds[set->i -1]->exit_code;
	if (exit_status == 141)
		exit_status = 0;
	return (exit_status);
}

int	ft_setting_redirections_and_pipes(t_param_sr para)
{
	int			exit_status;
	t_settings	*set;
	t_cmd		*cmd;
	t_data_env	*data_env;

	cmd = para.cmd;
	data_env = para.data_env;
	set = ft_struct_init_settings(&set);
	if (cmd->simplecmds[set->i] == NULL)
	{
		ft_free_struct_t_settings(&set);
		return (0);
	}
	ft_save_in_and_out(set);
	ft_set_fdin_for_first_simplecmd(set, cmd);
	ft_go_through_each_process(set, cmd, para);
	exit_status = ft_exit_status(set->ret, set);
	exit_status = ft_set_exit_status_after_fork(cmd, set, exit_status);
	data_env->lst_envp = ft_get_exit_status(&data_env->lst_envp, "?=", \
											exit_status);
	ft_restore_original_in_and_out(set);
	ft_free_struct_t_settings(&set);
	return (exit_status);
}
