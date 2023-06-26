/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_init_lexer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 11:45:22 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/06/21 11:45:52 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_data	*ft_struct_init_data2(t_data **data, t_list *lst_token, t_list *token)
{

	(*data)->token = token;
	(*data)->lst_token = lst_token;
	return(*data);
}

t_data	*ft_struct_init_data(t_data **data)
{
	char	*token_content;
	t_list	*token;
	t_list	*lst_token;


	*data = (t_data *)malloc(sizeof(t_data));
	if (!(*data))
		return (NULL);

	token_content = NULL;
	token = ft_lstnew_data_token(token_content);
	lst_token = ft_create_list();
	(*data)->token = token;
	(*data)->lst_token = lst_token;
	return(*data);
}
