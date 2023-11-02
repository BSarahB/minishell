/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_settings_redirections.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 17:24:14 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/08/08 17:24:23 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	ft_open_outfiles(t_settings *set, t_cmd *cmd)
{
	if (set->j != 0 && set->fdout)
		close(set->fdout);
	if (cmd->simplecmds[set->i]->append_track_index[set->j] == 1)
		set->fdout = open(cmd->simplecmds[set->i]->outfile[set->j], \
				O_CREAT | O_RDWR | O_APPEND, 0644);
	else if (cmd->simplecmds[set->i]->append_track_index[set->j] == 2)
		set->fdout = -2;
	else
		set->fdout = open(cmd->simplecmds[set->i]->outfile[set->j], \
				O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (set->fdout == -2)
	{
		ft_error_msg3(cmd->simplecmds[set->i]->outfile[set->j]);
		cmd->simplecmds[set->i]->exit_code = 1;
	}
	if (set->fdout == -1)
	{
		perror("minishell");
		cmd->simplecmds[set->i]->exit_code = 1;
		cmd->simplecmds[set->i]->nofile = 1;
		cmd->simplecmds[set->i]->out_denied = 1;
	}
}

void	ft_open_outfiles_in_last_but_not_first_simplecmd(\
										t_settings *set, t_cmd *cmd, int k)
{
	if (k != 0 && set->fdout)
		close(set->fdout);
	set->j = k;
	if (cmd->simplecmds[set->i]->append_track_index[set->j] == 1)
		set->fdout = open(cmd->simplecmds[set->i]->outfile[set->j], \
				O_CREAT | O_RDWR | O_APPEND, 0644);
	else if (cmd->simplecmds[set->i]->append_track_index[set->j] == 2)
		set->fdout = -2;
	else
		set->fdout = open(cmd->simplecmds[set->i]->outfile[set->j], \
				O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (set->fdout == -2)
		ft_error_msg3(cmd->simplecmds[set->i]->outfile[set->j]);
	if (set->fdout == -1)
	{
		perror("minishell");
		cmd->simplecmds[set->i]->exit_code = 1;
		cmd->simplecmds[set->i]->nofile = 1;
		cmd->simplecmds[set->i]->out_denied = 1;
	}
}
