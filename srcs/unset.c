/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 23:05:52 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/09 23:06:03 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_listenvp *ft_delete_node(t_listenvp *alst, char *str)
{


	t_listenvp *curr;
	t_listenvp *to_remove;

	curr = alst;


	if(alst == NULL)
		return(NULL);

	if((alst)->key_value == str)
	{
		to_remove = alst;
		alst = (alst)->next;
		free(to_remove->key_value);
		to_remove->key_value = NULL;
		free(to_remove);
		to_remove = NULL;
		return(alst);
	}
	while(curr->next != NULL)
	{
		if(curr->next->key_value == str)
		{
			to_remove = curr->next;
			curr->next= curr->next->next;
			free(to_remove->key_value);
			to_remove->key_value = NULL;
			free(to_remove);
			to_remove = NULL;
			return(alst);
		}
		curr=  curr->next;
	}
	return(alst);
}

int	ft_count_n_for_key(char *str)
{
	int i;

	i = 0;
	while(str[i])
	{
		if(str[i] == '=')
			return(i);
		i++;
	}
	return(i);
}


int ft_is_var_to_unset_already_in_lst_envp_d(t_data_env *data_env, char *str)
{
	t_listenvp *tmp;
	int n;
	char *key;

	tmp = data_env->lst_envp_d;
	n = 0;
	key = NULL;
	while(tmp)
	{
		n = ft_count_n_for_key(tmp->key_value);
		key = ft_strndup(tmp->key_value, n);
		if(ft_strcmp(key, str) == 0)
		{

			printf("unset  = %s to remove in lst_envp_d\n", tmp->key_value);

			data_env->lst_envp_d = ft_delete_node(data_env->lst_envp_d, tmp->key_value);
			//on va delete le maillon de lst_envpft_delone(&lst_envp);
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

int ft_is_var_to_unset_already_in_lst_envp(t_data_env *data_env, char *str)
{
	t_listenvp *tmp;
	int n;
	char *key;

	tmp = data_env->lst_envp;
	n = 0;
	key = NULL;
	while(tmp)
	{
		n = ft_count_n_for_key(tmp->key_value);
		key = ft_strndup(tmp->key_value, n);
		if(ft_strcmp(key, str) == 0)
		{
			printf("unset  = %s to remove in lst_envp\n", tmp->key_value);
			data_env->lst_envp = ft_delete_node(data_env->lst_envp, tmp->key_value);
			//on va elete le maillon de lst_envpft_delone(&lst_envp);
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


void ft_remove_var_from_lst_envp_d(t_list *tmp, t_data_env *data_env)
{

	ft_is_var_to_unset_already_in_lst_envp_d(data_env, tmp->content);
//	ft_aff_list_envp_d_sur_char_content(lst_envp_d); // pour void		*content; de type char *
}

void ft_remove_var_from_lst_envp(t_list *tmp, t_data_env *data_env)
{

	ft_is_var_to_unset_already_in_lst_envp(data_env, tmp->content);
//	ft_aff_list_envp_sur_char_content(lst_envp); // pour void		*content; de type char *

}

int	ft_unset_value_is_valid(t_list *tmp)
{
	(void)tmp;
	char *str;
	int i;

	i = 0;
	str = tmp->content;
	if(str[0] == '\0')
	{
		return(0);//not valide
	}
	while (str[i])
	{
		if(str[i] == '=')
			return(0);
		i++;
	}

	return(1); //-> valide
}

void ft_check_unset(t_cmd *cmd, t_list *start_lst_token_retokenized, t_simpleCmd *simpleCmd, t_data_env *data_env)
{
	(void)cmd;
	(void)start_lst_token_retokenized;
	(void)simpleCmd;
	(void)data_env;
	t_list *tmp;
	int flag_unset;

	flag_unset = -1;
	tmp = start_lst_token_retokenized;
	if(tmp != NULL)
	{
		if(flag_unset == -1)
		{
			if(flag_unset == -1 && (ft_strcmp("unset", tmp->content) == 0))
			{
				flag_unset = 1;
				simpleCmd->is_builtin = 1;
				simpleCmd->builtin = unset; // == 3
				if(tmp->next == NULL)
					simpleCmd->unset_no_option = 1;
				else
					{
						tmp = tmp->next;
						if(ft_strcmp(tmp->content, "?") == 0)
							tmp = tmp->next;
					}
				if(cmd->nb_of_simpleCmds == 1)
					{
						simpleCmd->unset_solo = 1;
						simpleCmd->builtin_solo = 1;
					}
			}
			else
				flag_unset = 0;
		}
	}

	while (tmp != NULL && tmp->position < simpleCmd->end_simpleCmd_pos && simpleCmd->unset_no_option == 0 && flag_unset == 1)
	{
		if(simpleCmd->nofile == 1)
			break;
		if(simpleCmd->unset_solo == 1)
		{
			
				
			if(ft_strcmp(tmp->content, "PWD") == 0)
				data_env->flag_oldpwd_istoremove = 1;
			if (ft_unset_value_is_valid(tmp) == 1)
			{

					ft_remove_var_from_lst_envp(tmp, data_env);
					ft_remove_var_from_lst_envp_d(tmp, data_env);

			}
			else
				ft_remove_var_from_lst_envp_d(tmp, data_env);
		}
		tmp = tmp->next;
	}
}
