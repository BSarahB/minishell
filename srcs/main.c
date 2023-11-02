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

int	g_signal;

void	handler_sigint(int num)
{
	int	fd_heredoc;

	(void)num;
	fd_heredoc = 0;
	if (g_signal == IN_PROMPT)
	{
		ft_putstr_fd("\n", 0);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		ft_set_exit_code_in_lst_envp(NULL, 0, 130);
	}
	if (g_signal == IN_HD)
		g_signal = HD_STOP;
	if (g_signal == HD_STOP)
	{
		ft_putstr_fd("\n", 0);
		ft_set_exit_code_in_lst_envp(NULL, 0, 130);
		dup2(fd_heredoc, STDIN_FILENO);
		close(fd_heredoc);
		return ;
	}
	if (g_signal == IN_S_CMD)
		ft_putstr_fd("\n", 2);
}

void	ft_execute_scmds(t_param_m *para)
{
	if (g_signal != HD_STOP)
	{
		para->envp_t = ft_lst_to_tab(para->data_env->lst_envp);
		para->exit_status = ft_setting_redirections_and_pipes(\
			(t_param_sr){para->cmd, para->envp_t, para->data, \
						para->line, para->data_env});
		if (para->data_env->exit_parent == 1)
		{
			para->exit_status = ft_get_exit_status_atoi(\
													para->data_env->lst_envp);
			ft_free(para->cmd, para->lst_token, para->data, para->line);
			ft_free_struct_t_cmd_only(&(para->cmd));
			if (para->envp_t != NULL)
				ft_free_tab(&(para->envp_t));
			if (para->data_env != NULL)
				ft_free_struct_t_data_env(&(para->data_env));
			exit(para->exit_status);
		}
	}
}

void	ft_signal_setup(char **envp)
{
	g_signal = IN_PROMPT;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handler_sigint);
	ft_check_tab_envp_removed(envp);
}

void	ft_handle_exit(t_param_m *para)
{
	ft_putstr_fd("exit\n", 1);
	ft_get_para_exit_status(para);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_param_m	para;

	ft_init_t_param(&para);
	(void)argc;
	(void)argv;
	while (1)
	{
		ft_signal_setup(envp);
		para.line = readline(" ~ ");
		if (!para.line)
		{
			ft_handle_exit(&para);
			break ;
		}
		add_history(para.line);
		para.data = ft_tokenize_line(para.line);
		para.lst_token = para.data->lst_token;
		para.exit_syntax_error = ft_check_bash_syntax_error_caracteres_volee(\
															para.lst_token);
		ft_execute_command(&para, envp);
		g_signal = IN_PROMPT;
	}
	if (para.data_env != NULL)
		ft_free_struct_t_data_env(&(para.data_env));
	return (para.exit_status);
}
