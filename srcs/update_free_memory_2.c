/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_free_memory_2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 15:08:53 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/08/09 15:09:09 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    ft_free_struct_t_expand(t_expand **exp)
{
    if(*exp != NULL)
    {
        free(*exp);
        *exp = NULL;
    }
}

void    ft_free_struct_t_settings_del(t_settings_del **del)
{
	if(*del != NULL)
	{
		free(*del);
		*del = NULL;
	}
}

void    ft_free_struct_t_settings(t_settings **set)
{
    if(*set != NULL)
    {
        free(*set);
        *set = NULL;
    }
}

void    ft_free_struct_t_list_token(t_list **token)
{
  // ft_free_struct_str(&(*token)->content); //content est a NULL si pas de token tokenize, mais siinon il est malloc a ft_initcstring dans le ft_get_token
    if (*token != NULL)
    {
        free(*token);
        *token = NULL;
    }
}

void    ft_free_struct_t_data(t_data **data)
{
    ft_free_struct_t_list_token(&(*data)->token);
    if (*data != NULL)
    {
        free(*data);
        *data = NULL;
    }
}
