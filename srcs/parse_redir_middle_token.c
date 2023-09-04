/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir_middle_token.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 11:55:21 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/09/04 11:55:32 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_settings_del *ft_redir_err_middle(t_list *curr, t_cmd *cmd, t_list *lst_token, t_settings_del *del)
{
	//TODO REGLER L ORDRE COMME POUR LES OUT ET IN
	t_list *lst_token_to_remove;
	t_list *lst_token_to_remove2;

	(void)lst_token;
	lst_token_to_remove = curr->next;
	ft_lstdelone(&lst_token_to_remove);
	curr->next->next->title = redir_err;

	if(cmd->simpleCmds[del->index]->nofile == 0)
		cmd->simpleCmds[del->index]->nb_of_errfile_before_nofile++;

	lst_token_to_remove2 = curr->next->next;
	curr->next = curr->next->next->next;
	ft_lstdelone2(&lst_token_to_remove2,
	 cmd->simpleCmds[del->index], del->k, cmd);
	del->k++;
	if(curr->next == NULL)
		cmd->simpleCmds[del->index]->end_simpleCmd_pos = curr->position;
	return(del);
}

t_settings_del *ft_redir_out_middle(t_list *curr, t_cmd *cmd, t_list *lst_token, t_settings_del *del)
{
	t_list *lst_token_to_remove;
	t_list *lst_token_to_remove2;

	(void)lst_token;
	lst_token_to_remove = curr->next;
	lst_token_to_remove2 = curr->next->next;
	if(curr->next->title == redir_out)
		curr->next->next->title = redir_out;
	if(curr->next->title == redir_append)
		curr->next->next->title = redir_append;
	curr->next = curr->next->next->next;
	ft_lstdelone(&lst_token_to_remove);
	if(cmd->simpleCmds[del->index]->nofile == 0)
		cmd->simpleCmds[del->index]->nb_of_outfile_before_nofile++;
	ft_lstdelone2(&lst_token_to_remove2, cmd->simpleCmds[del->index], del->j, cmd);
	del->j++;
	if(curr->next == NULL)
		cmd->simpleCmds[del->index]->end_simpleCmd_pos = curr->position;
	return(del);
}

t_settings_del *ft_redir_in_middle(t_list *curr, t_cmd *cmd, t_list *lst_token, t_settings_del *del)
{
	t_list *lst_token_to_remove;
	t_list *lst_token_to_remove2;
	int fdin;

	(void)lst_token;
	lst_token_to_remove = curr->next;
	lst_token_to_remove2 = curr->next->next;
	if(curr->next->title == redir_in)
		curr->next->next->title = redir_in;
	if(curr->next->title == redir_heredoc)
		curr->next->next->title = redir_heredoc;
	if(cmd->simpleCmds[del->index]->nofile == 0 && curr->next->next->title == redir_in)
	{
		if((fdin = open(lst_token_to_remove2->content, O_RDONLY)) == -1)
			cmd->simpleCmds[del->index]->nofile = 1;
		else
			close(fdin);
	}
	curr->next =  curr->next->next->next;//ls
	ft_lstdelone(&lst_token_to_remove);
	ft_lstdelone2(&lst_token_to_remove2, cmd->simpleCmds[del->index], del->i, cmd);
	del->i++;
	if(curr->next == NULL)
		cmd->simpleCmds[del->index]->end_simpleCmd_pos = curr->position;
	return(del);
}

t_list	*ft_middle_redir_token(t_list *curr, t_cmd *cmd, t_list *lst_token, t_settings_del *del)
{
	if(curr->next->title == redir_in || curr->next->title == redir_heredoc)
		del = ft_redir_in_middle(curr, cmd, lst_token, del);
	else if(curr->next->title == redir_out || curr->next->title == redir_append)
		del = ft_redir_out_middle(curr, cmd, lst_token, del);
	else if(curr->next->title == redir_err)
		del = ft_redir_err_middle(curr, cmd, lst_token, del);
	else
		curr = curr->next;	
	return(curr);
}