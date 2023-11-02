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

void	ft_free_struct_t_simplecmd(t_simpleCmd **simpleCmd)
{
	ft_free_1(simpleCmd);
	ft_free_2(simpleCmd);
}

void	ft_free_struct_t_simplecmds(t_simpleCmd ***simpleCmds)
{
	int	i;

	i = 0;
	while ((*simpleCmds)[i])
	{
		ft_free_struct_t_simplecmd(&(*simpleCmds)[i]);
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
	if ((*cmd)->path_tab != NULL)
		ft_free_tab(&(*cmd)->path_tab);
	if ((*cmd)->simplecmds != NULL)
		ft_free_struct_t_simplecmds(&(*cmd)->simplecmds);
	if ((*cmd)->lst_token != NULL)
		ft_free_struct_t_list_lst_token(&(*cmd)->lst_token);
	if ((*cmd)->lst_token_retokenized != NULL)
		ft_free_struct_t_list_lst_token(&(*cmd)->lst_token_retokenized);
	if ((*cmd)->heredocs_track_index != NULL)
		ft_free_struct_int_tab(&(*cmd)->heredocs_track_index);
	if ((*cmd)->nb_of_heredocs != 0)
	{
		ft_free_tab(&(*cmd)->heredocs);
		unlink(".heredoc");
	}
}

void	ft_free_in_child(t_cmd *cmd, t_data *data, char *line)
{
	ft_free_struct_str(&line);
	if (cmd != NULL)
		ft_free_struct_t_cmd(&cmd);
	ft_free_struct_t_data(&data);
}

void	ft_free(t_cmd *cmd, t_list *lst_token, t_data *data, char *line)
{
	ft_free_struct_str(&line);
	if (cmd != NULL)
		ft_free_struct_t_cmd(&cmd);
	else
	{
		if (lst_token)
			ft_free_struct_t_list_lst_token(&lst_token);
	}
	ft_free_struct_t_data(&data);
}
