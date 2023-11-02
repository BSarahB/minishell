/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   regular_simpleCmd_setting_redirections2.c          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 10:35:58 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/08/09 10:36:03 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	ft_set_fdin_for_regular_simplecmd(t_settings *set, t_cmd *cmd, \
									int flag_random_heredoc)
{
	if (set->j != 0 && set->fdin)
	{
		if (flag_random_heredoc == 1)
			flag_random_heredoc = 0;
		else
			close(set->fdin);
	}
	if (cmd->simplecmds[set->i]->heredoc_track_index[set->j] == 1)
		flag_random_heredoc = 1;
	if (cmd->simplecmds[set->i]->heredoc_track_index[set->j] == 42)
		set->fdin = open(".heredoc", O_RDONLY);
	else
	{
		if (cmd->simplecmds[set->i]->heredoc_track_index[set->j] == -1)
			set->fdin = open(cmd->simplecmds[set->i]->infile[set->j], O_RDONLY);
		if (cmd->simplecmds[set->i]->heredoc_track_index[set->j] == 2)
			set->fdin = -2;
	}
	return (flag_random_heredoc);
}
