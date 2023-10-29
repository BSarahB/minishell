/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 13:49:09 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/12 13:49:23 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void ft_check_echo(t_cmd *cmd, t_list *start_lst_token_retokenized, t_simpleCmd *simpleCmd, t_data_env *data_env)
{
	(void)cmd;
	(void)start_lst_token_retokenized;
	(void)simpleCmd;
	(void)data_env;
	t_list *tmp;

	tmp = start_lst_token_retokenized;
	if(tmp != NULL && simpleCmd->builtin == echo)
	{
		if(tmp->next == NULL)
			simpleCmd->echo_no_option = 1;
		if(cmd->nb_of_simpleCmds == 1)
			simpleCmd->builtin_solo = 1;
	}
}
