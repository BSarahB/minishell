/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_init_parser.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 11:46:30 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/06/21 11:46:38 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_init_t_param_countsp(t_param_countsp *para, size_t k, \
									size_t simpleCmd_nbr, t_list *tmp)
{
	para->k = k;
	para->simplecmd_nbr = simpleCmd_nbr;
	para->tmp = tmp;
}

size_t	ft_count_simplecmds_nbr(t_list *lst_token)
{
	t_param_countsp	para;

	ft_init_t_param_countsp(&para, 0, 0, NULL);
	ft_simplify_list(lst_token);
	para.tmp = lst_token;
	while (para.tmp)
	{
		while (para.tmp && para.tmp->title == operator)
			para.tmp = para.tmp->next;
		while (para.tmp && para.tmp->title != operator)
		{
			if (para.tmp->content != NULL)
			{
				para.k++;
				para.tmp = para.tmp->next;
			}
			else
				break ;
		}
		if (para.k != 0)
			para.simplecmd_nbr++;
		if (para.tmp)
			para.tmp = para.tmp->next;
	}
	return (para.simplecmd_nbr);
}

void	ft_struct_update_cmd(t_list *lst_token, size_t k, t_cmd **cmd, \
													size_t nbr_of_simpleCmds)
{
	(*cmd)->simplecmds[k] = 0;
	(*cmd)->nb_of_heredocs = 0;
	(*cmd)->k = 0;
	(*cmd)->line_count = 0;
	(*cmd)->nb_of_simplecmds = nbr_of_simpleCmds;
	(*cmd)->background = 0;
	(*cmd)->path_tab = 0;
	(*cmd)->lst_token = lst_token;
	(*cmd)->lst_token_retokenized = NULL;
	(*cmd)->flag_head_list = -1;
	(*cmd)->flag_empty_head_list = -1;
	(*cmd)->heredocs = NULL;
	(*cmd)->outputfile = NULL;
	(*cmd)->inputfile = NULL;
	(*cmd)->errfile = NULL;
	(*cmd)->background = 0;
}

int	ft_allocate_cmd(t_cmd **cmd, size_t nbr_of_simpleCmds)
{
	*cmd = (t_cmd *)malloc(sizeof (t_cmd));
	if (!(*cmd))
		return (0);
	(*cmd)->simplecmds = \
		malloc(sizeof (t_simpleCmd *) * (nbr_of_simpleCmds + 1));
	if (!((*cmd)->simplecmds))
		return (0);
	return (1);
}

t_cmd	*ft_struct_init_cmd(t_cmd **cmd, t_list *lst_token)
{
	size_t		k;
	size_t		nbr_of_simplecmds;
	t_simpleCmd	*simplecmd;
	int			*heredocs_track_index;

	k = 0;
	heredocs_track_index = NULL;
	nbr_of_simplecmds = ft_count_simplecmds_nbr(lst_token);
	if (!ft_allocate_cmd(cmd, nbr_of_simplecmds))
		return (NULL);
	while (k < nbr_of_simplecmds)
	{
		(*cmd)->simplecmds[k] = ft_struct_init_simplecmd(&simplecmd);
		k++;
	}
	(*cmd)->heredocs_track_index = ft_init_ctab(&heredocs_track_index, \
														nbr_of_simplecmds, 0);
	if (!((*cmd)->heredocs_track_index))
		return (NULL);
	ft_struct_update_cmd(lst_token, k, cmd, nbr_of_simplecmds);
	return (*cmd);
}
