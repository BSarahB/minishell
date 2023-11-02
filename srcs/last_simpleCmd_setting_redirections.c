/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_simpleCmd_setting_redirections.c              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 10:35:28 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/08/09 10:35:37 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	ft_set_fdin_in_last_but_not_first_simplecmd(t_settings *set, \
										t_cmd *cmd, int flag_random_heredoc)
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

void	ft_check_fds(t_cmd *cmd, t_settings *set)
{
	size_t	k;

	k = 0;
	while (k < cmd->simplecmds[set->i]->nb_of_outfile_before_nofile)
	{
		ft_open_outfiles_in_last_but_not_first_simplecmd(set, cmd, k);
		k++;
		if (set->fdout != -1)
		{
			break ;
			close(set->fdout);
		}
	}
}

void	ft_open_infiles_in_last_but_not_first_simplecmd(t_settings *set, \
																t_cmd *cmd)
{
	int	flag_random_heredoc;

	flag_random_heredoc = 0;
	while (set->j < cmd->simplecmds[set->i]->nb_of_infile)
	{
		flag_random_heredoc = ft_set_fdin_in_last_but_not_first_simplecmd(\
											set, cmd, flag_random_heredoc);
		if (set->fdin == -1 || set->fdin == -2)
		{
			cmd->simplecmds[set->i]->nofile = 1;
			ft_set_fdin_error_msg(set, cmd);
			if (cmd->simplecmds[set->i]->outfile != NULL \
				&& cmd->simplecmds[set->i]->nb_of_outfile_before_nofile != 0)
			{
				ft_check_fds(cmd, set);
			}
			set->j = 0;
			break ;
		}
		set->j++;
	}
}

void	ft_last_simplecmd_chech_outfiles(t_settings *set, t_cmd *cmd)
{
	while (set->j < cmd->simplecmds[set->i]->nb_of_outfile)
	{
		ft_open_outfiles(set, cmd);
		if (cmd->simplecmds[set->i]->out_denied == 1)
		{
			cmd->simplecmds[set->i]->out_denied = 0;
			break ;
		}
		(set->j)++;
	}
}

void	ft_last_simplecmd(t_settings *set, t_cmd *cmd)
{
	set->j = 0;
	if (cmd->simplecmds[set->i]->outfile != NULL \
		&& (cmd->simplecmds[set->i]->nofile == 0))
		ft_last_simplecmd_chech_outfiles(set, cmd);
	else
		set->fdout = dup(set->saveout);
	set->j = 0;
	if (cmd->simplecmds[set->i]->infile != NULL && set->i != 0)
	{
		ft_open_infiles_in_last_but_not_first_simplecmd(set, cmd);
		if (set->j != 0)
		{
			dup2(set->fdin, STDIN_FILENO);
			if (set->fdin != -1)
				close(set->fdin);
		}
		set->j = 0;
	}
	if (cmd->simplecmds[set->i]->nofile == 1 || (cmd->simplecmds[set->i] \
				== NULL || cmd->simplecmds[set->i]->cmd_and_args == NULL))
		ft_redirect_output(set);
}
