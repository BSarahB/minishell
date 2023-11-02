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

void	ft_check_env_token(t_param_en *para, t_simpleCmd *simpleCmd)
{
	while (para->tmp != NULL && para->tmp->position \
				<= simpleCmd->end_simplecmd_pos && para->flag_env == 1)
	{
		if (simpleCmd->nofile == 1)
			break ;
		if (simpleCmd->nb_of_tokens_in_simplecmd >= 2)
		{
			ft_error_msg6(para->tmp->content);
			simpleCmd->exit_code = 127;
			break ;
		}
		para->tmp = para->tmp->next;
	}
}

void	ft_check_env_first_token(t_param_en *para, \
t_cmd *cmd, t_simpleCmd *simpleCmd)
{
	if (para->tmp != NULL)
	{
		if (para->flag_env == -1 && (ft_strcmp("env", para->tmp->content) == 0))
		{
			para->flag_env = 1;
			simpleCmd->is_builtin = 1;
			simpleCmd->builtin = env;
			if (para->tmp->next == NULL)
				simpleCmd->env_no_option = 1;
			else
				para->tmp = para->tmp->next;
			if (cmd->nb_of_simplecmds == 1)
				simpleCmd->builtin_solo = 1;
		}
		else
			para->flag_env = 0;
	}
}

void	ft_check_env(t_cmd *cmd, t_list *start_lst_token_retokenized, \
t_simpleCmd *simpleCmd, t_data_env *data_env)
{
	t_param_en	para;

	(void)data_env;
	para.tmp = NULL;
	para.flag_env = -1;
	para.tmp = start_lst_token_retokenized;
	ft_check_env_first_token(&para, cmd, simpleCmd);
	ft_check_env_token(&para, simpleCmd);
}
