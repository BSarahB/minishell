/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 23:05:52 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/09 23:06:03 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_remove_var_from_lst_envp(t_list *tmp, t_data_env *data_env)
{
	ft_is_var_to_unset_already_in_lst_envp(data_env, tmp->content);
}

int	ft_unset_value_is_valid(t_list *tmp)
{
	char	*str;
	int		i;

	(void)tmp;
	i = 0;
	str = tmp->content;
	if (str[0] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] == '=')
			return (0);
		i++;
	}
	return (1);
}

void	ft_check_unset_token(t_param_u *para, t_simpleCmd *simpleCmd, \
												t_data_env *data_env)
{
	while (para->tmp != NULL && para->tmp->position \
		<= simpleCmd->end_simplecmd_pos && simpleCmd->unset_no_option == 0 \
		&& para->flag_unset == 1)
	{
		if (simpleCmd->nofile == 1)
			break ;
		if (simpleCmd->unset_solo == 1)
		{
			if (ft_strcmp(para->tmp->content, "PWD") == 0)
				data_env->flag_oldpwd_istoremove = 1;
			if (ft_unset_value_is_valid(para->tmp) == 1)
			{
				ft_remove_var_from_lst_envp(para->tmp, data_env);
				ft_remove_var_from_lst_envp_d(para->tmp, data_env);
			}
			else
				ft_remove_var_from_lst_envp_d(para->tmp, data_env);
		}
		para->tmp = para->tmp->next;
	}
}

void	ft_check_unset_first_token(t_param_u *para, \
								t_cmd *cmd, t_simpleCmd *simpleCmd)
{
	if (para->tmp != NULL)
	{
		if (para->flag_unset == -1 \
			&& (ft_strcmp("unset", para->tmp->content) == 0))
		{
			para->flag_unset = 1;
			simpleCmd->is_builtin = 1;
			simpleCmd->builtin = unset;
			if (para->tmp->next == NULL)
				simpleCmd->unset_no_option = 1;
			else
			{
				para->tmp = para->tmp->next;
				if (ft_strcmp(para->tmp->content, "?") == 0)
					para->tmp = para->tmp->next;
			}
			if (cmd->nb_of_simplecmds == 1)
			{
				simpleCmd->unset_solo = 1;
				simpleCmd->builtin_solo = 1;
			}
		}
		else
			para->flag_unset = 0;
	}
}

void	ft_check_unset(t_cmd *cmd, t_list *start_lst_token_retokenized, \
					t_simpleCmd *simpleCmd, t_data_env *data_env)
{
	t_param_u	para;

	para.tmp = NULL;
	para.flag_unset = -1;
	para.tmp = start_lst_token_retokenized;
	ft_check_unset_first_token(&para, cmd, simpleCmd);
	ft_check_unset_token(&para, simpleCmd, data_env);
}
