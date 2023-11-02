/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 23:16:28 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/29 23:16:43 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_list	*ft_lstfind(t_list *alst, int position)
{
	t_list	*tmp;

	tmp = alst;
	while (tmp != NULL)
	{
		if (tmp->position == (size_t)position)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*tab;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!s)
		return (NULL);
	tab = (char *)malloc(sizeof(char) * (len + 1));
	if (!tab)
		return (NULL);
	while (s[j])
	{
		if (start <= j && i < len)
		{
			tab[i] = s[j];
			i++;
		}
		j++;
	}
	tab[i] = '\0';
	return (tab);
}

int	ft_get_token_content2(t_data *data, int start_token_pos, \
int end_token_pos, char *line)
{
	t_list	*new;

	ft_get_token_content_lengh_for_malloc(data->token, \
start_token_pos, end_token_pos);
	data->token->content = ft_memcpy(data->token->content, \
	&line[start_token_pos], (size_t)(end_token_pos - start_token_pos + 1));
	new = ft_lstnew_for_lst2(data);
	ft_lstadd_back(&(data->lst_token), new);
	data->token->quoting_rule = 0;
	data->token->quoting_rule_adequate = 0;
	data->token->title = -1;
	start_token_pos = -1;
	return (start_token_pos);
}
