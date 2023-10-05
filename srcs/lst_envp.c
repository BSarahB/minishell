/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_envp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 11:25:04 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/04 11:25:18 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void    ft_free_struct_t_data_env(t_data_env **data_env)
{
    if((*data_env)->lst_envp != NULL)
			ft_free_struct_t_list_lst_envp(&((*data_env)->lst_envp));
	if((*data_env)->lst_envp_d != NULL)
			ft_free_struct_t_list_lst_envp(&((*data_env)->lst_envp_d));
    if (*data_env != NULL)
    {
        free(*data_env);
        *data_env = NULL;
    }
}

t_data_env	*ft_struct_init_data_env(t_data_env **data_env)
{
	
	*data_env = (t_data_env *)malloc(sizeof(t_data_env));
	if (!(*data_env))
		return (NULL);
	(*data_env)->lst_envp = NULL;
	(*data_env)->lst_envp_d = NULL;
	return(*data_env);
}



size_t ft_count_keys_in_lst_envp(t_listenvp *lst_envp)
{
	size_t i;
	t_listenvp *tmp;

	i = 0;
	tmp = lst_envp;
	if(tmp == NULL)
		return(i);
	while(tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return(i);
}

char **ft_lst_to_tab(t_listenvp *lst_envp)
{
	char **tab_envp;
	t_listenvp *tmp;
	size_t i;
	size_t k;

	tab_envp = NULL;
	tmp = lst_envp;
	k = 0;
	i = ft_count_keys_in_lst_envp(lst_envp);
	tab_envp = malloc(sizeof(*tab_envp) *(i + 1));
	
	if(!tab_envp)
		return(NULL);
	while(k < i)
	{
		tab_envp[k] = ft_strdup(tmp->key_value);
		tmp = tmp->next;
		k++;
	}
	tab_envp[k] = NULL;
	return(tab_envp);
}


t_listenvp *ft_lstlast_envp(t_listenvp *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void ft_lstadd_back_envp(t_listenvp **alst, t_listenvp *new)
{
	t_listenvp *tmp;

	if (new)
		{
			new->next = NULL;
			new->prev = NULL;
		}
	if (!(*alst))
		*alst = new;
	else
		{
			tmp = ft_lstlast_envp(*alst);
			ft_lstlast_envp(*alst)->next = new;
			new->prev = tmp;
		}
}

/*

t_listenvp *ft_lstnew_for_lst_retokenized2_envp(t_list *token)
{
	t_listenvp *new;

	new = malloc(sizeof(t_listenvp));
	if (!new)
		return (NULL);
	new->key_value = ft_strdup(token->content);
//	new->title = token->title;
//	new->position = token->position;

	new->next = NULL;
	new->prev = NULL;
	return (new);
}

*/

t_listenvp *ft_lstnew_for_lst_envp(char *str)
{
	t_listenvp *new;

	new = malloc(sizeof(t_listenvp));
	if (!new)
		return (NULL);
	new->key_value = ft_strdup(str);
	//new->title = tmp->title;
	new->position = 0;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}
