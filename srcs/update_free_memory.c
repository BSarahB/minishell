/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_free_memory.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 14:10:57 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/03/13 14:10:59 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	ft_free_tab(char ***tab)
{
	int	i;

	i = 0;
	while ((*tab)[i])
	{
		ft_free_struct_str(&(*tab)[i]);
		i++;
	}
	if (*tab != NULL)
	{
		free(*tab);
		*tab = NULL;
	}
}

void	ft_free_struct_t_simpleCmd(t_simpleCmd **simpleCmd)
{
//on est proteges dans notre code contre le double free
	ft_free_tab(&(*simpleCmd)->cmd_and_args);
	ft_free_tab(&(*simpleCmd)->abs_cmd_and_args);
	if (*simpleCmd != NULL)
	{
		free(*simpleCmd);
		*simpleCmd = NULL;
	}
}

void	ft_free_struct_t_simpleCmds(t_simpleCmd ***simpleCmds)
{
	int	i;

	i = 0;
	while ((*simpleCmds)[i])
	{
		ft_free_struct_t_simpleCmd(&(*simpleCmds)[i]);
		i++;
	}
	if (*simpleCmds != NULL)
	{
		free(*simpleCmds);
		*simpleCmds = NULL;
	}
}

void	ft_free_struct_t_cmd(t_cmd **cmd)
{
	//TODO : plus tard free les 3 io char * quand ils seront utilises
	//ft_free_tab(&(cmd->simpleCmds[i]->cmd_and_args)); on a deja free ces elements dans la ft_execute_cmd
	
	ft_free_tab(&(*cmd)->path_tab);
	ft_free_tab(&(*cmd)->blocks);
	
	ft_free_struct_t_simpleCmds(&(*cmd)->simpleCmds);

	if (*cmd != NULL)
	{
		free(*cmd);
		*cmd = NULL;
	}
}

void	ft_free_struct_t_cmd_only(t_cmd **cmd)
{
	if (*cmd != NULL)
	{
		free(*cmd);
		*cmd = NULL;
	}
}
