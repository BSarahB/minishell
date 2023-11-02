/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_check_var_in.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 00:19:00 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/30 00:19:11 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	ft_update_keys(char *str, t_listenvp *tmp, char **key, \
char **key_input)
{
	int	n;

	n = 0;
	n = ft_count_n_for_key(tmp->key_value);
	*key = ft_strndup(tmp->key_value, n);
	n = ft_count_n_for_key(str);
	*key_input = ft_strndup(str, n);
}

int	ft_is_var_already_in_lst_envp(t_listenvp *lst_envp, char *str)
{
	t_listenvp	*tmp;
	char		*key;
	char		*key_input;

	tmp = lst_envp;
	key = NULL;
	key_input = NULL;
	while (tmp)
	{
		ft_update_keys(str, tmp, &key, &key_input);
		if (ft_strcmp(key, key_input) == 0)
		{
			ft_update_string(&tmp->key_value, ft_strdup(str));
			ft_free_keys(&key_input, &key);
			return (1);
		}
		else
		{
			ft_free_keys(&key_input, &key);
			tmp = tmp->next;
		}
	}
	ft_free_keys(&key_input, &key);
	return (0);
}

int	ft_is_var_already_in_lst_envp_d(t_listenvp *lst_envp_d, char *str)
{
	t_listenvp	*tmp;
	int			n;
	char		*key;

	tmp = lst_envp_d;
	n = 0;
	key = NULL;
	while (tmp)
	{
		n = ft_count_n_for_key(tmp->key_value);
		key = ft_strndup(tmp->key_value, n);
		if (ft_strcmp(key, str) == 0)
		{
			ft_free_struct_str(&key);
			return (1);
		}
		else
		{
			ft_free_struct_str(&key);
			tmp = tmp->next;
		}
	}
	ft_free_struct_str(&key);
	return (0);
}

void	ft_add_var_to_lst_envp_d(t_list *tmp, t_listenvp *lst_envp_d)
{
	t_listenvp	*new;

	new = NULL;
	if (ft_is_var_already_in_lst_envp_d(lst_envp_d, tmp->content) == 0)
	{
		new = ft_lstnew_for_lst_envp(tmp->content);
		ft_lstadd_back_envp(&lst_envp_d, new);
	}
}

void	ft_add_var_to_lst_envp(t_list *tmp, t_listenvp *lst_envp)
{
	t_listenvp	*new;

	new = NULL;
	if (ft_is_var_already_in_lst_envp(lst_envp, tmp->content) == 0)
	{
		new = ft_lstnew_for_lst_envp(tmp->content);
		ft_lstadd_back_envp(&lst_envp, new);
	}
}
