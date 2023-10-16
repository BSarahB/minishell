/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 18:02:08 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/11 18:02:23 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_check_env(t_cmd *cmd, t_list *start_lst_token_retokenized, t_simpleCmd *simpleCmd, t_data_env *data_env)
{
	(void)cmd;
	(void)start_lst_token_retokenized;
	(void)simpleCmd;
	(void)data_env;
	t_list *tmp;
	int flag_env;

	flag_env = -1;
	tmp = start_lst_token_retokenized;
	if(tmp != NULL)
	{
		if(flag_env == -1)
		{
			if(flag_env == -1 && (ft_strcmp("env", tmp->content) == 0))
			{
				flag_env = 1;
				simpleCmd->is_builtin = 1;
				simpleCmd->builtin = env; // ==
				if(tmp->next == NULL)
					simpleCmd->env_no_option = 1;
				else
					tmp = tmp->next;
				if(cmd->nb_of_simpleCmds == 1)
					{
						//simpleCmd->env_solo = 1;
						simpleCmd->builtin_solo = 1;
					}
			}
			else
				flag_env = 0;
		}
	}
}
