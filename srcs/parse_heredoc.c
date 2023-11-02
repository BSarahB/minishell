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

int	ft_get_last_heredoc_index(int *tab, int len)
{
	int	simplecmd_index;

	len = len - 1;
	simplecmd_index = len;
	if (!len)
		return (0);
	while (len >= 0)
	{
		if (tab[len] == 0)
			len = len -1;
		else
		{
			simplecmd_index = len;
			break ;
		}
	}
	return (simplecmd_index);
}

int	ft_get_max_heredoc_index(int *tab, int len)
{
	int	max_value;
	int	simplecmd_index;

	if (!len)
		return (0);
	max_value = tab[--len];
	simplecmd_index = len;
	while (len--)
	{
		if (tab[len] > max_value)
		{
			max_value = tab[len];
			simplecmd_index = len;
		}
	}
	return (simplecmd_index);
}

void	ft_tag_last_heredoc_in_infile(t_simpleCmd *simpleCmd)
{
	int	i;

	i = 0;
	while (simpleCmd->heredoc_track_index[i] != '\0')
		i++;
	while (i)
	{
		i--;
		if (simpleCmd->heredoc_track_index[i] == 1)
		{
			simpleCmd->heredoc_track_index[i] = 42;
			break ;
		}
	}
}

void	ft_get_last_heredoc_position(t_cmd *cmd)
{
	int	simplecmd_index;

	simplecmd_index = 0;
	if (cmd->nb_of_heredocs != 0)
	{
		simplecmd_index = ft_get_last_heredoc_index(cmd->heredocs_track_index, \
													cmd->nb_of_simplecmds);
		ft_tag_last_heredoc_in_infile(cmd->simplecmds[simplecmd_index]);
	}
}
