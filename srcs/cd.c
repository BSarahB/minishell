/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 10:59:44 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/13 11:00:04 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	ft_check_cd_token(t_param_b *para, t_simpleCmd *simpleCmd, \
t_data_env *data_env)
{
	while (para->tmp != NULL && para->tmp->position \
		<= simpleCmd->end_simplecmd_pos && simpleCmd->cd_no_option == 0 && \
para->flag_cd == 1)
	{
		if (simpleCmd->nofile == 1)
			break ;
		if (simpleCmd->nb_of_tokens_in_simplecmd > 2)
		{
			ft_error_msg4("cd");
			simpleCmd->exit_code = 1;
			break ;
		}
		if (ft_strcmp(para->tmp->content, "-") == 0)
			ft_cd_option_dash(data_env, "OLDPWD", simpleCmd);
		else if (ft_strcmp(para->tmp->content, "--") == 0 || \
ft_strcmp(para->tmp->content, "~") == 0)
			ft_cd_option_dash_dash(data_env, "HOME", simpleCmd);
		else if (ft_strcmp(para->tmp->content, "/") == 0)
			ft_cd_option_slash(data_env, "/", simpleCmd);
		else
			ft_check_path(data_env, para->tmp->content, simpleCmd);
		para->tmp = para->tmp->next;
	}
}

void	ft_check_cd_first_token(t_param_b *para, \
t_cmd *cmd, t_simpleCmd *simpleCmd, t_data_env *data_env)
{
	if (para->tmp != NULL)
	{
		if (para->flag_cd == -1 && (ft_strcmp("cd", para->tmp->content) == 0))
		{
			para->flag_cd = 1;
			simpleCmd->is_builtin = 1;
			simpleCmd->builtin = cd;
			if (cmd->nb_of_simplecmds == 1)
			{
				simpleCmd->cd_solo = 1;
				simpleCmd->builtin_solo = 1;
			}
			if (para->tmp->next == NULL)
			{
				simpleCmd->cd_no_option = 1;
				ft_cd_no_option(data_env, "HOME", simpleCmd);
			}
			else
				para->tmp = para->tmp->next;
		}
		else
			para->flag_cd = 0;
	}
}

void	ft_check_cd(t_cmd *cmd, t_list *start_lst_token_retokenized, \
t_simpleCmd *simpleCmd, t_data_env *data_env)
{
	t_param_b	para;

	para.tmp = NULL;
	para.flag_cd = -1;
	para.tmp = start_lst_token_retokenized;
	ft_check_cd_first_token(&para, cmd, simpleCmd, data_env);
	ft_check_cd_token(&para, simpleCmd, data_env);
}
