/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 11:53:45 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/09/04 11:53:59 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	ft_get_token_quoting_rule3(t_param_qr2 para, char *buffer_dequote)
{
	char	c;

	(void)buffer_dequote;
	c = para.str[para.i];
	if (ft_skip_quote(&para, c) || ft_skip_quote2(&para, c) || \
ft_skip_quote3(&para, c) || ft_skip_quote4(&para, c) || \
ft_skip_quote5(&para, c) || ft_skip_quote6(&para) || ft_skip_quote7(&para))
		return (*(para.quoting_rule_adequate));
	else
	{
		buffer_dequote[*(para.j)] = c;
		*(para.j) = *(para.j) + 1;
		return (*(para.quoting_rule_adequate));
	}
}

void	ft_set_quoting_rule_adequate_2b(int *quoting_rule_adequate, \
int *quoting_rule)
{
	if (*quoting_rule_adequate == 1)
	{
		*quoting_rule_adequate = 0;
		*quoting_rule = 0;
	}
	else
		*quoting_rule_adequate = 1;
}

int	ft_get_token_quoting_rule2b(char *str, size_t i, int *quoting_rule, \
int *quoting_rule_adequate)
{
	char	c;

	c = str[i];
	if (*quoting_rule == 0 && c == '\"' && str[i + 1] != '\0')
		*quoting_rule = 2;
	else if (c == '\"' && *quoting_rule == 2)
	{
		*quoting_rule = 2;
		ft_set_quoting_rule_adequate_2b(quoting_rule_adequate, quoting_rule);
	}
	else if (*quoting_rule == 0 && c == '\'' && str[i + 1] != '\0')
		*quoting_rule = 1;
	else if (c == '\'' && *quoting_rule == 1)
	{
		*quoting_rule = 1;
		ft_set_quoting_rule_adequate_2b(quoting_rule_adequate, quoting_rule);
	}
	else if (*quoting_rule == 0 && str[i + 1] == '\0')
	{
		if (c == '\"')
			*quoting_rule = 2;
		if (c == '\'')
			*quoting_rule = 1;
	}
	return (*quoting_rule_adequate);
}

void	ft_set_quoting_rule_adequate_2(int *quoting_rule_adequate)
{
	if (*quoting_rule_adequate == 1)
		*quoting_rule_adequate = 0;
	else
		*quoting_rule_adequate = 1;
}

int	ft_get_token_quoting_rule2(char *str, size_t i, int *quoting_rule, \
int *quoting_rule_adequate)
{
	char	c;

	c = str[i];
	if (*quoting_rule == 0 && c == '\"' && str[i + 1] != '\0')
		*quoting_rule = 2;
	else if (c == '\"' && *quoting_rule == 2)
	{
		*quoting_rule = 2;
		ft_set_quoting_rule_adequate_2(quoting_rule_adequate);
	}
	else if (*quoting_rule == 0 && c == '\'' && str[i + 1] != '\0')
		*quoting_rule = 1;
	else if (c == '\'' && *quoting_rule == 1)
	{
		*quoting_rule = 1;
		ft_set_quoting_rule_adequate_2(quoting_rule_adequate);
	}
	else if (*quoting_rule == 0 && str[i + 1] == '\0')
	{
		if (c == '\"')
			*quoting_rule = 2;
		if (c == '\'')
			*quoting_rule = 1;
	}
	return (*quoting_rule_adequate);
}
