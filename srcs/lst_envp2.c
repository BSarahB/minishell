/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_envp2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 01:35:16 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/30 01:35:29 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_listenvp	*ft_lstlast_envp(t_listenvp *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	ft_free_struct_t_data_env(t_data_env **data_env)
{
	if ((*data_env)->lst_envp != NULL)
		ft_free_struct_t_list_lst_envp(&((*data_env)->lst_envp));
	if ((*data_env)->lst_envp_d != NULL)
		ft_free_struct_t_list_lst_envp(&((*data_env)->lst_envp_d));
	if (*data_env != NULL)
	{
		free(*data_env);
		*data_env = NULL;
	}
}

t_data_env	*ft_struct_init_data_env(t_data_env **data_env)
{
	*data_env = (t_data_env *)malloc(sizeof (t_data_env));
	if (!(*data_env))
		return (NULL);
	(*data_env)->lst_envp = NULL;
	(*data_env)->lst_envp_d = NULL;
	(*data_env)->flag_oldpwd_istoremove = 0;
	(*data_env)->exit_parent = 0;
	return (*data_env);
}

size_t	ft_count_keys_in_lst_envp(t_listenvp *lst_envp)
{
	size_t		i;
	t_listenvp	*tmp;

	i = 0;
	tmp = lst_envp;
	if (tmp == NULL)
		return (i);
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

char	**ft_lst_to_tab(t_listenvp *lst_envp)
{
	char		**tab_envp;
	t_listenvp	*tmp;
	size_t		i;
	size_t		k;

	tab_envp = NULL;
	tmp = lst_envp;
	k = 0;
	i = ft_count_keys_in_lst_envp(lst_envp);
	tab_envp = malloc(sizeof (*tab_envp) *(i + 1));
	if (!tab_envp)
		return (NULL);
	while (k < i)
	{
		tab_envp[k] = ft_strdup(tmp->key_value);
		tmp = tmp->next;
		k++;
	}
	tab_envp[k] = NULL;
	return (tab_envp);
}
