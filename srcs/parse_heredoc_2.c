/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 12:23:29 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/09/04 12:23:43 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_malloc_heredocs_tab(t_cmd *cmd)
{
	char **heredocs;

	heredocs = NULL;
	heredocs = malloc(sizeof(*heredocs) * (cmd->nb_of_heredocs + 1));
	if(!heredocs)
		return(NULL);
	return(heredocs);
}

void	ft_count_nbr_of_heredocs_in_cmd_line(t_cmd *cmd)
{
	t_list *tmp;

	tmp = cmd->lst_token;
	while(tmp != NULL)
	{
		if(tmp->title == redir_heredoc)
			{
				cmd->nb_of_heredocs++;
				tmp =tmp->next;
			}
		tmp = tmp->next;
	}
}

void	ft_malloc_heredocs_of_cmd(t_cmd *cmd)
{
	ft_count_nbr_of_heredocs_in_cmd_line(cmd);
	if(cmd->nb_of_heredocs != 0)
		cmd->heredocs = ft_malloc_heredocs_tab(cmd);
}