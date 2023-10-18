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


void	ft_free_struct_t_list_lst_envp(t_listenvp **lst_envp)
{
	//dans lst_token j ai malloc str et content mais le str, je m en occupe dans le parsing du cmd_and_Args je crois 
	if(*lst_envp != NULL)
	{
		if((*lst_envp)->key_value != NULL)
		{
			free((*lst_envp)->key_value);
			(*lst_envp)->key_value = NULL;
		}
	
		ft_free_struct_t_list_lst_envp(&((*lst_envp)->next));
		free(*lst_envp);
		*lst_envp = NULL;
	}
}




void	ft_free_struct_t_list_lst_token(t_list **lst_token)
{
	//dans lst_token j ai malloc str et content mais le str, je m en occupe dans le parsing du cmd_and_Args je crois 
	if(*lst_token != NULL)
	{
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


void	ft_free_struct_t_simpleCmd(t_simpleCmd **simpleCmd)
{
//on est proteges dans notre code contre le double free
//	ft_free_tab(&(*simpleCmd)->cmd_and_args);
	
	if((*simpleCmd)->cmd_and_args != NULL)
		ft_free_tab(&(*simpleCmd)->cmd_and_args);	
	if((*simpleCmd)->abs_cmd_and_args != NULL)
		ft_free_tab(&(*simpleCmd)->abs_cmd_and_args);
	if((*simpleCmd)->outfile != NULL)
		{
			ft_free_tab(&(*simpleCmd)->outfile);
			ft_free_struct_int_tab(&(*simpleCmd)->append_track_index);
		}
	if((*simpleCmd)->heredoc != NULL)
		{
			ft_free_tab(&(*simpleCmd)->heredoc);
		}
	if((*simpleCmd)->infile != NULL)
		{
			ft_free_tab(&(*simpleCmd)->infile);
			ft_free_struct_int_tab(&(*simpleCmd)->heredoc_track_index);
		}

	if((*simpleCmd)->errfile != NULL)
		ft_free_tab(&(*simpleCmd)->errfile);
	if((*simpleCmd)->oldpwd != NULL)
		{
			ft_free_struct_str(&(*simpleCmd)->oldpwd);
		}
	if((*simpleCmd)->pwd != NULL)
		{
			ft_free_struct_str(&(*simpleCmd)->pwd);
		}
	if((*simpleCmd)->exit_str != NULL)
		{
			ft_free_struct_str(&(*simpleCmd)->exit_str);
		}	
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
	if((*cmd)->path_tab != NULL)
		ft_free_tab(&(*cmd)->path_tab);
	
	if((*cmd)->simpleCmds != NULL)
		ft_free_struct_t_simpleCmds(&(*cmd)->simpleCmds);
	
	if((*cmd)->lst_token != NULL)
		ft_free_struct_t_list_lst_token(&(*cmd)->lst_token);
	if((*cmd)->lst_token_retokenized != NULL)
		ft_free_struct_t_list_lst_token(&(*cmd)->lst_token_retokenized);
//	if((*cmd)->lst_envp != NULL)
//		ft_free_struct_t_list_lst_envp(&(*cmd)->lst_envp);
	
	if((*cmd)->heredocs_track_index != NULL)
		ft_free_struct_int_tab(&(*cmd)->heredocs_track_index);
	if((*cmd)->nb_of_heredocs != 0)
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
		if(lst_token)
			ft_free_struct_t_list_lst_token(&lst_token);
	}
	ft_free_struct_t_data(&data);
}

