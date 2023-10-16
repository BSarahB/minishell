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

int ft_check_format_is_key_value(t_list *tmp)
{
	(void)tmp;
	char *str;
	int i;

	i = 0;
	str = tmp->content;
	while(str[i])
	{
		if(str[i] == '=') //->return 0 dans le cas d un key=value
			return(1);
		i++;
	}
	return (0);	//->dans le cas d un key
}

int ft_isalnum(int c)
{
	return(ft_isdigit(c) || ft_isalpha(c));
}

int	ft_key_value_is_valid(t_list *tmp)
{
	(void)tmp;
	char *str;
	int i;

	i = 1;
	str = tmp->content;
	if(str[0] == '\0')
	{
		ft_putstr_fd("bash: export: `': not a valid identifier\n", 2); //le echo $? est 1 car il y a une erreur
		return(0);//not valide
	}
	if(str[0])
	{
		if (ft_isalpha(str[0]) == 0 && str[0] != '_')
			{
				ft_putstr_fd("bash: export: `", 2);
				ft_putstr_fd(tmp->content, 2);
				ft_putstr_fd("': not a valid identifier\n", 2); //le echo $? est 1 car il y a une erreur
				return(0);
			}
	}

	while (str[i])
	{
		if(ft_isalnum(str[i]) == 0 && str[i] != '_' && str[i] != '=')
		{
			ft_putstr_fd("bash: export: `", 2);
			ft_putstr_fd(tmp->content, 2);
			ft_putstr_fd("': not a valid identifier\n", 2); //le echo $? est 1 ca
			return (0);
		}
		if(str[i] == '=')
			return(1);
		i++;
	}
	return(1); //-> valide
}

int ft_is_var_already_in_lst_envp(t_listenvp *lst_envp, char *str)
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
	return(0);
}

int ft_is_var_already_in_lst_envp_d(t_listenvp *lst_envp_d, char *str)
{
	t_listenvp *tmp;
	int n;
	char *key;

	tmp = lst_envp_d;
	n = 0;
	key = NULL;
	while(tmp)
	{
		n = ft_count_n_for_key(tmp->key_value);
		key = ft_strndup(tmp->key_value, n);
		if(ft_strcmp(key, str) == 0)
		{
			free(key);
			key = NULL;
			return(1);
		}
		else
		{
			free(key);
			key = NULL;
			tmp = tmp->next;
		}
	}
	if(key != NULL)
	{
		free(key);
		key = NULL;
	}
	return(0);
}



void ft_add_var_to_lst_envp_d(t_list *tmp, t_listenvp *lst_envp_d)
{
	t_listenvp *new;

	new = NULL;
	if(ft_is_var_already_in_lst_envp_d(lst_envp_d, tmp->content) == 0)
	{
		new = ft_lstnew_for_lst_envp(tmp->content);
		ft_lstadd_back_envp(&lst_envp_d, new);
	}
//		ft_aff_list_envp_d_sur_char_content(lst_envp_d); // pour void		*content; de type char *
}

void ft_add_var_to_lst_envp(t_list *tmp, t_listenvp *lst_envp)
{
	t_listenvp *new;

	new = NULL;
	if(ft_is_var_already_in_lst_envp(lst_envp, tmp->content) == 0)
	{
		new = ft_lstnew_for_lst_envp(tmp->content);
		ft_lstadd_back_envp(&lst_envp, new);
	}
	
}

void ft_check_export(t_cmd *cmd, t_list *start_lst_token_retokenized, t_simpleCmd *simpleCmd, t_data_env *data_env)
{
	(void)cmd;
	(void)start_lst_token_retokenized;
	(void)simpleCmd;
	(void)data_env;
	t_list *tmp;
	int flag_export;

	flag_export = -1;
	tmp = start_lst_token_retokenized;
	if(tmp != NULL)
	{
		if(flag_export == -1)
		{
			if(flag_export == -1 && (ft_strcmp("export", tmp->content) == 0))
			{
				flag_export = 1;
				simpleCmd->is_builtin = 1;
				simpleCmd->builtin = export; // == 3
				if(tmp->next == NULL)
					simpleCmd->export_no_option = 1;
				else
					tmp = tmp->next;
				if(cmd->nb_of_simpleCmds == 1)
					{
						simpleCmd->export_solo = 1;
						simpleCmd->builtin_solo = 1;
					}
			}
			else
				flag_export = 0;
		}
	}

	while (tmp != NULL && tmp->position < simpleCmd->end_simpleCmd_pos && simpleCmd->export_no_option == 0 && flag_export == 1)
	{
		if(simpleCmd->nofile == 1)
			break;
		if(simpleCmd->export_solo == 1)
		{
			if (ft_key_value_is_valid(tmp) == 1)
			{
				if(ft_check_format_is_key_value(tmp) == 1)
				{
					ft_add_var_to_lst_envp(tmp, data_env->lst_envp);
					ft_add_var_to_lst_envp(tmp, data_env->lst_envp_d);

				}
				if(ft_check_format_is_key_value(tmp) == 0)
				{
					ft_add_var_to_lst_envp_d(tmp, data_env->lst_envp_d);
				}

			}

			//1) KEY seule pas de =VALUE et valid identifier:
			//remplir lst_envp_d en ajoutant le maillon (pas dans lst_envp)
			//si key=VALUE ->on met key=value dans lst_envp && lst_envp_d
		}
		tmp = tmp->next;
	}
	/*
	if(tmp != NULL && tmp->position == simpleCmd->end_simpleCmd_pos && tmp->title != operator && simpleCmd->export_x == 0 && flag_export == 1)
	{
		//if
		//{
		  // if()
		//}

	}
*/
}
