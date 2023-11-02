/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 17:44:17 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/29 17:44:29 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	ft_reset_missing_oldpwd(char *cwd, char *oldpwd, t_listenvp *new, \
t_data_env *data_env)
{
	oldpwd = ft_strjoin("OLDPWD=", cwd);
	new = ft_lstnew_for_lst_envp(oldpwd);
	ft_lstadd_back_envp(&data_env->lst_envp, new);
	if (ft_is_var_to_unset_already_in_lst_envp_d(data_env, "OLDPWD") == 1)
	{
		new = ft_lstnew_for_lst_envp(oldpwd);
		ft_lstadd_back_envp(&data_env->lst_envp_d, new);
	}
	ft_free_struct_str(&oldpwd);
}

void	ft_remove_oldpwd(t_data_env *data_env)
{
	get_var_in_lst_envp_for_cd(data_env, "OLDPWD");
	ft_is_var_to_unset_already_in_lst_envp(data_env, "OLDPWD");
	ft_is_var_to_unset_already_in_lst_envp_d(data_env, "OLDPWD");
	ft_add_var_oldpwd_to_lst_envp_d_for_cd_dash("OLDPWD", data_env->lst_envp_d);
	data_env->flag_oldpwd_istoremove = 0;
}

void	ft_update_oldpwd_and_pwd_in_lst_envp_for_cd_dash_dash(t_data_env \
		*data_env, char *cwd)
{
	char		*tmp_pwd_value;
	char		*oldpwd;
	char		*newpwd;
	t_listenvp	*new;

	new = NULL;
	oldpwd = NULL;
	newpwd = NULL;
	tmp_pwd_value = get_var_in_lst_envp_for_cd(data_env, "PWD");
	if (tmp_pwd_value == NULL)
	{
		if (data_env->flag_oldpwd_istoremove == 1)
			ft_remove_oldpwd(data_env);
		else if (data_env->flag_oldpwd_istoremove == 0)
		{
			if (ft_lstfind_content(data_env->lst_envp, "OLDPWD=") == NULL)
				ft_reset_missing_oldpwd(cwd, oldpwd, new, data_env);
		}
		return ;
	}
	oldpwd = ft_strjoin("OLDPWD=", tmp_pwd_value);
	newpwd = ft_strjoin("PWD=", cwd);
	ft_lst_add_back_var(data_env, oldpwd, newpwd, new);
}

void	ft_update_oldpwd_and_pwd_in_lst_envp_for_cd_dash(t_data_env *data_env, \
char *cwd, t_simpleCmd *simpleCmd)
{
	char		*tmp_pwd_value;
	char		*oldpwd;
	char		*newpwd;
	t_listenvp	*new;

	new = NULL;
	oldpwd = NULL;
	newpwd = NULL;
	tmp_pwd_value = get_var_in_lst_envp_for_cd(data_env, "PWD");
	if (tmp_pwd_value == NULL)
	{
		oldpwd = get_var_in_lst_envp_for_cd(data_env, "OLDPWD");
		simpleCmd->oldpwd = ft_strdup(oldpwd);
		ft_is_var_to_unset_already_in_lst_envp(data_env, "OLDPWD");
		ft_is_var_to_unset_already_in_lst_envp_d(data_env, "OLDPWD");
		ft_add_var_oldpwd_to_lst_envp_d_for_cd_dash("OLDPWD", \
data_env->lst_envp_d);
		return ;
	}
	oldpwd = ft_strjoin("OLDPWD=", tmp_pwd_value);
	newpwd = ft_strjoin("PWD=", cwd);
	simpleCmd->oldpwd = ft_strdup(cwd);
	ft_lst_add_back_var(data_env, oldpwd, newpwd, new);
}

void	ft_update_oldpwd_and_pwd_in_lst_envp(t_data_env *data_env, char *cwd)
{
	char		*pwd_value;
	char		*oldpwd;
	char		*newpwd;
	t_listenvp	*new;

	new = NULL;
	oldpwd = NULL;
	newpwd = NULL;
	pwd_value = get_var_in_lst_envp_for_cd(data_env, "PWD");
	if (pwd_value == NULL)
		return ;
	oldpwd = ft_strjoin("OLDPWD=", pwd_value);
	newpwd = ft_strjoin("PWD=", cwd);
	ft_lst_add_back_var(data_env, oldpwd, newpwd, new);
}
