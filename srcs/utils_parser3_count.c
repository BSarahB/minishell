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

t_list *ft_count_nb_of_redir_append_in_sCmd(t_simpleCmd *simpleCmd, t_list *tmp)
{
	simpleCmd->nb_of_outfile++;
	simpleCmd->nb_of_redir_token =simpleCmd->nb_of_redir_token + 2;
	tmp = tmp->next;
	return(tmp);
}


t_list *ft_count_nb_of_redir_err_in_sCmd(t_simpleCmd *simpleCmd, t_list *tmp)
{
	simpleCmd->nb_of_errfile++;
	simpleCmd->nb_of_redir_token =simpleCmd->nb_of_redir_token + 2;
	tmp = tmp->next;
	return(tmp);
}

t_list *ft_count_nb_of_redir_out_in_sCmd(t_simpleCmd *simpleCmd, t_list *tmp)
{
	simpleCmd->nb_of_outfile++;
	simpleCmd->nb_of_redir_token = simpleCmd->nb_of_redir_token + 2;
	tmp = tmp->next;
	return(tmp);
}

 void	ft_count_nb_of_redir_token_in_simpleCmd(t_cmd *cmd, t_simpleCmd *simpleCmd, t_list *start_lst_token, size_t i)
 {
	t_list	*tmp;
	int		k;

	k = 0;
	tmp = start_lst_token;
	while(tmp != NULL && tmp->position < simpleCmd->end_simpleCmd_pos)
	{
		if(tmp->title == redir_in || tmp->title == redir_heredoc)
		{
			simpleCmd->nb_of_infile++;
			simpleCmd->nb_of_redir_token =simpleCmd->nb_of_redir_token + 2;
			if(tmp->title == redir_heredoc)
			{
				k++;
				cmd->heredocs_track_index[i] = k;
				simpleCmd->nb_of_heredoc++;
			}
			tmp =tmp->next;
		}
		else if(tmp->title == redir_out)
			tmp = ft_count_nb_of_redir_out_in_sCmd(simpleCmd, tmp);
		else if(tmp->title == redir_append)
			tmp = ft_count_nb_of_redir_append_in_sCmd(simpleCmd, tmp);
		else if(tmp->title == redir_err)
			tmp = ft_count_nb_of_redir_err_in_sCmd(simpleCmd, tmp);
		tmp = tmp->next;
	}
 }
