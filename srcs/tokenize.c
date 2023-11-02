/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 11:44:37 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/06/21 11:44:44 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_char_after_ws_isnull_token_exists_noqr(char *line, size_t i, \
											t_data *data)
{
	data->token->end_token_pos = i - 2;
	ft_get_token_content(data, data->token->start_token_pos, \
						data->token->end_token_pos, line);
	ft_reset_positions(data);
}

void	ft_char_after_ws_token_exist_no_qr(char *line, size_t i, t_data *data)
{
	if ((data->token->quoting_rule == whitespace_separator || \
		data->token->quoting_rule_adequate == 1) && \
			(data->token->start_token_pos_exists == 1))
	{
		data->token->end_token_pos = i - 2;
		ft_get_token_content(data, data->token->start_token_pos, \
									data->token->end_token_pos, line);
		ft_reset_positions(data);
	}
}

int	ft_char_isnull_no_qr(char *line, size_t i, t_data *data)
{
	if ((data->token->quoting_rule == whitespace_separator) && \
			(data->token->start_token_pos_exists == 0))
		return (0);
	if ((data->token->quoting_rule == whitespace_separator) && \
			(data->token->start_token_pos_exists == 1))
	{
		data->token->end_token_pos = i - 1;
		if (data->token->end_token_pos != 0)
			ft_get_token_content(data, data->token->start_token_pos, \
								data->token->end_token_pos, line);
		ft_reset_positions(data);
	}
	if ((data->token->quoting_rule == double_quote || single_quote) && \
			(data->token->start_token_pos_exists == 1))
	{
		data->token->end_token_pos = i - 1;
		if (data->token->end_token_pos != 0)
			ft_get_token_content(data, data->token->start_token_pos, \
								data->token->end_token_pos, line);
		ft_reset_positions(data);
	}
	return (0);
}

int	ft_char_is_ws_aux(char *str, t_data *data, char *line, size_t *i)
{
	if (ft_get_token_type(&str[*i], data, *i, line))
		*i = ft_char_after_ws_is_ope_notokn_noqr(*i, line, data);
	else
	{
		ft_reset_positions2(data, *i);
		return (1);
	}
	return (0);
}

size_t	ft_char_is_whitespace(char *str, char *line, size_t i, t_data *data)
{
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
	{
		i++;
		if (str[i])
		{
			if ((data->token->quoting_rule == whitespace_separator) && \
				(data->token->start_token_pos_exists == 0) && \
					(!(str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))))
				if (ft_char_is_ws_aux(str, data, line, &i))
					break ;
			ft_char_after_ws_token_exist_no_qr(line, i, data);
		}
		if ((str[i] == 0) && (data->token->quoting_rule == \
			whitespace_separator || data->token->quoting_rule_adequate == 1) \
				&& (data->token->start_token_pos_exists == 1))
		{
			ft_char_after_ws_isnull_token_exists_noqr(line, i, data);
			break ;
		}
	}
	return (i);
}
