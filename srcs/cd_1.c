/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 17:07:35 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/29 17:07:52 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	ft_cd_option_slash(t_data_env *data_env, char *str, t_simpleCmd *simpleCmd)
{
		if(simpleCmd->cd_solo == 1)
			ft_change_directory_for_cd_dash_dash(data_env, str , simpleCmd); //PWD devient OLD PWD et OLD PWD DEVIENT
}

void	ft_cd_option_dash_dash(t_data_env *data_env, char *str, t_simpleCmd *simpleCmd)
{
	char *new_path;

	new_path = get_var_in_lst_envp_for_cd(data_env, str);
	//on a recupere new_path /home/s
	if(!new_path) //on anticipe les unset HOME ou les export HOME="" ou export HOME="
	{
		ft_putstr_fd("bash : cd : HOME not set \n", 2);
		simpleCmd->exit_code = 1;
		return;
	}
	else if(new_path[0] == '\0')
		return;
	else if(ft_check_is_space(new_path) != 0)
	{
		ft_error_msg5(new_path);
		simpleCmd->exit_code = 1;
		return;
	}
	else //si tout est OK et que HOME a une bonne value
	{
		if(simpleCmd->cd_solo == 1)
			ft_change_directory_for_cd_dash_dash(data_env, new_path, simpleCmd); //PWD devient OLD PWD et OLD PWD DEVIENT
	}
}

void	ft_cd_option_dash(t_data_env *data_env, char *str, t_simpleCmd *simpleCmd)
{
	char *new_path;

	new_path = get_var_in_lst_envp_for_cd(data_env, str);
	//on a recupere new_path /home/s
	if(!new_path) //on anticipe les unset HOME ou les export HOME="" ou export HOME="
	{
		ft_putstr_fd("bash : cd : OLDPWD not set \n", 2);
		simpleCmd->exit_code = 1;
		return;
	}
	else if(new_path[0] == '\0')
		return;
	else if(ft_check_is_space(new_path) != 0)
	{
		ft_error_msg5(new_path);
		simpleCmd->exit_code = 1;
		return;
	}
	else //si tout est OK et que HOME a une bonne value
	{
		if(simpleCmd->cd_solo == 1)
			ft_change_directory_for_cd_dash(data_env, new_path, simpleCmd); //PWD devient OLD PWD et OLD PWD DEVIENT
	}
}

void	ft_cd_no_option(t_data_env *data_env, char *str, t_simpleCmd *simpleCmd)
{
	char *new_path;

	new_path = get_var_in_lst_envp_for_cd(data_env, str);
	//on a recupere new_path /home/s
	if(!new_path) //on anticipe les unset HOME ou les export HOME="" ou export HOME="
	{
		ft_putstr_fd("bash : cd : HOME not set \n", 2);
		simpleCmd->exit_code = 1;
		return;
	}
	else if(new_path[0] == '\0')
		return;
	else if(ft_check_is_space(new_path) != 0)
	{
		ft_error_msg5(new_path);
		simpleCmd->exit_code = 1;
		return;
	}
	else //si tout est OK et que HOME a une bonne value
	{
		if(simpleCmd->cd_solo == 1)
			ft_change_directory(data_env, new_path, simpleCmd);
	}
}