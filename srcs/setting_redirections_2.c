/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setting_redirections_2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 19:06:24 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/09/05 19:06:45 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_redirect_output(t_settings *set)
{
	dup2(set->fdout, STDOUT_FILENO);
	if (set->fdout != -1)
		close(set->fdout);
}

void	ft_redirect_input(t_settings *set, t_cmd *cmd)
{
	(void)cmd;
	dup2(set->fdin, STDIN_FILENO);
	if (set->fdin != -1)
		close(set->fdin);
}

void	ft_set_fdin_for_first_simplecmd(t_settings *set, t_cmd *cmd)
{
	if (cmd->simplecmds[set->i]->infile != NULL)
		ft_first_simplecmd_w_infile(set, cmd);
	else
		set->fdin = dup(set->savein);
}

void	ft_save_in_and_out(t_settings *set)
{
	set->savein = dup(0);
	set->saveout = dup(1);
}

void	ft_restore_original_in_and_out(t_settings *set)
{
	dup2(set->savein, 0);
	dup2(set->saveout, 1);
	close(set->savein);
	close(set->saveout);
}
