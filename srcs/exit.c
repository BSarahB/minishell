/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 21:48:43 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/17 21:48:54 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	ft_printf_exit2(t_simpleCmd *simpleCmd, t_data_env *data_env, \
t_list *tmp)
{
	if (ft_exit_is_arg_valid(tmp->content, simpleCmd) == 1)
	{
		if (simpleCmd->exit_solo == 1)
		{
			ft_putstr_fd("exit\n", 1);
			data_env->exit_parent = 1;
		}
		simpleCmd->exit_code = ft_atoi_modulo(tmp->content, simpleCmd);
	}
	else
	{
		if (simpleCmd->exit_solo == 1)
			data_env->exit_parent = 1;
	}
}

void	ft_check_exit_token(t_simpleCmd *simpleCmd, t_data_env *data_env, \
t_list *tmp)
{
	while (tmp != NULL && tmp->position <= simpleCmd->end_simplecmd_pos && \
simpleCmd->exit_no_option == 0 && simpleCmd->builtin == exxit)
	{
		if (simpleCmd->nofile == 1)
			break ;
		if (simpleCmd->nb_of_tokens_in_simplecmd > 2)
		{
			if (ft_exit_is_arg_valid(tmp->content, simpleCmd) == 1)
			{
				ft_too_many_arg_msg(simpleCmd);
				return ;
			}
			else
				break ;
		}
		else
		{
			ft_printf_exit2(simpleCmd, data_env, tmp);
			break ;
		}
	}
}

void	ft_printf_exit(t_simpleCmd *simpleCmd, t_data_env *data_env)
{
	simpleCmd->exit_str = ft_strdup("exit");
	ft_putstr_fd("exit\n", 2);
	simpleCmd->exit_code = ft_get_exit_status_atoi(data_env->lst_envp);
	data_env->exit_parent = 1;
}

void	ft_check_exit_first_token(t_cmd *cmd, t_simpleCmd *simpleCmd, \
t_data_env *data_env, t_list **tmp)
{
	if ((*tmp != NULL) && (ft_strcmp("exit", (*tmp)->content) == 0))
	{
		simpleCmd->is_builtin = 1;
		simpleCmd->builtin = exxit;
		if (cmd->nb_of_simplecmds == 1)
		{
			simpleCmd->exit_solo = 1;
			simpleCmd->builtin_solo = 1;
		}
		if ((*tmp)->next == NULL)
		{
			simpleCmd->exit_no_option = 1;
			if (simpleCmd->exit_solo == 1)
			{
				ft_printf_exit(simpleCmd, data_env);
				return ;
			}
			simpleCmd->exit_code = 0;
			return ;
		}
		else
			*tmp = (*tmp)->next;
	}
}

void	ft_check_exit(t_cmd *cmd, t_list *start_lst_token_retokenized, \
t_simpleCmd *simpleCmd, t_data_env *data_env)
{
	t_list	*tmp;

	tmp = NULL;
	tmp = start_lst_token_retokenized;
	ft_check_exit_first_token(cmd, simpleCmd, data_env, &tmp);
	ft_check_exit_token(simpleCmd, data_env, tmp);
}
