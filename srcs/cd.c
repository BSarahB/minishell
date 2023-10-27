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

void ft_add_var_oldpwd_to_lst_envp_d_for_cd_dash(char *str, t_listenvp *lst_envp_d)
{
	t_listenvp *new;

	new = NULL;
	if(ft_is_var_already_in_lst_envp_d(lst_envp_d, str) == 0)
	{
		new = ft_lstnew_for_lst_envp(str);
		ft_lstadd_back_envp(&lst_envp_d, new);
	}
//		ft_aff_list_envp_d_sur_char_content(lst_envp_d); // pour void		*content; de type char *
}




t_listenvp *ft_lstfind_content(t_listenvp *alst, char *keyequal)
{
	t_listenvp *tmp;
	tmp = alst;
	while(tmp != NULL)
	{
		if(ft_strncmp(keyequal, tmp->key_value, ft_strlen(keyequal)) == 0)
			return(tmp);
		tmp = tmp->next;
	}
	return (NULL);
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
	//t_listenvp *new2;


	new = NULL;
	//new2 = NULL;
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

/*
int ft_change_directory_for_cd_dot(t_data_env *data_env, char *cwd, t_simpleCmd *simpleCmd)
{
	(void)data_env;
	(void)simpleCmd;
	if (chdir(cwd) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);//mettre en place TODO l erreur ERRNO le msg approprie errno
		if(errno == 14)
			errno = ENOENT; // The directory specified in path does not exist.
		ft_putstr_fd(cwd, 2);
		ft_putstr_fd(": ", 2);

		ft_putstr_fd(strerror(errno), 2);
		//printf("errno = %d\n", errno);
		ft_putstr_fd("\n", 2);//mettre en place TODO l erreur ERRNO le msg approprie errno
		return (errno);
	}
	if (!cwd)
	{
		ft_putstr_fd("cd: error retrieving current directory, getcwd: cannot access parent directory", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		return(errno);
	}

	//sur cd - on affiche le OLDPWD en sortie standard
//	simpleCmd->oldpwd = ft_strdup(cwd);
	ft_update_oldpwd_and_pwd_in_lst_envp_for_cd_dash_dash(data_env, cwd, simpleCmd);//faire une copie de PWD avant de la modifier en cwd
	//free(cwd);
	//cwd =NULL;

	return (0);
}

*/

int ft_change_directory_for_cd_slash(t_data_env *data_env, char *new_path, t_simpleCmd *simpleCmd)
{
	char	buf[1096];
	char	*cwd;
	(void)data_env;
	(void)simpleCmd;
	cwd = NULL;
	if (chdir(new_path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);//mettre en place TODO l erreur ERRNO le msg approprie errno
		if(errno == 14 || errno == 116)
			errno = ENOENT; // The directory specified in path does not exist.
		ft_putstr_fd(new_path, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);//mettre en place TODO l erreur ERRNO le msg approprie errno
		simpleCmd->exit_code = 1;
		return (errno);
	}
	cwd = getcwd(buf, 1096); //si error du return : errno is set
	if (!cwd)
	{
		ft_putstr_fd("cd: error retrieving current directory, getcwd: cannot access parent directory", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		simpleCmd->exit_code = 1;
		return(errno);

	}

	//sur cd - on affiche le OLDPWD en sortie standard
//	simpleCmd->oldpwd = ft_strdup(cwd);
	ft_update_oldpwd_and_pwd_in_lst_envp_for_cd_dash_dash(data_env, cwd, simpleCmd);//faire une copie de PWD avant de la modifier en cwd
	//free(cwd);
	//cwd =NULL;

	return (0);
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
		if(errno == 14 || errno == 116)
			errno = ENOENT; // The directory specified in path does not exist.
		ft_putstr_fd(new_path, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);//mettre en place TODO l erreur ERRNO le msg approprie errno
		simpleCmd->exit_code = 1;
		return (errno);
	}
	cwd = getcwd(buf, 1096); //si error du return : errno is set
	if (!cwd)
	{
		ft_putstr_fd("cd: error retrieving current directory, getcwd: cannot access parent directory", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		simpleCmd->exit_code = 1;
		return(errno);

	}

	//sur cd - on affiche le OLDPWD en sortie standard
//	simpleCmd->oldpwd = ft_strdup(cwd);
	ft_update_oldpwd_and_pwd_in_lst_envp_for_cd_dash(data_env, cwd, simpleCmd);//faire une copie de PWD avant de la modifier en cwd
	//free(cwd);
	//cwd =NULL;

	return (0);
}


int ft_change_directory(t_data_env *data_env, char *new_path, t_simpleCmd *simpleCmd)
{
	char	buf[1096];
	char	*cwd;

	cwd = NULL;
	if (chdir(new_path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);//mettre en place TODO l erreur ERRNO le msg approprie errno
		if(errno == 14 || errno == 116)
			errno = ENOENT; // The directory specified in path does not exist.
		ft_putstr_fd(new_path, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);//mettre en place TODO l erreur ERRNO le msg approprie errno
		simpleCmd->exit_code = 1;
		return (errno);
	}
	cwd = getcwd(buf, 1096); //si error du return : errno is set
	if (!cwd)
	{
		ft_putstr_fd("cd: error retrieving current directory, getcwd: cannot access parent directory", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		simpleCmd->exit_code = 1;
		return(errno);

	}
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


/*

void	ft_cd_option_dot(t_data_env *data_env, t_simpleCmd *simpleCmd)
{
	char	*cwd;
	char	buf[1096];

	
	cwd = NULL;
	cwd = getcwd(buf, 1096); //si error du return : errno is set

		if(simpleCmd->cd_solo == 1)
			ft_change_directory_for_cd_dot(data_env, cwd, simpleCmd); //PWD devient OLD PWD et OLD PWD DEVIENT
}


*/

int ft_change_directory_for_cd_dash_dash(t_data_env *data_env, char *new_path, t_simpleCmd *simpleCmd)
{
	char	buf[1096];
	char	*cwd;
	(void)data_env;
	(void)simpleCmd;
	cwd = NULL;
	if (chdir(new_path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);//mettre en place TODO l erreur ERRNO le msg approprie errno
		if(errno == 14 || errno == 116)
			errno = ENOENT; // The directory specified in path does not exist.
		ft_putstr_fd(new_path, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);//mettre en place TODO l erreur ERRNO le msg approprie errno
		simpleCmd->exit_code = 1;
		return (errno);
	}
	cwd = getcwd(buf, 1096); //si error du return : errno is set
	if (!cwd)
	{
		ft_putstr_fd("cd: error retrieving current directory, getcwd: cannot access parent directory", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		simpleCmd->exit_code = 1;
		return(errno);
	}

	//sur cd - on affiche le OLDPWD en sortie standard
//	simpleCmd->oldpwd = ft_strdup(cwd);
	ft_update_oldpwd_and_pwd_in_lst_envp_for_cd_dash_dash(data_env, cwd, simpleCmd);//faire une copie de PWD avant de la modifier en cwd
	//free(cwd);
	//cwd =NULL;

	return (0);
}


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


int ft_check_path(t_data_env *data_env, char *new_path, t_simpleCmd *simpleCmd)
{
	char	buf[1096];
	char	*cwd;
	(void)data_env;
	(void)simpleCmd;
	cwd = NULL;
	if (chdir(new_path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);//mettre en place TODO l erreur ERRNO le msg approprie errno
		if(errno == 14 || errno == 116)
			errno = ENOENT; // The directory specified in path does not exist.
		ft_putstr_fd(new_path, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);//mettre en place TODO l erreur ERRNO le msg approprie errno
		simpleCmd->exit_code = 1;
		return (errno);

	}
	cwd = getcwd(buf, 1096); //si error du return : errno is set
	if (!cwd)
	{
		ft_putstr_fd("cd: error retrieving current directory, getcwd: cannot access parent directory", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		simpleCmd->exit_code = 1;
		return(errno);
	}

	//sur cd - on affiche le OLDPWD en sortie standard
//	simpleCmd->oldpwd = ft_strdup(cwd);
	ft_update_oldpwd_and_pwd_in_lst_envp_for_cd_dash_dash(data_env, cwd, simpleCmd);//faire une copie de PWD avant de la modifier en cwd
	//free(cwd);
	//cwd =NULL;

	return (0);
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











