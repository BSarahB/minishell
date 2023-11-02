/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_free_memory_4.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 11:52:52 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/09/04 11:53:14 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_struct_t_list_lst_token(t_list **lst_token)
{
	if (*lst_token != NULL)
	{
		if ((*lst_token)->content != NULL)
		{
			free((*lst_token)->content);
			(*lst_token)->content = NULL;
		}
		ft_free_struct_t_list_lst_token(&((*lst_token)->next));
		free(*lst_token);
		*lst_token = NULL;
	}
}

void	ft_free_1(t_simpleCmd **simpleCmd)
{
	if ((*simpleCmd)->cmd_and_args != NULL)
		ft_free_tab(&(*simpleCmd)->cmd_and_args);
	if ((*simpleCmd)->abs_cmd_and_args != NULL)
		ft_free_tab(&(*simpleCmd)->abs_cmd_and_args);
	if ((*simpleCmd)->outfile != NULL)
	{
		ft_free_tab(&(*simpleCmd)->outfile);
		ft_free_struct_int_tab(&(*simpleCmd)->append_track_index);
	}
	if ((*simpleCmd)->heredoc != NULL)
		ft_free_tab(&(*simpleCmd)->heredoc);
	if ((*simpleCmd)->infile != NULL)
	{
		ft_free_tab(&(*simpleCmd)->infile);
		ft_free_struct_int_tab(&(*simpleCmd)->heredoc_track_index);
	}
}

void	ft_free_2(t_simpleCmd **simpleCmd)
{
	if ((*simpleCmd)->errfile != NULL)
		ft_free_tab(&(*simpleCmd)->errfile);
	if ((*simpleCmd)->oldpwd != NULL)
		ft_free_struct_str(&(*simpleCmd)->oldpwd);
	if ((*simpleCmd)->pwd != NULL)
		ft_free_struct_str(&(*simpleCmd)->pwd);
	if ((*simpleCmd)->exit_str != NULL)
		ft_free_struct_str(&(*simpleCmd)->exit_str);
	if (*simpleCmd != NULL)
	{
		free(*simpleCmd);
		*simpleCmd = NULL;
	}
}
