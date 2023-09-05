/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir_head_token.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 11:54:52 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/09/04 11:55:07 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_settings_del 	*ft_redir_err_head(t_list *curr, t_cmd *cmd, t_list *lst_token, t_settings_del *del)
{//TODO REGLER L ORDRE COMME POUR LES OUT ET IN
	t_list *lst_token_to_remove;
	t_list *lst_token_to_remove2;

	(void)lst_token;
	lst_token_to_remove = curr;
	lst_token_to_remove2 = curr->next;
	if(cmd->simpleCmds[del->index]->nofile == 0)
		cmd->simpleCmds[del->index]->nb_of_errfile_before_nofile++;
	curr->next->title = redir_err;
	ft_lstdelone(&lst_token_to_remove);
	ft_lstdelone2(&lst_token_to_remove2, cmd->simpleCmds[del->index], del->k, cmd);
	del->k++;
	return(del);
}

t_settings_del *ft_redir_out_head(t_list *curr, t_cmd *cmd, t_list *lst_token, t_settings_del *del)
{
	t_list *lst_token_to_remove;
	t_list *lst_token_to_remove2;

	lst_token_to_remove = curr;
	lst_token_to_remove2 = curr->next;
	if(cmd->simpleCmds[del->index]->nofile == 0)
		cmd->simpleCmds[del->index]->nb_of_outfile_before_nofile++;
	if(curr->title == redir_out)
		curr->next->title = redir_out;
	if(curr->title == redir_append)
		curr->next->title = redir_append;
	if(del->index != 0)
	{
		ft_reconnect_lst_token(lst_token, lst_token_to_remove->position);
		ft_reconnect_lst_token(lst_token, lst_token_to_remove2->position);
	}
	ft_lstdelone(&lst_token_to_remove);
	ft_lstdelone2(&lst_token_to_remove2, cmd->simpleCmds[del->index], del->j, cmd);
	del->j++;
	return(del);
}

t_settings_del	*ft_redir_in_head(t_list *curr, t_cmd *cmd, t_list *lst_token, t_settings_del *del)
{
	t_list *lst_token_to_remove;
	t_list *lst_token_to_remove2;
	int fdin;

	lst_token_to_remove = curr;
	lst_token_to_remove2 = curr->next;
	if(curr->title == redir_in)
		curr->next->title = redir_in;
	if(curr->title == redir_heredoc)
		curr->next->title = redir_heredoc;
	if(cmd->simpleCmds[del->index]->nofile == 0  && curr->next->title == redir_in) //J AI RETIRE UN ->next
	{
		if((fdin = open(lst_token_to_remove2->content, O_RDONLY)) == -1) //TODO revenir ici
			cmd->simpleCmds[del->index]->nofile = 1;
		else
			close(fdin);
	}
	if(del->index != 0)
	{
		ft_reconnect_lst_token(lst_token, lst_token_to_remove->position);
		ft_reconnect_lst_token(lst_token, lst_token_to_remove2->position);
	}
	ft_lstdelone(&lst_token_to_remove);
	ft_lstdelone2(&lst_token_to_remove2, cmd->simpleCmds[del->index], del->i, cmd);
	del->i++;
	return(del);
}

void	ft_flag_head_list(t_cmd *cmd, t_settings_del *del)
{
	if(cmd->flag_head_list == -1 && del->index == 0)
		cmd->flag_head_list = 1;
}

t_list	*ft_head_redir_token(t_list *curr, t_cmd *cmd, t_list *lst_token, t_settings_del *del)
{
	t_list			*next;

	next = NULL;
	ft_flag_head_list(cmd, del);
	next = curr->next->next;
	if(next && del->index == 0)
		next->prev = NULL;
	if(curr->title == redir_in || curr->title == redir_heredoc)
		del = ft_redir_in_head(curr, cmd, lst_token, del);
	else if(curr->title == redir_out || curr->title == redir_append)
		del = ft_redir_out_head(curr, cmd, lst_token, del);
	else if(curr->title == redir_err)
		del = ft_redir_err_head(curr, cmd, lst_token, del);
	curr = next;
	return(curr);
}