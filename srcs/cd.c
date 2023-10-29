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

void ft_check_cd(t_cmd *cmd, t_list *start_lst_token_retokenized, t_simpleCmd *simpleCmd, t_data_env *data_env)
{
	(void)cmd;
	(void)start_lst_token_retokenized;
	(void)simpleCmd;
	(void)data_env;
	t_list *tmp;
	int flag_cd;

	flag_cd = -1;
	tmp = start_lst_token_retokenized;
	if(tmp != NULL)
	{
		if(flag_cd == -1)
		{
			if(flag_cd == -1 && (ft_strcmp("cd", tmp->content) == 0))
			{
				flag_cd = 1;
				simpleCmd->is_builtin = 1;
				simpleCmd->builtin = cd; // ==
				if(cmd->nb_of_simpleCmds == 1)
					{
						simpleCmd->cd_solo = 1;
						simpleCmd->builtin_solo = 1;
					}
				if(tmp->next == NULL)
				{
					simpleCmd->cd_no_option = 1;
					//if(cd sans option) -> aller dans home
					ft_cd_no_option(data_env, "HOME", simpleCmd);
				}
				else
					tmp = tmp->next;
			}
			else
				flag_cd = 0;
		}
	}
	while (tmp != NULL && tmp->position <= simpleCmd->end_simpleCmd_pos && simpleCmd->cd_no_option == 0 && flag_cd == 1)
	{
		if(simpleCmd->nofile == 1)
			break;
		if(simpleCmd->nb_of_tokens_in_simpleCmd > 2)
		{
				ft_error_msg4("cd");
				simpleCmd->exit_code = 1;
				break;
		}
		//if cd [-] TODO DANS L EXECUTION IL FUDRA AFFICHER SUR STDOUT LE MESSAGE D AFICHAGE DU CHEMIN DU OLDPWD (ainsi si c est une redir dans un outfile il faudra la mettre dans l outfile)
		if(ft_strcmp(tmp->content, "-") == 0)
			ft_cd_option_dash(data_env, "OLDPWD", simpleCmd);
		// if cd [--] // ca fait retourner a HOME
		else if(ft_strcmp(tmp->content, "--") == 0 || ft_strcmp(tmp->content, "~") == 0)  //TODO GERER || ft_strcmp(tmp->content, "~") la tilde ->cqfd car si on retire HOME la tilde n est pas impactee
			ft_cd_option_dash_dash(data_env, "HOME", simpleCmd);	
		else if(ft_strcmp(tmp->content, "/") == 0)
			ft_cd_option_slash(data_env, "/", simpleCmd);
		else
			ft_check_path(data_env, tmp->content, simpleCmd);
		tmp = tmp->next;
	}
}