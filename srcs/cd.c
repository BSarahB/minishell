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


int ft_is_var_already_in_lst_envp_for_cd(t_listenvp *lst_envp, char *str)
{
	t_listenvp *tmp;
	int n;
	char *new_key_val;
	char *key;
	char *key_input;

	tmp = lst_envp;
	n = 0;
	new_key_val = NULL;
	key = NULL;
	key_input = NULL;
	while(tmp)
	{
		n = ft_count_n_for_key(tmp->key_value);
		key = ft_strndup(tmp->key_value, n);
		n = ft_count_n_for_key(str);
		key_input = ft_strndup(str, n);
		if(ft_strcmp(key, key_input) == 0)
		{
			new_key_val = ft_strdup(str);
			free(str);
			str = NULL;
			ft_update_string(&tmp->key_value, new_key_val);
			//ft_lstfind_and_overwrite(&lst_envp, tmp->key_value);
			free(key);
			key = NULL;
			free(key_input);
			key_input = NULL;
			return(1);
		}
		else
		{
			free(key);
			key = NULL;
			free(key_input);
			key_input = NULL;
			tmp = tmp->next;
		}
	}
	if(key != NULL)
	{
		free(key);
		key = NULL;
	}
	if(key_input != NULL)
	{
		free(key_input);
		key_input = NULL;
	}
	if(str != NULL)
	{
		free(str);
		str = NULL;
	}
	return(0);
}

//je peux utiliser la meme fonction du dessous plutot que d en avoir 2. mais pour l instant je laisse comme ca

void ft_update_oldpwd_and_pwd_in_lst_envp_for_cd_dash(t_data_env *data_env, char *cwd)
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

void ft_update_oldpwd_and_pwd_in_lst_envp(t_data_env *data_env, char *cwd)
{
	(void)data_env;
	(void)cwd;
	char *pwd_value;
	char *oldpwd;
	char *newpwd;
	t_listenvp *new;

	new = NULL;
	oldpwd =NULL;
	newpwd = NULL;
	pwd_value = get_var_in_lst_envp_for_cd(data_env, "PWD");

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


int ft_change_directory_for_cd_dash(t_data_env *data_env, char *new_path, t_simpleCmd *simpleCmd)
{
	char	buf[1096];
	char	*cwd;
	(void)data_env;
	(void)simpleCmd;
	cwd = NULL;
	if (chdir(new_path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);//mettre en place TODO l erreur ERRNO le msg approprie errno
		strerror(errno);
		ft_putstr_fd("\n", 2);//mettre en place TODO l erreur ERRNO le msg approprie errno
		return (errno);
	}
	cwd = getcwd(buf, 1096); //si error du return : errno is set
	if (!cwd)
	{
		ft_putstr_fd("cd: error retrieving current directory, getcwd: cannot access parent directories", 2);
		strerror(errno);
		return(errno);
	}

	//sur cd - on affiche le OLDPWD en sortie standard
//	simpleCmd->oldpwd = ft_strdup(cwd);
	ft_update_oldpwd_and_pwd_in_lst_envp_for_cd_dash(data_env, cwd);//faire une copie de PWD avant de la modifier en cwd
	//free(cwd);
	//cwd =NULL;

	return (0);
}


int ft_change_directory(t_data_env *data_env, char *new_path)
{
	char	buf[1096];
	char	*cwd;

	cwd = NULL;
	if (chdir(new_path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);//mettre en place TODO l erreur ERRNO le msg approprie errno
		strerror(errno);
		ft_putstr_fd("\n", 2);//mettre en place TODO l erreur ERRNO le msg approprie errno
		return (errno);
	}
	cwd = getcwd(buf, 1096); //si error du return : errno is set
	if (!cwd)
	{
		ft_putstr_fd("cd: error retrieving current directory, getcwd: cannot access parent directories", 2);
		strerror(errno);
		return(errno);
	}
//	else
	//	cwd = ft_strdup(buf);
	ft_update_oldpwd_and_pwd_in_lst_envp(data_env, cwd);
	return (1);
}



int	ft_is_space(int c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\r'
		|| c == '\v' || c == '\f')
		return (c);
	return (0);
}

