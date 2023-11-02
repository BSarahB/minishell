/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 23:01:29 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/29 23:01:41 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	ft_strcmp_char(char c1, char c2)
{
	return (c1 - c2);
}

int	ft_is_expand_unvalidated(char **invalidators, char c)
{
	int	i;

	i = 0;
	while (invalidators[i] != NULL)
	{
		if (ft_strcmp_char(invalidators[i][0], c) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	ft_isdigit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	ft_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}

int	ft_is_alphanum(char c)
{
	if (ft_isalpha(c) || ft_isdigit(c))
		return (1);
	return (0);
}
