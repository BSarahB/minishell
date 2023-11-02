/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   regular_simpleCmd_setting_redirections.c           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 10:35:58 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/08/09 10:36:03 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	ft_open_infiles_setfds(t_settings *set, t_cmd *cmd)
{
	size_t	k;

	cmd->simplecmds[set->i]->nofile = 1;
	if (set->fdin == -1)
	{
		ft_error_msg(cmd->simplecmds[set->i]->infile[set->j]);
		cmd->simplecmds[set->i]->exit_code = 1;
	}
	if (set->fdin == -2)
	{
		ft_error_msg3(cmd->simplecmds[set->i]->infile[set->j]);
		cmd->simplecmds[set->i]->exit_code = 1;
	}
	if (cmd->simplecmds[set->i]->outfile != NULL \
		&& cmd->simplecmds[set->i]->nb_of_outfile_before_nofile != 0)
	{
		k = 0;
		while (k < cmd->simplecmds[set->i]->nb_of_outfile_before_nofile)
		{
			ft_open_outfiles_in_last_but_not_first_simplecmd(set, cmd, k);
			k++;
		}
	}
}

void	ft_open_infiles(t_settings *set, t_cmd *cmd)
{
	int	flag_random_heredoc;

	flag_random_heredoc = 0;
	while (set->j < cmd->simplecmds[set->i]->nb_of_infile)
	{
		flag_random_heredoc = ft_set_fdin_for_regular_simplecmd(set, cmd, \
														flag_random_heredoc);
		if (set->fdin == -1 || set->fdin == -2)
		{
			ft_open_infiles_setfds(set, cmd);
			break ;
		}
		(set->j)++;
	}
	if (cmd->simplecmds[set->i]->nofile != 1)
		ft_redirect_input(set, cmd);
}

void	ft_create_pipe2(t_settings *set)
{
	if (pipe(set->pip) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	set->fdout = set->pip[1];
	set->fdin = set->pip[0];
	set->pip_exists = 1;
}

void	ft_regular_simplecmd_conditions(t_settings *set, t_cmd *cmd)
{
	ft_create_pipe2(set);
	if ((cmd->simplecmds[set->i]->nb_of_tokens_in_simplecmd == 1) \
		&& (cmd->simplecmds[set->i]->infile == NULL) \
		&& (ft_strcmp(cmd->simplecmds[set->i]->cmd_and_args[0], "cat") == 0))
	{
		if (cmd->simplecmds[set->i + 1]->nofile == 0)
			if ((ft_strcmp(cmd->simplecmds[set->i + 1]->cmd_and_args[0], "wc") \
			!= 0) && (ft_strcmp(cmd->simplecmds[set->i + 1]->cmd_and_args[0], \
			"grep") != 0) && \
				(ft_strcmp(cmd->simplecmds[set->i + 1]->cmd_and_args[0], \
																"rev") != 0))
				close(set->pip[0]);
		if (cmd->simplecmds[set->i + 1]->nofile == 1)
		{
			if (ft_strcmp(cmd->simplecmds[set->i + 1]->cmd_and_args[0], "wc") \
																		== 0)
				close(set->pip[0]);
			else if ((ft_strcmp(cmd->simplecmds[set->i + 1]->cmd_and_args[0], \
				"cat") == 0) || (ft_strcmp(\
				cmd->simplecmds[set->i + 1]->cmd_and_args[0], "grep") == 0) \
				|| (ft_strcmp(cmd->simplecmds[set->i + 1]->cmd_and_args[0], \
																"rev") == 0))
				close(set->pip[0]);
		}
	}
}

void	ft_regular_simplecmd(t_settings *set, t_cmd *cmd)
{
	set->j = 0;
	if (cmd->simplecmds[set->i]->infile != NULL && set->i != 0)
	{
		ft_open_infiles(set, cmd);
		set->j = 0;
	}
	ft_regular_simplecmd_conditions(set, cmd);
	if (cmd->simplecmds[set->i]->outfile != NULL && \
		(cmd->simplecmds[set->i]->nofile == 0))
	{
		while (set->j < cmd->simplecmds[set->i]->nb_of_outfile)
		{
			ft_open_outfiles(set, cmd);
			(set->j)++;
		}
		close(set->pip[1]);
	}
	if (cmd->simplecmds[set->i]->nofile == 1 \
		|| (cmd->simplecmds[set->i] == NULL \
			|| cmd->simplecmds[set->i]->cmd_and_args == NULL))
	{
		close(set->pip[1]);
		ft_redirect_output(set);
	}
	set->j = 0;
}
