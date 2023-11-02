/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_check_key.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 00:18:38 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/30 00:18:49 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	ft_check_format_is_key_value(t_list *tmp)
{
	char	*str;
	int		i;

	i = 0;
	str = tmp->content;
	while (str[i])
	{
		if (str[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

int	ft_isalnum(int c)
{
	return (ft_isdigit(c) || ft_isalpha(c));
}

int	ft_not_a_valid_identifiar(char *str, t_simpleCmd *simpleCmd, t_list *tmp)
{
	if (str[0] == '\0')
	{
		ft_putstr_fd("bash: export: `': not a valid identifier\n", 2);
		simpleCmd->exit_code = 1;
		return (0);
	}
	if (str[0])
	{
		if (ft_isalpha(str[0]) == 0 && str[0] != '_')
		{
			ft_putstr_fd("bash: export: `", 2);
			ft_putstr_fd(tmp->content, 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			simpleCmd->exit_code = 1;
			return (0);
		}
	}
	return (1);
}

int	ft_key_value_is_valid(t_list *tmp, t_simpleCmd *simpleCmd)
{
	char	*str;
	int		i;

	i = 1;
	str = tmp->content;
	if (ft_not_a_valid_identifiar(str, simpleCmd, tmp) == 0)
		return (0);
	while (str[i])
	{
		if (ft_isalnum(str[i]) == 0 && str[i] != '_' && str[i] != '=')
		{
			ft_putstr_fd("bash: export: `", 2);
			ft_putstr_fd(tmp->content, 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			simpleCmd->exit_code = 1;
			return (0);
		}
		if (str[i] == '=')
			return (1);
		i++;
	}
	return (1);
}
