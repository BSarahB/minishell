/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_token_3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 01:45:59 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/30 01:46:10 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_aff_list_ptr_sur_char_content3(t_list *alst)
{
	printf("RETOKENIZED ENTIRE list diplayed: \n");
	while (alst)
	{
		printf("[%s]\n", (char *)(alst)->content);
		alst = (alst)->next;
	}
	printf("RETOKENIZED ETNIRE end_list dipslay\n");
}

t_list	*ft_lstlast(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	ft_lstadd_back(t_list **alst, t_list *new)
{
	t_list	*tmp;

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

t_list	*ft_lstnew_for_lst_retokenized2(t_list *token)
{
	t_list	*new;

	new = malloc(sizeof (t_list));
	if (!new)
		return (NULL);
	new->content = ft_strdup(token->content);
	new->title = token->title;
	new->position = token->position;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

t_list	*ft_lstnew_for_lst_token_copy(t_list *tmp)
{
	t_list	*new;

	new = malloc(sizeof (t_list));
	if (!new)
		return (NULL);
	new->content = ft_dequote(ft_strdup(tmp->content));
	new->title = tmp->title;
	new->position = tmp->position;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}
