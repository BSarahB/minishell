/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 09:17:57 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/04 09:18:11 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	ft_check_export_token(t_param_e *para, t_simpleCmd *simpleCmd, \
t_data_env *data_env)
{
	while (para->tmp != NULL && para->tmp->position \
		<= simpleCmd->end_simplecmd_pos && simpleCmd->export_no_option == 0 \
	&& para->flag_export == 1)
	{
		if (simpleCmd->nofile == 1)
			break ;
		if (simpleCmd->export_solo == 1)
		{
			if (ft_key_value_is_valid(para->tmp, simpleCmd) == 1)
			{
				if (ft_check_format_is_key_value(para->tmp) == 1)
				{
					ft_add_var_to_lst_envp(para->tmp, data_env->lst_envp);
					ft_add_var_to_lst_envp(para->tmp, data_env->lst_envp_d);
				}
				if (ft_check_format_is_key_value(para->tmp) == 0)
					ft_add_var_to_lst_envp_d(para->tmp, data_env->lst_envp_d);
			}
		}
		para->tmp = para->tmp->next;
	}
}

void	ft_check_export_first_token(t_param_e *para, \
t_cmd *cmd, t_simpleCmd *simpleCmd)
{
	if (para->tmp != NULL)
	{
		if (para->flag_export == -1 && (ft_strcmp("export", \
para->tmp->content) == 0))
		{
			para->flag_export = 1;
			simpleCmd->is_builtin = 1;
			simpleCmd->builtin = export;
			if (para->tmp->next == NULL)
				simpleCmd->export_no_option = 1;
			else
				para->tmp = para->tmp->next;
			if (cmd->nb_of_simplecmds == 1)
			{
				simpleCmd->export_solo = 1;
				simpleCmd->builtin_solo = 1;
			}
		}
		else
			para->flag_export = 0;
	}
}

void	ft_check_export(t_cmd *cmd, t_list *start_lst_token_retokenized, \
t_simpleCmd *simpleCmd, t_data_env *data_env)
{
	t_param_e	para;

	para.tmp = NULL;
	para.flag_export = -1;
	para.tmp = start_lst_token_retokenized;
	ft_check_export_first_token(&para, cmd, simpleCmd);
	ft_check_export_token(&para, simpleCmd, data_env);
}
