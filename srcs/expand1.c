/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 18:32:55 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/31 18:33:09 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_skip_quote5(t_param_qr2 *para, char c)
{
	if (*(para->quoting_rule_adequate) == 1 && *(para->quoting_rule) == 0 && \
c == '\'' && para->str[para->i + 1] != '\0')
	{
		*(para->quoting_rule) = 1;
		*(para->quoting_rule_adequate) = 0;
		return (1);
	}
	return (0);
}

int	ft_skip_quote4(t_param_qr2 *para, char c)
{
	if (*(para->quoting_rule_adequate) == 1 && *(para->quoting_rule) == 0 && \
c == '\"' && para->str[para->i + 1] != '\0')
	{
		*(para->quoting_rule) = 2;
		*(para->quoting_rule_adequate) = 0;
		return (1);
	}
	return (0);
}

int	ft_skip_quote3(t_param_qr2 *para, char c)
{
	if (*(para->quoting_rule_adequate) == 0 && *(para->quoting_rule) == 1 && \
c == '\'' && para->str[para->i + 1] != '\0')
	{
		*(para->quoting_rule) = 0;
		*(para->quoting_rule_adequate) = 1;
		return (1);
	}
	return (0);
}

int	ft_skip_quote2(t_param_qr2 *para, char c)
{
	if (*(para->quoting_rule_adequate) == 0 && *(para->quoting_rule) == 2 && \
c == '\"' && para->str[para->i + 1] != '\0')
	{
		*(para->quoting_rule) = 0;
		*(para->quoting_rule_adequate) = 1;
		return (1);
	}
	return (0);
}

int	ft_skip_quote(t_param_qr2 *para, char c)
{
	if (*(para->quoting_rule_adequate) == 0 && *(para->quoting_rule) == 0 && \
c == '\'' && para->str[para->i + 1] != '\0')
	{
		*(para->quoting_rule) = 1;
		return (1);
	}
	if (*(para->quoting_rule_adequate) == 0 && *(para->quoting_rule) == 0 && \
c == '\"' && para->str[para->i + 1] != '\0')
	{
		*(para->quoting_rule) = 2;
		return (1);
	}
	return (0);
}
