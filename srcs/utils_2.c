/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 09:37:46 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/09/06 09:38:02 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strcpy(char *dst, char const *src)
{
	int	i;

	i = 0;
	while (src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

char	*ft_init_string(size_t len)
{
	char	*str;
	size_t	i;

	i = 0;
	str = (char *)malloc((len + 1) * sizeof(char));
	if (!str)
		return (NULL);
	if (len > 0)
	{
		while (i < len)
			str[i++] = 0;
	}
	(str)[len] = '\0';
	return (str);
}

char	*ft_strcat(char *dest, char const *src)
{
	int	i;
	int	l;

	i = 0;
	l = 0;
	while (dest[l] != '\0')
		l++;
	while (src[i] != '\0')
	{
		dest[l + i] = src[i];
		i++;
	}
	dest[l + i] = '\0';
	return (dest);
}

char	*ft_strjoin(char *s1, char const *s2)
{
	char	*str;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (s1[i])
		i++;
	while (s2[j])
		j++;
	str = ft_init_string(i + j);
	if (!s1 || !s2 || !str)
		return (NULL);
	str = ft_strcat(ft_strcpy(str, s1), s2);
	return (str);
}

char	*ft_strdup(const char *s)
{
	char	*str;
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	str = (char *)malloc((i + 1) * sizeof (char));
	if (!str)
		return (NULL);
	ft_strcpy(str, s);
	return (str);
}
