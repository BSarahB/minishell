/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_data_token_quoting_rule2.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 18:47:34 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/31 18:47:45 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_quoting_rule_0(t_list *lst_token)
{
	if ((lst_token->quoting_rule == 1 || lst_token->quoting_rule == 2) \
		&& lst_token->quoting_rule_adequate == 1)
	{
		lst_token->quoting_rule = 0;
		lst_token->quoting_rule_adequate = 0;
		return (1);
	}
	return (0);
}

int	ft_quoting_rule_1(t_list *lst_token, char c, char *str, size_t i)
{
	if (lst_token->quoting_rule == 0 && c == '\"' && str[i + 1] != '\0')
	{
		lst_token->quoting_rule = 2;
		return (1);
	}
	return (0);
}

int	ft_quoting_rule_2(t_list *lst_token, char c, char *str, size_t i)
{
	if (c == '\"' && lst_token->quoting_rule == 2 \
		&& lst_token->quoting_rule_adequate == 0)
	{
		lst_token->quoting_rule = 2;
		lst_token->quoting_rule_adequate = 1;
		if (str[i + 1] == '\0' \
			|| (str[i + 1] == ' ' || (str[i + 1] >= 9 && str[i + 1] <= 13)) \
			|| (ft_is_char_operand(&str[i + 1], lst_token) >= 3))
			lst_token->end_token_pos = i;
		return (1);
	}
	return (0);
}

int	ft_quoting_rule_3(t_list *lst_token, char c, char *str, size_t i)
{
	if (lst_token->quoting_rule == 2 && c == '\"' && str[i + 1] != '\0')
	{
		lst_token->quoting_rule = 2;
		if (lst_token->quoting_rule_adequate == 1)
			lst_token->quoting_rule_adequate = 0;
		return (1);
	}
	return (0);
}

int	ft_quoting_rule_4(t_list *lst_token, char c, char *str, size_t i)
{
	if (lst_token->quoting_rule == 2 && c == '\"' && str[i + 1] != '\0')
	{
		lst_token->quoting_rule = 2;
		if (lst_token->quoting_rule_adequate == 1)
			lst_token->quoting_rule_adequate = 0;
		return (1);
	}
	return (0);
}
