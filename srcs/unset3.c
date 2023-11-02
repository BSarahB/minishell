/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 23:05:52 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/09 23:06:03 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_is_var_to_unset_already_in_lst_envp(t_data_env *data_env, char *str)
{
	t_listenvp	*tmp;
	char		*key;

	tmp = data_env->lst_envp;
	key = NULL;
	while (tmp)
	{
		key = ft_strndup(tmp->key_value, ft_count_n_for_key(tmp->key_value));
		if (ft_strcmp(key, str) == 0)
		{
			data_env->lst_envp = delnod(data_env->lst_envp, \
															tmp->key_value);
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

void	ft_remove_var_from_lst_envp_d(t_list *tmp, t_data_env *data_env)
{
	ft_is_var_to_unset_already_in_lst_envp_d(data_env, tmp->content);
}
