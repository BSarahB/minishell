/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   del_and_parse_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 14:34:29 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/09/04 14:34:52 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_lstdelone2(t_list **lst, t_simpleCmd *simpleCmd, size_t i, t_cmd *cmd)
{
	 if(*lst)
	 {
		ft_parse(*lst, simpleCmd, i, cmd);
		free((*lst)->content);
		(*lst)->content = NULL;
		free(*lst);
		*lst = NULL;
	 }
}


void	ft_lstdelone_beta(t_list **lst)
{
	 if(*lst)
	 {
		free((*lst)->content);
		(*lst)->content = NULL;
		free(*lst);
		*lst = NULL;
	 }
}


void	ft_lstdelone(t_list **lst)
{
	 if(*lst)
	 {
		free((*lst)->content);
		(*lst)->content = NULL;
		free(*lst);
		*lst = NULL;
	 }
}