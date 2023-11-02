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

void	ft_clean_all(t_param_utils para, char *str)
{
	if (para.key != NULL)
		ft_free_struct_str(&(para.key));
	if (para.key_input != NULL)
		ft_free_struct_str(&(para.key_input));
	if (str != NULL)
		ft_free_struct_str(&str);
}

void	ft_clean(char *str, t_param_utils para)
{
	ft_free_struct_str(&str);
	ft_free_struct_str(&(para.key));
	ft_free_struct_str(&(para.key_input));
}

void	ft_init_t_param2(t_param_utils *para)
{
	para->tmp = NULL;
	para->n = 0;
	para->new_key_val = NULL;
	para->key = NULL;
	para->key_input = NULL;
}

int	ft_is_var_already_in_lst_envp_for_cd(t_listenvp *lst_envp, char *str)
{
	t_param_utils	para;

	ft_init_t_param2(&para);
	para.tmp = lst_envp;
	while (para.tmp)
	{
		para.n = ft_count_n_for_key(para.tmp->key_value);
		para.key = ft_strndup(para.tmp->key_value, para.n);
		para.n = ft_count_n_for_key(str);
		para.key_input = ft_strndup(str, para.n);
		if (ft_strcmp(para.key, para.key_input) == 0)
		{
			para.new_key_val = ft_strdup(str);
			ft_update_string(&(para.tmp->key_value), para.new_key_val);
			return (1);
		}
		else
		{
			ft_free_struct_str(&(para.key));
			ft_free_struct_str(&(para.key_input));
			para.tmp = para.tmp->next;
		}
	}
	ft_clean_all(para, str);
	return (0);
}
