/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_epur_and_trim_buffer.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 23:49:41 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/29 23:49:59 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static int	ft_isset(char *set, char c)
{
	while (*set)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	len;
	char	*set2;
	char	*s;

	if (!s1 || !set)
		return (NULL);
	set2 = (char *)set;
	s = (char *)s1;
	start = 0;
	while (s[start] && (ft_isset(set2, s[start]) == 1))
		start++;
	len = ft_strlen((char *)&s[start]);
	if (len != 0)
		while (s[start + len - 1]
			&& (ft_isset(set2, s[start + len - 1]) == 1))
			len--;
	return (ft_substr(s1, start, len));
}

void	ft_init_t_param3(t_param_epur *para)
{
	para->i = 0;
	para->j = 0;
	para->flag = 0;
	para->quoting_rule = 0;
	para->quoting_rule_adequate = 0;
}

void	ft_fill_epur_buffer(t_param_epur *para, char *new_buffer, char *buffer)
{
	if (buffer[para->i] == ' ' || buffer[para->i] == '\t')
	{
		para->flag = 1;
		if (para->quoting_rule == 2 || para->quoting_rule == 1)
		{
			new_buffer[para->j] = ' ';
			para->j++;
			para->flag = 0;
		}
	}
	if (!(buffer[para->i] == ' ' || buffer[para->i] == '\t'))
	{
		if (para->flag)
		{
			new_buffer[para->j] = ' ';
			para->j++;
		}
		para->flag = 0;
		new_buffer[para->j] = buffer[para->i];
		para->j++;
	}
	if (para->flag == 1 && buffer[para->i + 1] == '\0')
		(new_buffer)[para->j] = ' ';
}

char	*ft_epur_buffer_ws(char *buffer)
{
	t_param_epur	para;
	char			*new_buffer;

	new_buffer = ft_init_string(1096);
	ft_init_t_param3(&para);
	if (buffer[para.i] == ' ' || buffer[para.i] == '\t')
		para.i++;
	while (buffer[para.i])
	{
		para.quoting_rule_adequate = ft_get_token_quoting_rule2(buffer, \
	(size_t)para.i, &(para.quoting_rule), &(para.quoting_rule_adequate));
		ft_fill_epur_buffer(&para, new_buffer, buffer);
		para.i++;
	}
	ft_update_string(&buffer, new_buffer);
	return (new_buffer);
}
