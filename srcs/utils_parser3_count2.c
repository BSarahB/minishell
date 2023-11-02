/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parser3_count2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 18:35:48 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/08/14 18:36:04 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_count_final_nb_of_tokens_in_simplecmd_retokenized( \
							t_list *start_lst_token, t_simpleCmd *simpleCmd)
{
	size_t	k;
	size_t	token_in_simplecmd_nbr;
	t_list	*tmp;

	(void)simpleCmd;
	(void)k;
	token_in_simplecmd_nbr = 0;
	tmp = start_lst_token;
	while (tmp != NULL && tmp->position < simpleCmd->end_simplecmd_pos)
	{
		token_in_simplecmd_nbr++;
		tmp = tmp->next;
	}
	if (tmp != NULL && tmp->position == simpleCmd->end_simplecmd_pos \
		&& tmp->title != operator)
	{
		token_in_simplecmd_nbr++;
		simpleCmd->nb_of_tokens_in_simplecmd = token_in_simplecmd_nbr;
		return ;
	}
	simpleCmd->nb_of_tokens_in_simplecmd = token_in_simplecmd_nbr;
}
