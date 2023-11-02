/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 18:33:14 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/31 18:33:40 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_skip_quote7(t_param_qr2 *para)
{
	if (para->str[para->i + 1] == '\0' && *(para->quoting_rule_adequate) == 0 \
	&& *(para->quoting_rule) == 1 && para->str[para->i] == '\'')
	{
		*(para->quoting_rule_adequate) = 1;
		*(para->quoting_rule) = 0;
		return (1);
	}
	return (0);
}

int	ft_skip_quote6(t_param_qr2 *para)
{
	if (para->str[para->i + 1] == '\0' && *(para->quoting_rule_adequate) == 0 \
	&& *(para->quoting_rule) == 2 && para->str[para->i] == '\"')
	{
		*(para->quoting_rule_adequate) = 1;
		*(para->quoting_rule) = 0;
		return (1);
	}
	return (0);
}
