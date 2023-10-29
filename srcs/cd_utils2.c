/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 17:50:52 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/29 17:51:04 by mbenmesb         ###   ########.fr       */
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
