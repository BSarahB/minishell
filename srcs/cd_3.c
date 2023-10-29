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

void ft_add_var_oldpwd_to_lst_envp_d_for_cd_dash(char *str, t_listenvp *lst_envp_d)
{
	t_listenvp *new;

	new = NULL;
	if(ft_is_var_already_in_lst_envp_d(lst_envp_d, str) == 0)
	{
		new = ft_lstnew_for_lst_envp(str);
		ft_lstadd_back_envp(&lst_envp_d, new);
	}
}

void ft_update_oldpwd_and_pwd_in_lst_envp_for_cd_dash_dash(t_data_env *data_env, char *cwd, t_simpleCmd *simpleCmd)
{
	(void)data_env;
	(void)cwd;
	(void)simpleCmd;
	char *tmp_pwd_value;
	char *oldpwd;
	char *newpwd;
	t_listenvp *new;

	new = NULL;
	oldpwd =NULL;
	newpwd = NULL;
	tmp_pwd_value = get_var_in_lst_envp_for_cd(data_env, "PWD");

	//PWD VA DEVENIR OLDPWD et CWD VA DEVENIR PWD     dans le cas de cd - ->
	if(tmp_pwd_value == NULL) //cas ou on  unset PWD par  ex t que qu on vt faire cd - le OLDPWD va devoir disparaitre
		{
			if(data_env->flag_oldpwd_istoremove == 1)
			{
				oldpwd = get_var_in_lst_envp_for_cd(data_env, "OLDPWD");
				ft_is_var_to_unset_already_in_lst_envp(data_env, "OLDPWD");
				ft_is_var_to_unset_already_in_lst_envp_d(data_env, "OLDPWD");
				ft_add_var_oldpwd_to_lst_envp_d_for_cd_dash("OLDPWD", data_env->lst_envp_d); // on affiche que OLDPWD sans rien dans le lst_envpdon envoie tmp content
				data_env->flag_oldpwd_istoremove = 0;
				return;
			}

			if (data_env->flag_oldpwd_istoremove == 0) //EN FAIT ON VA REMOVE OLDPWDcela signifie que on n a pas remove oldpwd puisquil n y etait pas. (o la remove au coup d avant car PWD etait unset)
			{
				//donc si OLDPWD n existe pas on met HOME pour indiquer notre position precedente il faut aussi le reintroduire dans lst_envp_d
				//reintroduire OLDPWD avec cwd
				if(ft_lstfind_content(data_env->lst_envp, "OLDPWD=") == NULL)
				{
					oldpwd =ft_strjoin("OLDPWD=", cwd);
					new = ft_lstnew_for_lst_envp(oldpwd);
					ft_lstadd_back_envp(&data_env->lst_envp, new);
					if (ft_is_var_to_unset_already_in_lst_envp_d(data_env, "OLDPWD") == 1) //on delete OLDPWD dans lst envp d et on va rajouter la version key value complete
						{
							new = ft_lstnew_for_lst_envp(oldpwd);
							ft_lstadd_back_envp(&data_env->lst_envp_d, new);
						}
					free(oldpwd);
					oldpwd = NULL;
					return;
				}
			}
			return;
		}
	oldpwd =ft_strjoin("OLDPWD=", tmp_pwd_value);
	newpwd = ft_strjoin("PWD=", cwd);
	if(ft_is_var_already_in_lst_envp_for_cd(data_env->lst_envp, oldpwd) == 0)
	{
		new = ft_lstnew_for_lst_envp(oldpwd);
		ft_lstadd_back_envp(&data_env->lst_envp, new);
	}
	if(ft_is_var_already_in_lst_envp_for_cd(data_env->lst_envp, newpwd) == 0)
	{
		new = ft_lstnew_for_lst_envp(newpwd);
		ft_lstadd_back_envp(&data_env->lst_envp, new);
	}

}

void ft_update_oldpwd_and_pwd_in_lst_envp_for_cd_dash(t_data_env *data_env, char *cwd, t_simpleCmd *simpleCmd)
{
	(void)data_env;
	(void)cwd;
	char *tmp_pwd_value;
	char *oldpwd;
	char *newpwd;
	t_listenvp *new;

	new = NULL;
	oldpwd =NULL;
	newpwd = NULL;
	tmp_pwd_value = get_var_in_lst_envp_for_cd(data_env, "PWD");
	//PWD VA DEVENIR OLDPWD et CWD VA DEVENIR PWD     dans le cas de cd - ->
	if(tmp_pwd_value == NULL) //cas ou on  unset PWD par  ex t que qu on vt faire cd - le OLDPWD va devoir disparaitre
		{
			oldpwd = get_var_in_lst_envp_for_cd(data_env, "OLDPWD");
			simpleCmd->oldpwd = ft_strdup(oldpwd);
			ft_is_var_to_unset_already_in_lst_envp(data_env, "OLDPWD");
			ft_is_var_to_unset_already_in_lst_envp_d(data_env, "OLDPWD");
			ft_add_var_oldpwd_to_lst_envp_d_for_cd_dash("OLDPWD", data_env->lst_envp_d); // on affiche que OLDPWD sans rien dans le lst_envpdon envoie tmp content
			return;
		}
	oldpwd =ft_strjoin("OLDPWD=", tmp_pwd_value);
	newpwd = ft_strjoin("PWD=", cwd);
	simpleCmd->oldpwd = ft_strdup(cwd);
	if(ft_is_var_already_in_lst_envp_for_cd(data_env->lst_envp, oldpwd) == 0) //FIXME bien ecrir pour ecraser les valeurs : au lieu de oldpwd ilf aut envoyer "OLDPWD"
	{
		new = ft_lstnew_for_lst_envp(oldpwd);
		ft_lstadd_back_envp(&data_env->lst_envp, new);
	}
	if(ft_is_var_already_in_lst_envp_for_cd(data_env->lst_envp, newpwd) == 0)
	{
		new = ft_lstnew_for_lst_envp(newpwd);
		ft_lstadd_back_envp(&data_env->lst_envp, new);
	}
}

void ft_update_oldpwd_and_pwd_in_lst_envp(t_data_env *data_env, char *cwd)
{
	char *pwd_value;
	char *oldpwd;
	char *newpwd;
	t_listenvp *new;

	new = NULL;
	oldpwd =NULL;
	newpwd = NULL;
	pwd_value = get_var_in_lst_envp_for_cd(data_env, "PWD");
	if(pwd_value == NULL)
		return;
	//PWD VA DEVENIR OLDPWD et CWD VA DEVENIR PWD     dans le cas de cd - ->
	oldpwd =ft_strjoin("OLDPWD=", pwd_value);
	newpwd = ft_strjoin("PWD=", cwd);
	if(ft_is_var_already_in_lst_envp_for_cd(data_env->lst_envp, oldpwd) == 0)
	{
		new = ft_lstnew_for_lst_envp(oldpwd);
		ft_lstadd_back_envp(&data_env->lst_envp, new);
	}
	if(ft_is_var_already_in_lst_envp_for_cd(data_env->lst_envp, newpwd) == 0)
	{
		new = ft_lstnew_for_lst_envp(newpwd);
		ft_lstadd_back_envp(&data_env->lst_envp, new);
	}
}