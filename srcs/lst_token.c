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

t_list	*ft_lstnew_for_lst_retokenized(t_list *start_lst_token_retokenized)
{
	t_list	*new;

	new = malloc(sizeof (t_list));
	if (!new)
		return (NULL);
	new->content = ft_strdup(start_lst_token_retokenized->content);
	new->title = -1;
	new->position = 0;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

t_list	*ft_lstnew_for_lst2(t_data *data)
{
	t_list	*new;

	new = malloc(sizeof (t_list));
	if (!new)
		return (NULL);
	new->content = ft_dequote(data->token->content);
	new->type = data->token->type;
	new->title = data->token->title;
	new->quoting_rule = data->token->quoting_rule;
	new->position = data->token->position;
	new->tag_ambigeous = data->token->tag_ambigeous;
	new->tag_empty_cmd_before_dq = data->token->tag_empty_cmd_before_dq;
	new->tag_empty_cmd_after_dq = data->token->tag_empty_cmd_after_dq;
	new->expand_exists = data->token->expand_exists;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

t_list	*ft_lstnew_for_lst(t_data *data)
{
	t_list	*new;

	new = malloc(sizeof (t_list));
	if (!new)
		return (NULL);
	new->content = data->token->content;
	new->type = data->token->type;
	new->title = data->token->title;
	new->quoting_rule = data->token->quoting_rule;
	new->retokenize_allowed = 0;
	new->tag_ambigeous = data->token->tag_ambigeous;
	new->tag_empty_cmd_before_dq = data->token->tag_empty_cmd_before_dq;
	new->tag_empty_cmd_after_dq = data->token->tag_empty_cmd_after_dq;
	new->expand_exists = data->token->expand_exists;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

t_list	*ft_lstnew_data_token(char *content)
{
	t_list	*list;

	list = malloc(sizeof (t_list));
	if (!list)
		return (NULL);
	list->content = content;
	list->position = 0;
	list->end_token_pos = 0;
	list->start_token_pos = 0;
	list->start_token_pos_exists = 0;
	list->type = 0;
	list->tag_ambigeous = 0;
	list->title = -1;
	list->quoting_rule = 0;
	list->quoting_rule_adequate = 0;
	list->expand_exists = 0;
	list->retokenize_allowed = 0;
	list->tag_empty_cmd_before_dq = 0;
	list->tag_empty_cmd_after_dq = 0;
	list->next = NULL;
	list->prev = NULL;
	return (list);
}

t_list	*ft_create_list(void)
{
	t_list	*alst;

	alst = NULL;
	return (alst);
}
