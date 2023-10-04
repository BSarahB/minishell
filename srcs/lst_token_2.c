/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_token_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 13:45:12 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/09/04 13:45:23 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	ft_aff_listenv_ptr_sur_char_content(t_listenvp *alst) // pour void		*content; de type char *
{
			printf("list diplayed: \n");
	while(alst)
	{
		printf("<%s>\n",(char *)(alst)->key_value); //ou printf("%d - ", (*(int *)(*alst)->content));
		//printf("%s , %zu\n",(char *)(alst)->content, alst->position); //ou printf("%d - ", (*(int *)(*alst)->content));
		alst = (alst)->next;
	}
	printf("end_list dipslay\n");
}

void	ft_aff_list_ptr_sur_char_content3(t_list *alst) // pour void		*content; de type char *
{
			printf("RETOKENIZED ENTIRE list diplayed: \n");
	while(alst)
	{
		printf("[%s]\n",(char *)(alst)->content); //ou printf("%d - ", (*(int *)(*alst)->content));
		//printf("%s , %zu\n",(char *)(alst)->content, alst->position); //ou printf("%d - ", (*(int *)(*alst)->content));
		alst = (alst)->next;
	}
	printf("RETOKENIZED ETNIRE end_list dipslay\n");
}


void	ft_aff_list_ptr_sur_char_content2(t_list *alst) // pour void		*content; de type char *
{
			printf("RETOKENIZED list diplayed: \n");
	while(alst)
	{
		printf("[%s]\n",(char *)(alst)->content); //ou printf("%d - ", (*(int *)(*alst)->content));
		//printf("%s , %zu\n",(char *)(alst)->content, alst->position); //ou printf("%d - ", (*(int *)(*alst)->content));
		alst = (alst)->next;
	}
	printf("RETOKENIZED end_list dipslay\n");
}

void	ft_aff_list_ptr_sur_char_content(t_list *alst) // pour void		*content; de type char *
{
			printf("list diplayed: \n");
	while(alst)
	{
		printf("<%s>\n",(char *)(alst)->content); //ou printf("%d - ", (*(int *)(*alst)->content));
		//printf("%s , %zu\n",(char *)(alst)->content, alst->position); //ou printf("%d - ", (*(int *)(*alst)->content));
		alst = (alst)->next;
	}
	printf("end_list dipslay\n");
}

void	ft_simplify_list(t_list *lst)
{
	t_list	*tmp;
	size_t	position;

	tmp = lst;
	position = 1;

	while (tmp)
	{
		tmp->position = position;
		tmp = tmp->next;
		position++;
	}
}