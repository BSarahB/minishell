/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_simpleCmd_setting_redirections2.c             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 10:35:28 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/08/09 10:35:37 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	ft_set_fdin_error_msg(t_settings *set, t_cmd *cmd)
{
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
}
