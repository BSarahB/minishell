/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_part3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 23:37:13 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/29 23:37:28 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

size_t	ft_skip_ws(char *str, size_t i)
{
	if (str[i] == ' ')
	{
		while (str[i] == ' ')
			i++;
	}
	else
		i++;
	return (i);
}

size_t	ft_tokenize2(char *str, char *line, size_t i, t_data *data)
{
	int	quoting_rule;
	int	q_r_a;
	int	start_pos;

	quoting_rule = 0;
	q_r_a = 0;
	while (str[i] == ' ')
		i++;
	start_pos = i;
	while (str[i])
	{
		if (start_pos == -1)
			start_pos = i;
		q_r_a = ft_get_token_quoting_rule4(str, i, &quoting_rule, &q_r_a);
		if (q_r_a == 1 && (quoting_rule == 1 || quoting_rule == 2))
			quoting_rule = 0;
		if (str[i] == ' ' && quoting_rule == 0)
			start_pos = ft_get_token_content2(data, start_pos, \
	(int)i - 1, line);
		i = ft_skip_ws(str, i);
		if (str[i] == '\0')
			ft_get_token_content2(data, start_pos, (int)i - 1, line);
	}
	return (i);
}

t_data	*ft_trim_and_clear2(char *line, t_data *data2)
{
	char	*str;
	size_t	i;

	str = line;
	i = 0;
	i = ft_tokenize2(str, line, i, data2);
	return (data2);
}

t_data	*ft_retokenize(t_list *tmp, t_data *data2)
{
	return (ft_trim_and_clear2(tmp->content, data2));
}

t_data	*ft_copy_token(t_list *tmp, t_data *data2)
{
	t_list	*new;

	new = ft_lstnew_for_lst_token_copy(tmp);
	ft_lstadd_back(&(data2->lst_token), new);
	return (data2);
}
