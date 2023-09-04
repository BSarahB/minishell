/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 11:54:18 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/09/04 11:54:32 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int ft_get_last_heredoc_index(int *tab, int len)
{
	int simpleCmd_index;

	len = len - 1;
	simpleCmd_index = len;
	if(!len)
		return (0);
	while(len >= 0)
	{
		if(tab[len] == 0)
			len = len -1;
		else
		{
			simpleCmd_index = len;
			break;
		}
	}
	return (simpleCmd_index);
}

int ft_get_max_heredoc_index(int *tab, int len)
{
	int max_value;
	int simpleCmd_index;

	if(!len)
		return (0);
	max_value = tab[--len];
	simpleCmd_index = len;
	while(len--)
	{
		if(tab[len]>max_value)
			{
				max_value = tab[len];
				simpleCmd_index = len;
			}
	}
	return (simpleCmd_index);
}

void	ft_tag_last_heredoc_in_infile(t_simpleCmd *simpleCmd)
{
	int i;

	i = 0;
	//TODO proteger si heredoc_track_index a un pb
	while(simpleCmd->heredoc_track_index[i]!= '\0')
		i++;
	
	while(i)
	{
		i--;
		if(simpleCmd->heredoc_track_index[i] == 1 )//simpleCmd->heredoc_track_index[i] != '\0' && 
			{
				simpleCmd->heredoc_track_index[i] = 42;
				break;
			}
	}
	//printf(" index de heredoc_track_index : i = %d et de infile : %s\n", i, simpleCmd->infile[i]);
}

void	ft_get_last_heredoc_position(t_cmd *cmd)
{
	int	simpleCmd_index;
	simpleCmd_index = 0;
	if(cmd->nb_of_heredocs != 0)
	{
		//1 recuperer l index de la simpleCmd qui a le plus de heredoc
	//	simpleCmd_index = ft_get_max_heredoc_index(cmd->heredocs_track_index, cmd->nb_of_simpleCmds);
		//printf("simpleCmd_index = %d\n", simpleCmd_index);
		simpleCmd_index = ft_get_last_heredoc_index(cmd->heredocs_track_index, cmd->nb_of_simpleCmds);

		//2 parcourir l infile prendre le last infile et lui mettre le tag de 42 dans le heredoc_track_index de la simpleCmd
		ft_tag_last_heredoc_in_infile(cmd->simpleCmds[simpleCmd_index]);

		//3 mettre dans l execution (setting redirections) la condition pour empecher les heredocs d etre ouverts comme des infile, SAUF le Last qui a le tag. auquel cas il faudra l ouvrir  
	}
}