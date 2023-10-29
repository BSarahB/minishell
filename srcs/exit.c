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

void ft_check_exit(t_cmd *cmd, t_list *start_lst_token_retokenized, t_simpleCmd *simpleCmd, t_data_env *data_env)
{
	(void)cmd;
	(void)start_lst_token_retokenized;
	(void)simpleCmd;
	(void)data_env;
	t_list *tmp;

	tmp = NULL;
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
						simpleCmd->exit_str = ft_strdup("exit");
						ft_putstr_fd("exit\n", 2);
						simpleCmd->exit_code = ft_get_exit_status_atoi(data_env->lst_envp);
						data_env->exit_parent = 1;
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
	while (tmp != NULL && tmp->position <= simpleCmd->end_simpleCmd_pos && simpleCmd->exit_no_option == 0 && simpleCmd->builtin == exxit)
	{ 
//TODO GERER LE CAS OU LA LONGUEUR EST SUPERIEURE AU BUFFER DE 1096->l argument est too long ?
		if(simpleCmd->nofile == 1)
				break;
		if(simpleCmd->nb_of_tokens_in_simpleCmd > 2)
		{
			if(ft_exit_is_arg_valid(tmp->content, simpleCmd) == 1)
			{
				ft_too_many_arg_msg(simpleCmd);
				return;
			}
			else
				//printf("exit_code = %d \n", simpleCmd->exit_code); //TODO Se debarrasser de cela
			break;

		}
		else
		{
			if(ft_exit_is_arg_valid(tmp->content, simpleCmd) == 1)
				{
					if(simpleCmd->exit_solo == 1)
					{
						ft_putstr_fd("exit\n", 1);
						data_env->exit_parent = 1;
					}
					simpleCmd->exit_code = ft_atoi_modulo(tmp->content, simpleCmd);
				}
			else
			{
				if(simpleCmd->exit_solo == 1)
				{
					ft_putstr_fd("exit\n", 1);
					data_env->exit_parent = 1;
				}
			}
			break;
		}
		tmp = tmp->next;
	}
}

/* GESTION DE CAS BORNES LIMITES

a partir ed 19 elements exit 9999999999999999999 donc  
	jusqu a 18 * 9 on  a exit normal 
LONG_MIN	Minimum value for a variable of type long.	-2147483648
LONG_MAX	Maximum value for a variable of type long.	2147483647
ULONG_MAX	Maximum value for a variable of type unsigned long.	4294967295 (0xffffffff)
LLONG_MIN	Minimum value for a variable of type long long	-9223372036854775808 ->sera OK
LLONG_MAX	Maximum value for a variable of type long long	9223372036854775807 ->sera OK
ULLONG_MAX	Maximum value for a variable of type unsigned long long	18446744073709551615 (0xffffffffffffffff)


OK EN TESTANT DANS LE TERMINAL ON SE REND COMPTE QUE C EST LLONG MAX ET MIN QUI SONT LES BORNES LIMITES 
~exit 9223372036854775807   ->PASSE TRES BIEN et return la valeur 255  
cest des qu on depasse de + 1 que cela ne focntionne plus -> on va devoir
afficher exit
bash: exit: 9223372036854775808: numeric argument required

> 18 -> on a plus exit  


*/