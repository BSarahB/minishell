/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 13:47:12 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/09/04 13:47:15 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list *ft_lstlast(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void ft_lstadd_back(t_list **alst, t_list *new)
{
	t_list *tmp;

	if (new)
		{
			new->next = NULL;
			new->prev = NULL;
		}
	if (!(*alst))
		*alst = new;
	else
		{
			tmp = ft_lstlast(*alst);
			ft_lstlast(*alst)->next = new;
			new->prev = tmp;
		}
}

t_list *ft_lstnew_for_lst(t_data *data)
{
	t_list *new;

	new = malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->content = data->token->content;
	new->type = data->token->type;
	new->title = data->token->title;
	new->quoting_rule = data->token->quoting_rule;
//	new->quoting_rule_adequate = 0;
//	new->retokenize_allowed = 0;
	new->tag_expand = data->token->tag_expand;
	new->expand_exists = data->token->expand_exists;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

t_list *ft_lstnew_data_token(char *content)
{
	t_list *list;

	list = malloc(sizeof(t_list));
	if (!list)
		return (NULL);
	list->content = content;
	list->position = 0;
	list->end_token_pos = 0;
	list->start_token_pos = 0;
	list->start_token_pos_exists = 0;
	list->type = 0;
	list->tag_expand = 0;
	list->title = -1;
	list->quoting_rule = 0; 
	list->quoting_rule_adequate = 0;
	list->expand_exists = 0;
	list->retokenize_allowed = 0;
	list->next = NULL;
	list->prev = NULL;
	return (list);
}

t_list *ft_create_list(void)
{
	t_list *alst;

	alst = NULL;
	return (alst);
}