int ft_check_is_space(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if(ft_is_space(str[i]) != 0)
			return (1);
		i++;
	}
	return(0);
}

char *ft_strchr(const char *s, int c)
{
	char *str;
	str = (char *)s;

	while(*str && *str != (unsigned char)c)
		str++;
	if(*str == (unsigned char)c || c == '\0')
		return(str);
	return (NULL);
}


char	*get_var_in_lst_envp_for_cd(t_data_env *data_env, char *str)
{
	char	*key;
	t_listenvp *tmp;

	key= ft_strjoin(str, "=");
	tmp = data_env->lst_envp;
	if (!key)
		return (NULL);
	while (tmp)
	{
		if (ft_strncmp(key, tmp->key_value, ft_strlen(key)) == 0) //a la difference des autres fonctions , ici je choisis moi meme la vairable qui est HOME dans les autres cas de figure pour les codes prcedents la variable etait imposee par l entree user et stockee sous forme de KEY=VALUE sur un seul token
		{
			free(key);
			key = NULL;
			return (ft_strchr(tmp->key_value, '=') + 1);
		}
		tmp = tmp->next;
	}
	free(key);
	key = NULL;
	return (NULL);
}




void	ft_cd_option_dash_dash(t_data_env *data_env, char *str, t_simpleCmd *simpleCmd)
{
	char *new_path;

	new_path = get_var_in_lst_envp_for_cd(data_env, str);
	//on a recupere new_path /home/s
	if(!new_path) //on anticipe les unset HOME ou les export HOME="" ou export HOME="
	{
		ft_putstr_fd("bash : cd : HOME not set \n", 2);
		return;
	}
	else if(new_path[0] == '\0')
		return;
	else if(ft_check_is_space(new_path) != 0)
	{
		ft_error_msg5(new_path);
		return;
	}
	else //si tout est OK et que HOME a une bonne value
	{
		if(simpleCmd->cd_solo == 1)
			ft_change_directory_for_cd_dash(data_env, new_path, simpleCmd); //PWD devient OLD PWD et OLD PWD DEVIENT
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
		return;
	}
	else if(new_path[0] == '\0')
		return;
	else if(ft_check_is_space(new_path) != 0)
	{
		ft_error_msg5(new_path);
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
		return;
	}
	else if(new_path[0] == '\0')
		return;
	else if(ft_check_is_space(new_path) != 0)
	{
		ft_error_msg5(new_path);
		return;
	}
	else //si tout est OK et que HOME a une bonne value
	{
		if(simpleCmd->cd_solo == 1)
			ft_change_directory(data_env, new_path);
	}


}


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

	while (tmp != NULL && tmp->position < simpleCmd->end_simpleCmd_pos && simpleCmd->cd_no_option == 0 && flag_cd == 1)
	{
		if(simpleCmd->nofile == 1)
			break;
		//si >2 args
		if(simpleCmd->nb_of_tokens_in_simpleCmd > 2)
		{
				ft_error_msg4("cd");
				break;
		}

		//if cd [-] TODO DANS L EXECUTION IL FUDRA AFFICHER SUR STDOUT LE MESSAGE D AFICHAGE DU CHEMIN DU OLDPWD (ainsi si c est une redir dans un outfile il faudra la mettre dans l outfile)
		if(ft_strcmp(tmp->content, "-") == 0)
		{
			//aller a OLDPWD. la position precedente, se proteger de si OLDPWD a ete unset par ex.
			ft_cd_option_dash(data_env, "OLDPWD", simpleCmd);
		}

		// if cd [--] // ca fait retourner a HOME
		else if(ft_strcmp(tmp->content, "--") == 0)
		{

			ft_cd_option_dash_dash(data_env, "HOME", simpleCmd);
		}

		else if(ft_strcmp(tmp->content, ".") == 0)
		{
			break;
		}
		else if(ft_strcmp(tmp->content, "..") == 0)
		{

		}
		else
		{
			//ft_check_path(tmp->content);
		}
		tmp = tmp->next;
	}

}











