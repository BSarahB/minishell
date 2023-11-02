/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_end_expand2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 18:56:57 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/31 18:57:07 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_get_end_expand_rule_0(char *str, char **expand, \
							t_expand *exp, size_t i)
{
	if (exp->quoting_rule == 1)
	{
		if (str[i] == '\"' || str[i] == '\'')
			*expand = ft_get_scope_expand(i - 1, \
						exp->start_expand_pos, str, &(exp->flag_expand_here));
		else if (str[i - 1] != '$' \
					&& (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13)))
			*expand = ft_get_scope_expand(i - 1, \
						exp->start_expand_pos, str, &(exp->flag_expand_here));
	}
}

int	ft_get_end_expand_rule_1(char *str, char **expand, t_expand *exp, size_t i)
{
	if (exp->quoting_rule == 2 && str[i - 1] != '$' \
		&& (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13)))
	{
		*expand = ft_get_scope_expand(i - 1, \
						exp->start_expand_pos, str, &(exp->flag_expand_here));
		return (1);
	}
	return (0);
}

int	ft_get_end_expand_rule_2(char *str, char **expand, t_expand *exp, size_t i)
{
	if (exp->quoting_rule == 2 && str[i - 1] == '$' \
		&& (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13)))
	{
		*expand = ft_get_scope_expand(i - 1, \
						exp->start_expand_pos, str, &(exp->flag_expand_here));
		return (1);
	}
	return (0);
}

int	ft_get_end_expand_rule_3(char *str, char **expand, t_expand *exp, size_t i)
{
	if (str[i - 1] == '$' && ft_isunderscore(str, i) > 0)
	{
		i = ft_isunderscore(str, i);
		if (str[i + 1] == '\0')
		{
			*expand = ft_get_scope_expand(i, \
						exp->start_expand_pos, str, &(exp->flag_expand_here));
			exp->flag_expand_here = 1;
		}
		return (1);
	}
	return (0);
}

int	ft_get_end_expand_rule_4(char *str, char **expand, t_expand *exp, size_t i)
{
	if (str[i] == '\"' || str[i] == '\'')
	{
		*expand = ft_get_scope_expand(i - 1, \
						exp->start_expand_pos, str, &(exp->flag_expand_here));
		return (1);
	}
	return (0);
}
