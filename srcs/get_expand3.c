/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_expand3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 23:24:31 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/29 23:24:42 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_get_expand_lengh_for_malloc(size_t start_expand_pos, \
													size_t end_expand_pos)
{
	size_t	size_content;
	char	*expand;

	size_content = end_expand_pos - start_expand_pos;
	expand = ft_init_cstring(&expand, size_content + 1, '\0');
	return (expand);
}

char	*ft_get_expand_content(size_t start_expand_pos, \
										size_t end_expand_pos, char *content)
{
	char	*expand;

	expand = ft_get_expand_lengh_for_malloc(start_expand_pos, end_expand_pos);
	expand = ft_memcpy(expand, &content[start_expand_pos], \
									end_expand_pos - start_expand_pos + 1);
	return (expand);
}

char	*ft_get_scope_expand(size_t end_expand_pos, \
				size_t start_expand_pos, char *str, int *flag_expand_here)
{
	char	*expand;

	expand = NULL;
	*flag_expand_here = 0;
	expand = ft_get_expand_content(start_expand_pos, end_expand_pos, str);
	return (expand);
}

size_t	ft_isunderscore(char *str, size_t i)
{
	size_t	i_tmp;

	i_tmp = i;
	while (str[i] && str[i] == '_')
		i++;
	if (i_tmp == i)
		return (0);
	else
		return (i -1);
}
