/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dequote.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 20:21:06 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/29 20:21:19 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

char *ft_dequote(char *str)
{
	char 	*buffer_dequote;
	int 	i;
	int		j;
	int 	quoting_rule_adequate;
	int 	quoting_rule;

	buffer_dequote = ft_init_string(1096);
	i = 0;
	j = 0;
	quoting_rule_adequate = 0;
	quoting_rule = 0;
	while(str[i])
	{
		ft_get_token_quoting_rule3(str, i, &quoting_rule, &quoting_rule_adequate, buffer_dequote, &j);
		i++;
	}
	buffer_dequote = ft_update_string(&str, buffer_dequote);
	return(buffer_dequote);
}