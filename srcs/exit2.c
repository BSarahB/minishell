/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 21:02:25 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/29 21:02:40 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	ft_too_many_arg_msg(t_simpleCmd *simpleCmd)
{
	if (simpleCmd->exit_solo == 1)
		ft_putstr_fd("exit\n", 1);
	ft_putstr_fd("minishell: exit: too many arguments\n", 2);
	simpleCmd->exit_code = 1;
}

int	ft_numeric_arg_required_msg(t_simpleCmd *simpleCmd, char *str)
{
	if (simpleCmd->exit_solo == 1)
		ft_putstr_fd("exit\n", 1);
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(" numeric argument required", 2);
	ft_putstr_fd("\n", 2);
	simpleCmd->exit_code = 2;
	return (-1);
}

unsigned long long	ft_atoi_modulo(char *s, t_simpleCmd *simpleCmd )
{
	unsigned long long	result;
	int					sign;
	char				*tmp;

	tmp = s;
	result = 0;
	sign = 1;
	while (*s == ' ' || (*s >= 9 && *s <= 13))
		s++;
	if (*s == '-')
		sign = -1;
	if (*s == '-' || *s == '+')
		s++;
	while (*s <= '9' && *s >= '0')
	{
		result = result * 10 + *s - '0';
		if ((sign == 1 && result > LLONG_MAX) || \
			(result > (-1 * (unsigned long)LLONG_MIN) && sign == -1))
		{
			ft_numeric_arg_required_msg(simpleCmd, tmp);
			return (2);
		}
		s++;
	}
	return ((sign * result) % 256);
}

int	ft_exit_is_arg_valid(char *str, t_simpleCmd *simpleCmd)
{
	int	i;

	i = 0;
	if (str == NULL)
		return (ft_numeric_arg_required_msg(simpleCmd, str));
	if (str[i] == '\0')
		return (ft_numeric_arg_required_msg(simpleCmd, str));
	while (ft_is_space(str[i]))
		i++;
	if (str[i] == '\0')
		return (ft_numeric_arg_required_msg(simpleCmd, str));
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (ft_isdigit(str[i]) == 0)
		return (ft_numeric_arg_required_msg(simpleCmd, str));
	while (str[i])
	{
		if (ft_isdigit(str[i]) == 0 || ft_is_space(str[i]) == 1)
			return (ft_numeric_arg_required_msg(simpleCmd, str));
		i++;
	}
	return (1);
}
