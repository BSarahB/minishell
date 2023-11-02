/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 15:10:59 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/03/07 15:11:02 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	ft_init(t_param_m *para, char **envp)
{
	ft_struct_init_data_env(&(para->data_env));
	para->data_env->lst_envp = ft_get_lst_envp(envp);
	para->data_env->lst_envp_d = ft_get_lst_envp(envp);
	ft_set_exit_code_in_lst_envp(para->data_env->lst_envp, 1, \
														para->exit_syntax);
	para->exit_syntax = -13;
	para->flag_save_envp = 0;
	para->envp_tab = ft_lst_to_tab(para->data_env->lst_envp);
	ft_expand_and_retokenize(para->lst_token, para->envp_tab);
	if (para->envp_tab != NULL)
		ft_free_tab(&(para->envp_tab));
}

void	ft_reinit(t_param_m *para)
{
	ft_set_exit_code_in_lst_envp(para->data_env->lst_envp, 1, \
														para->exit_syntax);
	para->exit_syntax = -13;
	para->envp_tab = ft_lst_to_tab(para->data_env->lst_envp);
	ft_expand_and_retokenize(para->lst_token, para->envp_tab);
	if (para->envp_tab != NULL)
		ft_free_tab(&(para->envp_tab));
}

void	ft_get_para_exit_status(t_param_m *para)
{
	if (para->data_env)
	{
		if (para->data_env->lst_envp)
			para->exit_status = ft_get_exit_status_atoi(\
											para->data_env->lst_envp);
	}
}

void	ft_init_t_param(t_param_m *para)
{
	para->line = NULL;
	para->cmd = NULL;
	para->lst_token = NULL;
	para->data = NULL;
	para->exit_status = 0;
	para->exit_syntax_error = 0;
	para->exit_syntax = -13;
	para->flag_save_envp = 1;
	para->envp_t = NULL;
	para->envp_tab = NULL;
	para->data_env = NULL;
}

void	ft_execute_command(t_param_m *para, char **envp)
{
	if (para->exit_syntax_error == 0)
	{
		if (para->flag_save_envp == 1)
			ft_init(para, envp);
		else
			ft_reinit(para);
		para->cmd = ft_struct_init_cmd(&(para->cmd), para->lst_token);
		para->cmd->path_tab = ft_get_path(envp);
		ft_parse_tokens_in_s_cmd(para->cmd, para->lst_token, para->data_env);
		if (para->cmd->nb_of_heredocs != 0)
			ft_heredoc_interaction(para->cmd, 0, 1);
		ft_execute_scmds(para);
	}
	else
	{
		if (para->exit_syntax_error > 0)
			para->exit_syntax = para->exit_syntax_error;
		if (para->exit_syntax_error == -12)
			para->exit_syntax = 0;
	}
	ft_free(para->cmd, para->lst_token, para->data, para->line);
	ft_free_struct_t_cmd_only(&(para->cmd));
	if (para->envp_t != NULL)
		ft_free_tab(&(para->envp_t));
}
