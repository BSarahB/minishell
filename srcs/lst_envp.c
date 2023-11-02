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

void	ft_lstadd_back_envp(t_listenvp **alst, t_listenvp *new)
{
	t_listenvp	*tmp;

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

void	ft_struct_init_param_dbquotes(t_param_dbquotes *para, char *str)
{
	para->buffer_quoted = ft_init_string((size_t)ft_strlen(str) + 2);
	para->flag_equal = 0;
	para->i = 0;
	para->j = 0;
}

char	*ft_add_double_quote_to_var(char *str)
{
	t_param_dbquotes	para;

	ft_struct_init_param_dbquotes(&para, str);
	while (str[para.i])
	{
		para.buffer_quoted[para.j] = str[para.i];
		if (str[para.i] == '=' && para.flag_equal == 0)
		{
			para.j = para.j +1;
			para.buffer_quoted[para.j] = '\"';
			para.flag_equal = 1;
		}
		para.j++;
		para.i++;
	}
	if (para.flag_equal == 1)
	{
		para.buffer_quoted[para.j] = '\"';
		para.j = para.j + 1;
	}
	para.buffer_quoted[para.j] = '\0';
	return (para.buffer_quoted);
}

t_listenvp	*ft_lstnew_for_lst_envp(char *str)
{
	t_listenvp	*new;

	new = malloc(sizeof (t_listenvp));
	if (!new)
		return (NULL);
	new->key_value = ft_strdup(str);
	new->position = 0;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

t_listenvp	*ft_add_double_quote_to_envp_d(t_listenvp *lst_envp_d)
{
	char		*key_value_quoted;
	t_listenvp	*tmp;

	tmp = lst_envp_d;
	while (tmp)
	{
		key_value_quoted = ft_add_double_quote_to_var(tmp->key_value);
		tmp->key_value = ft_update_string(&tmp->key_value, key_value_quoted);
		tmp = tmp->next;
	}
	return (lst_envp_d);
}
