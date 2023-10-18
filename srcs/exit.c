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

unsigned long	ft_atoi_modulo(const char *str)
{
	unsigned long	result;
	unsigned long	sign;

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

int ft_exit_arg_check(char *str, t_simpleCmd *simpleCmd) // export VAR="   123    120"  gerer le cas de exit $Q --> exit code ==1  mbenmesb@bess-f2r6s5:~$ exit "$Q" -> exit bash: exit: : numeric argument required car apres dequote on obtient \0

{
	int i;

	i = 0;
	if(str == NULL) //cqfd
		{
		if(simpleCmd->exit_solo == 1)
			ft_putstr_fd("exit\n", 2);
		ft_putstr_fd("minishell: exit: ", 2);//mettre en place TODO l erreur ERRNO le msg approprie errno
		ft_putstr_fd(str, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(" numeric argument required", 2);
		ft_putstr_fd("\n", 2);//mettre en place TODO l erreur ERRNO le msg approprie errno
		simpleCmd->exit_code = 2;
		return (-1);
		}
	if(str[i] == '\0') //cas du exit "$Q" ->soit exit  \0
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
	while(ft_is_space(str[i])) //~exit "    " -> apres dequote les espaces sont preoteges donc il faut gerer le cas ou str[i] == \0
		i++;
	if(str[i] == '\0')
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
	//ne sera accepte qu un seul signe + ou - 
	if(str[i] == '+' || str[i] == '-')
		i++;
	if(ft_isdigit(str[i]) == 0) //exit "   + 12" ou exit + 1
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
	while(str[i])
	{
		if(ft_isdigit(str[i]) == 0 || ft_is_space(str[i]) == 1)
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
//FIXME				//	else if(simpleCmd->exit_solo == 0 && qu on est en last simpleCmd)//TOO IF LAST_SIMPLE_CMD && EXIT_SOLO == 0 recuperer le exit_code 0 si on est en mode no option && que c est la derniere simpleCmd
				//	{
				//		simpleCmd->exit_code = 0; //ls | exit  on a le bon exit code de exit mais on n affiche pas de printf sauf message d erreur
					//return; //il faudrair return l exit code 0
						
				//	}
					simpleCmd->exit_code = 0; // si exit n est pas solo : exit | ls  ou ls | exit 
					return;
				
				}
				else
					tmp = tmp->next;
			}
		

	}


//quelque soit la position on va afficher les messages d erruer mais pas les printf 
	while (tmp != NULL && tmp->position < simpleCmd->end_simpleCmd_pos && simpleCmd->exit_no_option == 0 && simpleCmd->builtin == exxit)
	{ //GERER LE CAS DU EXIT SOLO = 0 MAIS FAISANT PARTIE DUN PIPE EN POSITION DE LAST SIMPLECMD : pas de printf exit mais on recupere le code return
		//j aurai la condition  if (simpleCmd->exit_solo == 1  || simpleCmd->exit solo = 0 && postion de last simpleCmd ->recupere l exit
		//exit 1 2 3 
//exit
//bash: exit: too many arguments
//TODO GERER LE CAS OU LA LONGUEUR EST SUPERIEURE AU BUFFER DE 1096->l argument est too long
		if(simpleCmd->nofile == 1)
			{
				//pas de printf et pas de code -> code == -1
				break;
			}
		if(simpleCmd->nb_of_tokens_in_simpleCmd > 2)
		{
			if(ft_exit_arg_check(tmp->content, simpleCmd) == -1)
				{
					
					printf("exit_code = %d \n", simpleCmd->exit_code);
					break;
				}
			else
			{
					if(simpleCmd->exit_solo == 1)
						ft_putstr_fd("exit\n", 2);
					ft_putstr_fd("minishell: exit: too many arguments\n", 2);//mettre en place TODO l erreur ERRNO le msg approprie errno
					simpleCmd->exit_code = 1;
					printf("exit_code = %d \n", simpleCmd->exit_code);
					break;
				}

		}
		else{
			if(ft_exit_arg_check(tmp->content, simpleCmd) == -1)
				{
					
					printf("exit_code = %d \n", simpleCmd->exit_code);
					break;
				}
			else
			{
					if(simpleCmd->exit_solo == 1)
						ft_putstr_fd("exit\n", 2);
					simpleCmd->exit_code = ft_atoi_modulo(tmp->content);
					printf("result = %d\n", simpleCmd->exit_code);
					printf("exit_code = %d \n", simpleCmd->exit_code);
				}

		}


/*
		if(tmp->content == -1) //fr le ft_atoi et gerer le modulo 256 comme a la pisicne
			simpleCmd->exit_code = 255;
		if(tmp->content == -4)
			simpleCmd->exit_code = 252;
		if(tmp->content == -99999)
			simpleCmd->exit_code = 193;
*/
	

/*
a partir ed 19 elements exit 9999999999999999999 donc  
	jusqu a 18 * 9 on  a exit normal 
LONG_MIN	Minimum value for a variable of type long.	-2147483648
LONG_MAX	Maximum value for a variable of type long.	2147483647
ULONG_MAX	Maximum value for a variable of type unsigned long.	4294967295 (0xffffffff)
LLONG_MIN	Minimum value for a variable of type long long	-9223372036854775808
LLONG_MAX	Maximum value for a variable of type long long	9223372036854775807
ULLONG_MAX	Maximum value for a variable of type unsigned long long	18446744073709551615 (0xffffffffffffffff)

> 18 -> on a plus exit  

*/
	
		//cas ou si j ai trop darguments : imprimer exit sur fd 2 (jamais sur STDOUT puisque exit n est jamais redirige dans un outfile) + too many arguments + exit code == 1
		//	if (ft_exit_arg_is_numeric(tmp) == 1)
		//	{
//
//					ft_get_exit_code(tmp, data_env); // exit code sera recupere avec ft_atoi ->gerer les limites MIN ET MAX comme dans ft_printf
//					ft_printf_exit(tmp, data_env); //sur fd 2 TU NE PRINTF PAS SI CE NEST PAS SOLO

//			}
//			else
				// ici notre argument n est pas valide : il n est pas numeric,  il est dope est n est pas on imprime exit sur fd1
				//on return 2
				//on imprime le message d erreur: bash: exit : 100n : numeric argument required

		tmp = tmp->next;
	}

}
