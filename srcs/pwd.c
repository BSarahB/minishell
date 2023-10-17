/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 14:16:06 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/17 14:16:17 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int ft_get_pwd(t_data_env *data_env, t_simpleCmd *simpleCmd)
{
	char	buf[1096];
	char    *cwd;
	(void)data_env;
	(void)simpleCmd;

    cwd = NULL;
	cwd = getcwd(buf, 1096); //si error du return : errno is set
	if (!cwd)
	{
		ft_putstr_fd("cd: error retrieving current directory, getcwd: cannot access parent directory", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		return(errno);
	}
    simpleCmd->pwd = ft_strdup(cwd);
	return (0);
}



void ft_check_pwd(t_cmd *cmd, t_list *start_lst_token_retokenized, t_simpleCmd *simpleCmd, t_data_env *data_env)
{
	
	(void)cmd;
	(void)start_lst_token_retokenized;
	(void)simpleCmd;
	(void)data_env;
	t_list *tmp;

	tmp = start_lst_token_retokenized;
	if(tmp != NULL)
	{
		
			if(ft_strcmp("pwd", tmp->content) == 0)
			{
				simpleCmd->is_builtin = 1;
				simpleCmd->builtin = pwd; // ==2
				if(cmd->nb_of_simpleCmds == 1)
					{
						simpleCmd->builtin_solo = 1;
					}
                ft_get_pwd(data_env, simpleCmd);
			}
	}
}
