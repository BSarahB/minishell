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

void	ft_free_struct_t_list_lst_token(t_list **lst_token)
{
	//dans lst_token j ai malloc str et content mais le str, je m en occupe dans le parsing du cmd_and_Args je crois 
	if(*lst_token != NULL)
	{
		//free((*lst_token)->content); pas bon deja free dans simpleCmd cmd and args
		if((*lst_token)->content != NULL)
		{
			free((*lst_token)->content);
			(*lst_token)->content = NULL;
		}
		ft_free_struct_t_list_lst_token(&((*lst_token)->next));
		free(*lst_token);
		*lst_token = NULL;
	}
}

void	ft_free_tab2(char ***tab)
{
	int	i;

	i = 0;

	if ((*tab)[i])
	{
		ft_free_struct_str(&(*tab)[i]);
	}
	if (*tab != NULL)
	{
		free(*tab);
		*tab = NULL;
	}
}

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
//	ft_free_tab(&(*simpleCmd)->cmd_and_args);
	
	if((*simpleCmd)->cmd_and_args != NULL)
		ft_free_tab(&(*simpleCmd)->cmd_and_args);	
	if((*simpleCmd)->abs_cmd_and_args != NULL)
		ft_free_tab(&(*simpleCmd)->abs_cmd_and_args);
	if((*simpleCmd)->outfile != NULL)
		ft_free_tab(&(*simpleCmd)->outfile);
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
	ft_free_tab(&(*cmd)->path_tab);
	ft_free_struct_t_simpleCmds(&(*cmd)->simpleCmds);
	ft_free_struct_t_list_lst_token(&(*cmd)->lst_token);
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
