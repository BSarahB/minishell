/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_end_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 22:59:01 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/29 22:59:14 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	ft_get_end_expand_rule_8(char *str, char **expand, t_expand *exp, size_t i)
{
	if ((str[i] == '?' || ft_isdigit(str[i]) == 1) \
									&& str[i + 1] != '\0' && str[i + 1] != '$')
	{
		*expand = ft_get_scope_expand(i, \
						exp->start_expand_pos, str, &(exp->flag_expand_here));
		exp->flag_expand_here = 1;
		exp->flag_dollar_quest = 1;
		return (1);
	}
	return (0);
}

int	ft_get_end_expand_rule_9(char *str, char **expand, t_expand *exp, size_t i)
{
	if ((ft_is_alphanum(str[i]) == 0) \
		&& (str[i] != '?' && str[i + 1] != '\0'))
	{
		if (ft_isunderscore(str, i) > 0)
		{
			i = ft_isunderscore(str, i);
			if (str[i + 1] == '\0')
			{
				*expand = ft_get_scope_expand(i, \
						exp->start_expand_pos, str, &(exp->flag_expand_here));
				exp->flag_expand_here = 1;
			}
		}
		else if (!(str[i] == '$' && str[i -1] == '$'))
			*expand = ft_get_scope_expand(i - 1, \
						exp->start_expand_pos, str, &(exp->flag_expand_here));
		else
			*expand = ft_get_scope_expand(i - 1, \
						exp->start_expand_pos, str, &(exp->flag_expand_here));
		return (1);
	}
	return (0);
}

int	ft_get_end_expand_rule_10(char *str, char **expand, t_expand *exp, size_t i)
{
	if ((ft_is_alphanum(str[i]) == 0) \
				&& (str[i] != '?' && str[i + 1] == '\0'))
	{
		if (!(str[i] == '$' && str[i -1] == '$'))
			*expand = ft_get_scope_expand(i - 1, \
						exp->start_expand_pos, str, &(exp->flag_expand_here));
		else
			*expand = ft_get_scope_expand(i - 1, \
						exp->start_expand_pos, str, &(exp->flag_expand_here));
		return (1);
	}
	return (0);
}

int	ft_get_end_expand_rule_11(char *str, char **expand, t_expand *exp, size_t i)
{
	if (str[i + 1] == '\0')
	{
		*expand = ft_get_scope_expand(i, \
						exp->start_expand_pos, str, &(exp->flag_expand_here));
		exp->flag_expand_here = 1;
		return (1);
	}
	return (0);
}

size_t	ft_get_end_expand(char *str, t_expand *exp, char **expand, size_t i)
{
	ft_get_end_expand_rule_0(str, expand, exp, i);
	if (exp->quoting_rule == 0 || exp->quoting_rule == 2)
	{
		if (!ft_get_end_expand_rule_1(str, expand, exp, i)
			&& !ft_get_end_expand_rule_2(str, expand, exp, i)
			&& !ft_get_end_expand_rule_3(str, expand, exp, i)
			&& !ft_get_end_expand_rule_4(str, expand, exp, i)
			&& !ft_get_end_expand_rule_5(str, expand, exp, i)
			&& !ft_get_end_expand_rule_6(str, expand, exp, i)
			&& !ft_get_end_expand_rule_7(str, expand, exp, i)
			&& !ft_get_end_expand_rule_8(str, expand, exp, i)
			&& !ft_get_end_expand_rule_9(str, expand, exp, i)
			&& !ft_get_end_expand_rule_10(str, expand, exp, i))
			ft_get_end_expand_rule_11(str, expand, exp, i);
	}
	return (i);
}
