/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_interaction_2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 17:59:22 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/09/05 17:59:41 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

extern int g_signal; 

#include "minishell.h"

void	ft_heredoc_input_is_null(t_cmd *cmd, size_t i)
{
	if(g_signal != HD_STOP)
	{
		cmd->line_count = cmd->line_count -1;						
		ft_error_heredoc(cmd->heredocs[i], cmd->line_count);
		if(cmd->heredocs[++i])
			ft_heredoc_interaction(cmd, i, 2);
	}
}

void	ft_add_history_and_free_rl(char *line_heredoc)
{
	add_history(line_heredoc); 
	free(line_heredoc);
}

int ft_last_heredoc(t_cmd *cmd, char *line_heredoc, size_t i)
{
	if(i  == cmd->k -1)
	{
		ft_add_history_and_free_rl(line_heredoc);
		return(1);
	}
	return(0);
}