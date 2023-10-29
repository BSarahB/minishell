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

void ft_too_many_arg_msg(t_simpleCmd *simpleCmd)
{
	if(simpleCmd->exit_solo == 1)
		ft_putstr_fd("exit\n", 1);
	ft_putstr_fd("minishell: exit: too many arguments\n", 2);//mettre en place TODO l erreur ERRNO le msg approprie errno
	simpleCmd->exit_code = 1;
}

int ft_numeric_arg_required_msg(t_simpleCmd *simpleCmd, char *str)
{
	if(simpleCmd->exit_solo == 1)
		ft_putstr_fd("exit\n", 1);
	ft_putstr_fd("minishell: exit: ", 2);//mettre en place TODO l erreur ERRNO le msg approprie errno
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(" numeric argument required", 2);
	ft_putstr_fd("\n", 2);//mettre en place TODO l erreur ERRNO le msg approprie errno
	simpleCmd->exit_code = 2;
	return(-1);
}

unsigned long long	ft_atoi_modulo(char *str, t_simpleCmd *simpleCmd )
{
	unsigned long long	result;
	int	sign;
	char *tmp;

	tmp = str;
	result = 0;
	sign = 1;
	while (*str == ' ' || *str == '\t' || *str == '\n' \
		 || *str == '\v' || *str == '\f')
		str++;
	if (*str == '-')
		sign = -1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str <= '9' && *str >= '0')
	{
		result = result * 10 + *str - '0';
		if((sign == 1 && result > LLONG_MAX) || ( result > (-1 * (unsigned long)LLONG_MIN) && sign == -1 ))//(result - min > 1 && sign == -1)) // -9223372036854775808(OK LLONGMIN) > -9223372036854775809(OUT OF RANGE) caster pour compatibilite
			{
				ft_numeric_arg_required_msg(simpleCmd, tmp);
				return(2);
			}
		str++;
	}
	result = sign * result;
	result = result % 256;
	return (result);
}

int ft_exit_is_arg_valid(char *str, t_simpleCmd *simpleCmd) // export VAR="   123    120"  gerer le cas de exit $Q --> exit code ==1  mbenmesb@bess-f2r6s5:~$ exit "$Q" -> exit bash: exit: : numeric argument required car apres dequote on obtient \0
{
	int i;

	i = 0;
	if(str == NULL) //cqfd
		return(ft_numeric_arg_required_msg(simpleCmd, str));
	if(str[i] == '\0') //cas du exit "$Q" ->soit exit  \0
		return(ft_numeric_arg_required_msg(simpleCmd, str));
	while(ft_is_space(str[i])) //~exit "    " -> apres dequote les espaces sont preoteges donc il faut gerer le cas ou str[i] == \0
		i++;
	if(str[i] == '\0')
		return(ft_numeric_arg_required_msg(simpleCmd, str));
	//ne sera accepte qu un seul signe + ou - 
	if(str[i] == '+' || str[i] == '-')
		i++;
	if(ft_isdigit(str[i]) == 0) //exit "   + 12" ou exit + 1
			return(ft_numeric_arg_required_msg(simpleCmd, str));
	while(str[i])
	{
		if(ft_isdigit(str[i]) == 0 || ft_is_space(str[i]) == 1)
			return(ft_numeric_arg_required_msg(simpleCmd, str));
		i++;
	}
	return(1); //arg est valide
}