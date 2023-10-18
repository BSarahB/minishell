/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 21:48:43 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/17 21:48:54 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_too_many_arg_msg(t_simpleCmd *simpleCmd)
{
	if(simpleCmd->exit_solo == 1)
		ft_putstr_fd("exit\n", 2);
	ft_putstr_fd("minishell: exit: too many arguments\n", 2);//mettre en place TODO l erreur ERRNO le msg approprie errno
	simpleCmd->exit_code = 1;
	printf("exit_code = %d \n", simpleCmd->exit_code);
}



int ft_numeric_arg_required_msg(t_simpleCmd *simpleCmd, char *str)
{
		if(simpleCmd->exit_solo == 1)
			ft_putstr_fd("exit\n", 2);
		ft_putstr_fd("minishell: exit: ", 2);//mettre en place TODO l erreur ERRNO le msg approprie errno
		ft_putstr_fd(str, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(" numeric argument required", 2);
		ft_putstr_fd("\n", 2);//mettre en place TODO l erreur ERRNO le msg approprie errno
		simpleCmd->exit_code = 2;
		return(-1);
}

unsigned long long	ft_atoi_modulo(const char *str)
{
	unsigned long long	result;
	unsigned long long	sign;

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



void ft_check_exit(t_cmd *cmd, t_list *start_lst_token_retokenized, t_simpleCmd *simpleCmd, t_data_env *data_env)
{
	(void)cmd;
	(void)start_lst_token_retokenized;
	(void)simpleCmd;
	(void)data_env;
	t_list *tmp;

	tmp = start_lst_token_retokenized;
	if(tmp != NULL)
	{
		if((ft_strcmp("exit", tmp->content) == 0))
			{
				simpleCmd->is_builtin = 1;
				simpleCmd->builtin = exxit; // == 6
				if(cmd->nb_of_simpleCmds == 1)
					{
						simpleCmd->exit_solo = 1;
						simpleCmd->builtin_solo = 1;
					}
				if(tmp->next == NULL)
				{
					simpleCmd->exit_no_option = 1;
					if(simpleCmd->exit_solo == 1)
					{
						//simpleCmd->exit_str = ft_strdup("exit");
						ft_putstr_fd("exit\n", 2);
						simpleCmd->exit_code = 0;
						printf("exit_code = %d \n", simpleCmd->exit_code);

						return;
					}
					simpleCmd->exit_code = 0; // si exit n est pas solo : exit | ls  ou ls | exit 
					return;
				}
				else
					tmp = tmp->next;
			}
	}

//quelque soit la position on va afficher les messages d erruer mais pas les printf 
	while (tmp != NULL && tmp->position < simpleCmd->end_simpleCmd_pos && simpleCmd->exit_no_option == 0 && simpleCmd->builtin == exxit)
	{ 
//TODO GERER LE CAS OU LA LONGUEUR EST SUPERIEURE AU BUFFER DE 1096->l argument est too long ?
		if(simpleCmd->nofile == 1)
				break;
		if(simpleCmd->nb_of_tokens_in_simpleCmd > 2)
		{
			if(ft_exit_is_arg_valid(tmp->content, simpleCmd) == 1)
				ft_too_many_arg_msg(simpleCmd);
			else
				printf("exit_code = %d \n", simpleCmd->exit_code); //TODO Se debarrasser de cela
			break;

		}
		else
		{
			if(ft_exit_is_arg_valid(tmp->content, simpleCmd) == 1)
				{
					if(simpleCmd->exit_solo == 1)
						ft_putstr_fd("exit\n", 2);
					simpleCmd->exit_code = ft_atoi_modulo(tmp->content);
					printf("result = %d\n", simpleCmd->exit_code);
					printf("exit_code = %d \n", simpleCmd->exit_code);
				}
			else
				printf("exit_code = %d \n", simpleCmd->exit_code);
			break;
		}

/* GESTION DE CAS BORNES LIMITES

a partir ed 19 elements exit 9999999999999999999 donc  
	jusqu a 18 * 9 on  a exit normal 
LONG_MIN	Minimum value for a variable of type long.	-2147483648
LONG_MAX	Maximum value for a variable of type long.	2147483647
ULONG_MAX	Maximum value for a variable of type unsigned long.	4294967295 (0xffffffff)
LLONG_MIN	Minimum value for a variable of type long long	-9223372036854775808
LLONG_MAX	Maximum value for a variable of type long long	9223372036854775807
ULLONG_MAX	Maximum value for a variable of type unsigned long long	18446744073709551615 (0xffffffffffffffff)


OK EN TESTANT DANS LE TERMINAL ON SE REND COMPTE QUE C EST LLONG MAX ET MIN QUI SONT LES BORNES LIMITES 
~exit 9223372036854775807   ->PASSE TRES BIEN et return la valeur 255  
cest des qu on depasse de + 1 que cela ne focntionne plus -> on va devoir
afficher exit
bash: exit: 9223372036854775808: numeric argument required

> 18 -> on a plus exit  


*/
		tmp = tmp->next;
	}

}
