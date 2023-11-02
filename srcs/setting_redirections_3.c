/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setting_redirections_3.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 19:06:24 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/09/05 19:06:45 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handler_sigquit(int num)
{
	(void)num;
	ft_putstr_fd("Quit (core dumped)\n", 2);
}

int	ft_is_flag_n(char *str)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	if (str[i] == '\0')
		return (flag);
	if (str[i] != '-')
		return (flag);
	if (str[i] == '-')
		i++;
	while (str[i])
	{
		if (str[i] != 'n')
			return (flag);
		i++;
	}
	flag = 1;
	return (1);
}

void	ft_echo(t_simpleCmd *simpleCmd)
{
	int	j;
	int	flag_n;

	flag_n = 0;
	j = 1;
	if (simpleCmd->echo_no_option == 1)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		return ;
	}
	while (simpleCmd->cmd_and_args[j] \
		&& ft_is_flag_n(simpleCmd->cmd_and_args[j]) == 1)
	{
		flag_n = 1;
		j++;
	}
	while (simpleCmd->cmd_and_args[j])
	{
		ft_putstr_fd(simpleCmd->cmd_and_args[j], STDOUT_FILENO);
		if (flag_n == 0 && simpleCmd->cmd_and_args[j + 1] == NULL)
			ft_putstr_fd("\n", STDOUT_FILENO);
		else if (simpleCmd->cmd_and_args[j + 1])
			ft_putstr_fd(" ", STDOUT_FILENO);
		j++;
	}
}

void	ft_free_and_exit_child(t_settings *set, t_param_sr *para)
{
	if (set->pip_exists == 1)
	{
		close(set->pip[0]);
		close(set->pip[1]);
	}
	close(set->savein);
	close(set->saveout);
	ft_free_struct_t_settings(&set);
	ft_free_in_child(para->cmd, para->data, para->line);
	ft_free_struct_t_cmd_only(&(para->cmd));
	if (para->envp_t != NULL)
		ft_free_tab(&(para->envp_t));
	if (para->data_env != NULL)
		ft_free_struct_t_data_env(&(para->data_env));
}

void	ft_aff_export(t_listenvp *tmp, t_param_sr para, t_settings *set)
{
	while (tmp && para.cmd->simplecmds[set->i]->builtin != 5)
	{
		if (ft_strncmp(tmp->key_value, "?=", 2) != 0)
		{
			ft_putstr_fd("declare -x ", STDOUT_FILENO);
			ft_putstr_fd(tmp->key_value, STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		tmp = tmp->next;
	}
}
