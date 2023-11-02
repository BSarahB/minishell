/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 23:05:52 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/09 23:06:03 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_listenvp	*ft_delete_node_first(t_listenvp *alst, t_listenvp *to_remove)
{
	to_remove = alst;
	alst = (alst)->next;
	free(to_remove->key_value);
	to_remove->key_value = NULL;
	free(to_remove);
	to_remove = NULL;
	return (alst);
}

t_listenvp	*delnod(t_listenvp *alst, char *str)
{
	t_listenvp	*curr;
	t_listenvp	*to_remove;

	to_remove = NULL;
	curr = alst;
	if (alst == NULL)
		return (NULL);
	if ((alst)->key_value == str)
		return (ft_delete_node_first(alst, to_remove));
	while (curr->next != NULL)
	{
		if (curr->next->key_value == str)
		{
			to_remove = curr->next;
			curr->next = curr->next->next;
			free(to_remove->key_value);
			to_remove->key_value = NULL;
			free(to_remove);
			to_remove = NULL;
			return (alst);
		}
		curr = curr->next;
	}
	return (alst);
}

int	ft_count_n_for_key(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (i);
		i++;
	}
	return (i);
}

void	ft_delete_key(char *key)
{
	if (key != NULL)
		ft_free_struct_str(&key);
}

int	ft_is_var_to_unset_already_in_lst_envp_d(t_data_env *data_env, char *str)
{
	t_listenvp	*tmp;
	int			n;
	char		*key;

	tmp = data_env->lst_envp_d;
	n = 0;
	key = NULL;
	while (tmp)
	{
		n = ft_count_n_for_key(tmp->key_value);
		key = ft_strndup(tmp->key_value, n);
		if (ft_strcmp(key, str) == 0)
		{
			data_env->lst_envp_d = delnod(data_env->lst_envp_d, tmp->key_value);
			ft_free_struct_str(&key);
			return (1);
		}
		else
		{
			ft_free_struct_str(&key);
			tmp = tmp->next;
		}
	}
	ft_delete_key(key);
	return (0);
}
