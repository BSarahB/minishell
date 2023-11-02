/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_init_execution.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 12:29:48 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/08/08 12:29:55 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_settings	*ft_struct_init_settings(t_settings **set)
{
	*set = (t_settings *)malloc(sizeof (t_settings));
	if (!(*set))
		return (0);
	(*set)->fdin = 0;
	(*set)->fdout = 0;
	(*set)->i = 0;
	(*set)->j = 0;
	(*set)->exit_status = 0;
	(*set)->savein = 0;
	(*set)->saveout = 0;
	(*set)->nofile = 0;
	(*set)->ret = -2;
	(*set)->pip_exists = 0;
	(*set)->pip[0] = 0;
	(*set)->pip[1] = 0;
	return (*set);
}

t_settings_del	*ft_struct_init_settings_del(t_settings_del **del)
{
	*del = (t_settings_del *)malloc(sizeof (t_settings_del));
	if (!(*del))
		return (0);
	(*del)->i = 0;
	(*del)->j = 0;
	(*del)->k = 0;
	(*del)->index = 0;
	return (*del);
}
