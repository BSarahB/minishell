/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 09:38:09 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/09/06 09:38:22 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char	*s;
	unsigned char	*d;

	s = (unsigned char *)src;
	d = (unsigned char *)dst;
	if (!src && !dst)
		return (NULL);
	while (n > 0)
	{
		*d = *s;
		d++;
		s++;
		n--;
	}
	return (dst);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	if (s == NULL)
		return (0);
	while (s[i])
		i++;
	return (i);
}

void	ft_putstr_fd(char *s, int fd)
{
	if (s != 0)
	{
		while (*s)
		{
			write(fd, s, 1);
			s++;
		}
	}
}

void	ft_free_struct_str(char **p)
{
	if (*p != NULL)
	{
		free(*p);
		*p = NULL;
	}
}

char	*ft_swap_ptr(char **token_content, char *blank_node)
{
	char	*tmp;

	tmp = *token_content;
	*token_content = blank_node;
	blank_node = tmp;
	ft_free_struct_str(token_content);
	return (blank_node);
}
