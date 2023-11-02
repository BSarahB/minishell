/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_parser2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 15:55:07 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/03/07 15:55:30 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_reconnect_lst_token(t_list *lst_token, size_t position)
{
	t_list	*curr;
	t_list	*next;
	t_list	*prev;

	curr = lst_token;
	next = NULL;
	prev = NULL;
	while (curr)
	{
		if (curr->position == position)
		{
			next = curr->next;
			prev = curr->prev;
			prev->next = next;
			if (next != NULL)
				next->prev = prev;
		}
		curr = curr->next;
	}
}

void	ft_del_parse_redir_token_in_scmd(t_list **alst, \
								size_t index, t_list **lst_token, t_cmd *cmd)
{
	t_list			*curr;
	t_settings_del	*del;

	curr = *alst;
	if (*alst == NULL)
		return ;
	del = ft_struct_init_settings_del(&del);
	del->index = index;
	curr = *alst;
	while (curr != NULL && curr->next != NULL && (curr->title == redir_out || \
				curr->title == redir_append || curr->title == redir_in || \
					curr->title == redir_heredoc || curr->title == redir_err))
		curr = ft_head_redir_token(curr, cmd, *lst_token, del);
	if (cmd->flag_head_list == 1)
	{
		*lst_token = curr;
		cmd->flag_head_list = 0;
	}
	*alst = curr;
	while (curr != NULL \
		&& (curr->position < cmd->simplecmds[del->index]->end_simplecmd_pos) \
		&& (curr->next != NULL && curr->next->position \
					< cmd->simplecmds[del->index]->end_simplecmd_pos))
		curr = ft_middle_redir_token(curr, cmd, *lst_token, del);
	ft_free_struct_t_settings_del(&del);
}

t_list	*ft_readjust_start_lst_token(t_list *start_lst_token, \
									t_cmd *cmd, size_t i)
{
	while (start_lst_token->position < cmd->simplecmds[i]->end_simplecmd_pos)
		start_lst_token = start_lst_token->next;
	if (start_lst_token->position == cmd->simplecmds[i]->end_simplecmd_pos)
		start_lst_token = start_lst_token->next;
	return (start_lst_token);
}

t_list	*ft_lst_first(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->prev)
		lst = lst->prev;
	return (lst);
}

void	ft_flag_empty_head_list(t_cmd *cmd, t_settings_del *del)
{
	if (cmd->flag_empty_head_list == -1 && del->index == 0)
		cmd->flag_empty_head_list = 1;
}
