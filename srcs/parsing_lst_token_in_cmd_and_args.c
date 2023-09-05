/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_lst_token_in_cmd_and_args.c                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 18:51:33 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/08/14 18:51:49 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_get_abs_argumentsb(char **abs_c_and_a)
{
	char	slash[2];

	slash[0] = '/';
	slash[1] = 0;
	ft_update_string(&abs_c_and_a[0], ft_strjoin(slash, abs_c_and_a[0]));
	return (abs_c_and_a);
}

int		ft_malloc_and_parse_cmd_and_args_tab_of_simpleCmd(t_list *lst_token, t_simpleCmd *simpleCmd)
{
	size_t	k;
	char	**c_and_a;
	char	**abs_c_and_a;

	k = 0;
	if(simpleCmd->nb_of_tokens_in_simpleCmd == 0)
		return(0);
	c_and_a = malloc(sizeof(*c_and_a) * (simpleCmd->nb_of_tokens_in_simpleCmd + 1));
	abs_c_and_a = malloc(sizeof(*abs_c_and_a) * (simpleCmd->nb_of_tokens_in_simpleCmd + 1));
	if(!c_and_a)
		return(0);
	if(!abs_c_and_a)
		return(0);
	while (k < simpleCmd->nb_of_tokens_in_simpleCmd)
	{
		c_and_a[k] = ft_strdup(lst_token->content);
		abs_c_and_a[k] = ft_strdup(lst_token->content);
		lst_token = lst_token->next;
		k++;
	}
	c_and_a[k] = NULL;
	simpleCmd->cmd_and_args = c_and_a;
	abs_c_and_a[k] = NULL;
	abs_c_and_a = ft_get_abs_argumentsb(abs_c_and_a);
	simpleCmd->abs_cmd_and_args = abs_c_and_a;
	return(1);
}