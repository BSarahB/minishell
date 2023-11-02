/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_end_expand3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 18:57:14 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/31 18:57:25 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_get_end_expand_rule_5(char *str, char **expand, t_expand *exp, size_t i)
{
	if (str[i] == '$' && str[i - 1] != '$')
	{
		*expand = ft_get_scope_expand(i - 1, \
						exp->start_expand_pos, str, &(exp->flag_expand_here));
		return (1);
	}
	return (0);
}

int	ft_get_end_expand_rule_6(char *str, char **expand, t_expand *exp, size_t i)
{
	if (str[i] == '$' && str[i - 1] != '$')
	{
		*expand = ft_get_scope_expand(i - 1, \
						exp->start_expand_pos, str, &(exp->flag_expand_here));
		return (1);
	}
	return (0);
}

int	ft_get_end_expand_rule_7(char *str, char **expand, t_expand *exp, size_t i)
{
	if (str[i] == '$' && str[i - 1] == '$' && str[i + 1] == '\0')
	{
		*expand = ft_get_scope_expand(i, \
						exp->start_expand_pos, str, &(exp->flag_expand_here));
		return (1);
	}
	return (0);
}
