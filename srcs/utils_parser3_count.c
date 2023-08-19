/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parser3_count.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 18:35:48 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/08/14 18:36:04 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_count_final_nb_of_tokens_in_simpleCmd(t_list *start_lst_token, t_simpleCmd *simpleCmd)
{
	size_t	k;
	size_t	token_in_simpleCmd_nbr;
	t_list	*tmp;
	(void)simpleCmd;
	(void)k;

	token_in_simpleCmd_nbr = 0;
	tmp = start_lst_token;
	while (tmp !=NULL && tmp->position < simpleCmd->end_simpleCmd_pos)
	{
		token_in_simpleCmd_nbr++;

		tmp = tmp->next;
	}
	if(tmp !=NULL && tmp->position == simpleCmd->end_simpleCmd_pos && tmp->title != operator)
	{
		token_in_simpleCmd_nbr++;
		simpleCmd->nb_of_tokens_in_simpleCmd = token_in_simpleCmd_nbr;
		return;
	}
	simpleCmd->nb_of_tokens_in_simpleCmd = token_in_simpleCmd_nbr;
}

 void	ft_count_nb_of_redir_token_in_simpleCmd(t_cmd *cmd, t_simpleCmd *simpleCmd, t_list *start_lst_token)
 {
	t_list *tmp;
	(void)cmd;

	tmp = start_lst_token;
	while(tmp != NULL && tmp->position < simpleCmd->end_simpleCmd_pos)
	{
		if(tmp->title == redir_in)
		{
			simpleCmd->nb_of_infile++;
			simpleCmd->nb_of_redir_token =simpleCmd->nb_of_redir_token +2;
			tmp =tmp->next;
		}
		else if(tmp->title == redir_out)
		{
			simpleCmd->nb_of_outfile++;
			simpleCmd->nb_of_redir_token =simpleCmd->nb_of_redir_token +2;
			tmp = tmp->next;
		}
		else if(tmp->title == redir_err)
		{
			simpleCmd->nb_of_errfile++;
			simpleCmd->nb_of_redir_token =simpleCmd->nb_of_redir_token +2;
			tmp = tmp->next;
		}
		
		else if(tmp->title == redir_append)
		{
			simpleCmd->nb_of_outfile++;
			simpleCmd->nb_of_redir_token =simpleCmd->nb_of_redir_token +2;
			tmp = tmp->next;
		}
			tmp = tmp->next;
	}
 }