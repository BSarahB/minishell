/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_validity_of_line_input2.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 18:44:54 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/29 18:45:12 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ft_strcmp(char *s1, char *s2)
{
	int i;

	i = 0;
	while(s1[i] != '\0' && s2[i] != '\0')
	{
		if(s1[i] != s2[i])
			return	(s1[i] - s2[i]);
		i++;
	}
	return (s1[i] - s2[i]);
}

int		ft_is_check_for_token(t_list *lst_token, char *str)
{
	t_list *tmp;

	tmp = lst_token;
	while(tmp)
	{
		
		if(ft_strcmp(tmp->content, str) == 0)
			return (1);
		tmp = tmp->next;
	}
	return(0);
}

int 	ft_exclamation(t_list *tmp)
{
	if((ft_strcmp(tmp->content, "!") == 0))
	{
		if(tmp->next == NULL)
			return(1);
	}
	return(0);
}

int ft_ampersand(t_list *tmp)
{
	if(ft_strcmp(tmp->content, "&") == 0)
	{
		if(tmp->next == NULL)
		{
			ft_error_msg2b(tmp->content);
			return(2);
		}
		if((ft_strcmp(tmp->next->content, "&") == 0))
		{
			ft_error_msg2c(tmp->content);
			return(2);
		}
	}
	if(ft_strcmp(tmp->content, "&&") == 0)
	{
		if(tmp->next == NULL)
		{
			ft_error_msg2b(tmp->content);
			return(2);
		}
		if((ft_strcmp(tmp->next->content, "&") == 0))
		{
			ft_error_msg2c(tmp->content);
			return(2);
		}
	}
	return(0);
}