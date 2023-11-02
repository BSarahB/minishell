/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_data_token_quoting_rule.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 00:32:46 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/30 00:32:57 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	ft_quoting_rule_8(t_list *lst_token, char c, char *str, size_t i)
{
	if (lst_token->quoting_rule == 0 && str[i + 1] == '\0')
	{
		lst_token->end_token_pos = i;
		if (c == '\"')
			lst_token->quoting_rule = 2;
		if (c == '\'')
			lst_token->quoting_rule = 1;
		return (1);
	}
	return (0);
}

int	ft_quoting_rule_9(t_list *lst_token, char *str, size_t i)
{
	if (lst_token->quoting_rule == 2 && str[i + 1] == '\0')
	{
		lst_token->end_token_pos = i;
		return (1);
	}
	return (0);
}

void	ft_get_token_quoting_rule(char *str, t_list *lst_token, size_t i)
{
	char	c;

	c = str[i];
	ft_quoting_rule_0(lst_token);
	if (! ft_quoting_rule_1(lst_token, c, str, i)
		&& ! ft_quoting_rule_2(lst_token, c, str, i)
		&& ! ft_quoting_rule_3(lst_token, c, str, i)
		&& ! ft_quoting_rule_4(lst_token, c, str, i)
		&& ! ft_quoting_rule_5(lst_token, c, str, i)
		&& ! ft_quoting_rule_6(lst_token, c, str, i)
		&& ! ft_quoting_rule_7(lst_token, c, str, i)
		&& ! ft_quoting_rule_8(lst_token, c, str, i))
		ft_quoting_rule_9(lst_token, str, i);
}
