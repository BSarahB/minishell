/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_lexer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 15:55:07 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/03/07 15:55:30 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_char_or_token_is_unique(char *line, size_t i, t_data *data)
{
	if (data->token->end_token_pos == 0 && data->token->start_token_pos == 0 \
								&& data->token->start_token_pos_exists == 1)
	{
		ft_get_token_content(data, data->token->start_token_pos, i, line);
		data->token->start_token_pos_exists = 0;
		data->token->end_token_pos = 0;
	}
}

int	ft_tokenize_aux(t_data *data, char *str, char *line, size_t i)
{
	if ((data->token->quoting_rule == whitespace_separator) \
		&& (data->token->start_token_pos_exists == 0) \
		&& (!(ft_get_token_type(&str[i], data, i, line))) \
		&& (!(str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))))
	{
		data->token->start_token_pos = i;
		data->token->start_token_pos_exists = 1;
	}
	ft_get_token_quoting_rule(str, data->token, i);
	if (ft_get_token_type(&str[i], data, i, line))
		i = ft_char_is_operator(line, i, data);
	if (data->token->end_token_pos != 0)
	{
		ft_get_token_content(data, data->token->start_token_pos, \
											data->token->end_token_pos, line);
		data->token->start_token_pos_exists = 0;
		data->token->end_token_pos = 0;
	}
	return (i);
}

size_t	ft_tokenize(char *str, char *line, size_t i, t_data *data)
{
	while (str[i])
	{
		i = ft_char_is_whitespace(str, line, i, data);
		if (str[i] == 0)
		{
			i = ft_char_isnull_no_qr(line, i, data);
			break ;
		}
		i = ft_tokenize_aux(data, str, line, i);
		i++;
	}
	return (i);
}

t_data	*ft_trim_and_clear(char *line, t_data *data)
{
	char	*str;
	size_t	i;

	str = line;
	i = 0;
	i = ft_tokenize(str, line, i, data);
	ft_char_or_token_is_unique(line, i, data);
	if (data->token->end_token_pos != 0 \
								&& data->token->start_token_pos_exists == 1)
	{
		ft_get_token_content(data, data->token->start_token_pos, \
											data->token->end_token_pos, line);
		data->token->start_token_pos_exists = 0;
		data->token->end_token_pos = 0;
	}
	return (data);
}

t_data	*ft_tokenize_line(char *line)
{
	t_data	*data;

	data = ft_struct_init_data(&data);
	data = ft_trim_and_clear(line, data);
	return (data);
}
