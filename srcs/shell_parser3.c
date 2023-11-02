/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_parser3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 15:55:07 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/03/07 15:55:30 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_settings_del	*ft_del_empty_in_middle(t_list *curr, t_cmd *cmd, \
								t_list *lst_token, t_settings_del *del)
{
	t_list	*lst_token_to_remove;

	(void)lst_token;
	lst_token_to_remove = curr->next;
	curr->next = curr->next->next;
	if (curr->next == NULL)
		cmd->simplecmds[del->index]->end_simplecmd_pos = curr->position;
	ft_lstdelone_beta(&lst_token_to_remove);
	del->i++;
	return (del);
}

t_list	*ft_middle_empty_token(t_list *curr, t_cmd *cmd, t_list *lst_token, \
							t_settings_del *del)
{
	if (curr->next == NULL)
		curr = curr->next;
	else if (curr->next->tag_empty_cmd_before_dq == 1)
	{
		del = ft_del_empty_in_middle(curr, cmd, lst_token, del);
	}
	else
		curr = curr->next;
	return (curr);
}

t_settings_del	*ft_del_empty_in_head(t_list *curr, t_cmd *cmd, \
									t_list *lst_token, t_settings_del *del)
{
	t_list	*lst_token_to_remove;

	(void)cmd;
	lst_token_to_remove = curr;
	if (del->index != 0)
	{
		ft_reconnect_lst_token(lst_token, lst_token_to_remove->position);
	}
	ft_lstdelone(&lst_token_to_remove);
	del->i++;
	return (del);
}

t_list	*ft_head_empty_token(t_list *curr, t_cmd *cmd, t_list *lst_token, \
							t_settings_del *del)
{
	t_list	*next;

	next = NULL;
	ft_flag_empty_head_list(cmd, del);
	next = curr->next;
	if (next && del->index == 0)
		next->prev = NULL;
	if (curr->tag_empty_cmd_before_dq)
		del = ft_del_empty_in_head(curr, cmd, lst_token, del);
	curr = next;
	return (curr);
}
