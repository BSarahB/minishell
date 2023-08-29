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
	if(set->j != 0 && set->fdout)//TODO proteger des pbs a l ouverture
		close(set->fdout);
	if(cmd->simpleCmds[set->i]->append_track_index[set->j] == 1)
		set->fdout = open(cmd->simpleCmds[set->i]->outfile[set->j], O_CREAT | O_RDWR | O_APPEND, 0644);
	else
		set->fdout = open(cmd->simpleCmds[set->i]->outfile[set->j], O_CREAT | O_RDWR | O_TRUNC, 0644);
	// if(fdout == -1) gerer les erreurs d ouverture ici avec perror
	if(set->fdout == -1)
	{
		perror("minishell");
		exit(1);
		//fermer les pipes, nettoyer la memoire etc...
	}
}

void	ft_open_outfiles_in_last_but_not_first_simpleCmd(t_settings *set, t_cmd *cmd, int k)
{
	if(k != 0 && set->fdout)//TODO proteger des pbs a l ouverture
		close(set->fdout);
	set->j = k;
	if(cmd->simpleCmds[set->i]->append_track_index[set->j] == 1)
		set->fdout = open(cmd->simpleCmds[set->i]->outfile[set->j], O_CREAT | O_RDWR | O_APPEND, 0644);
	else
		set->fdout = open(cmd->simpleCmds[set->i]->outfile[set->j], O_CREAT | O_RDWR | O_TRUNC, 0644);
	// if(fdout == -1) gerer les erreurs d ouverture ici avec perror
	if(set->fdout == -1)
	{
		perror("minishell");
		exit(1);
		//fermer les pipes, nettoyer la memoire etc...
	}				
}