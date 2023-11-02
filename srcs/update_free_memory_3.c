/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_free_memory_3.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 11:52:52 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/09/04 11:53:14 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_struct_t_list_lst_envp(t_listenvp **lst_envp)
{
	if (*lst_envp != NULL)
	{
		if ((*lst_envp)->key_value != NULL)
		{
			free((*lst_envp)->key_value);
			(*lst_envp)->key_value = NULL;
		}
		ft_free_struct_t_list_lst_envp(&((*lst_envp)->next));
		free(*lst_envp);
		*lst_envp = NULL;
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

void	ft_free_struct_int_tab(int **p)
{
	if (*p != NULL)
	{
		free(*p);
		*p = NULL;
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
