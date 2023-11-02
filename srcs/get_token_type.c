/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_token_type.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 18:07:45 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/09/04 18:08:06 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	ft_get_token_type_pipe(t_list *token, t_data *data, size_t i, char *line)
{
	if (data->token->start_token_pos_exists != 0)
	{
		data->token->end_token_pos = i - 1;
		ft_get_token_content(data, data->token->start_token_pos, \
											data->token->end_token_pos, line);
		data->token->start_token_pos_exists = i;
		data->token->end_token_pos = i;
	}
	token->title = operator;
	return (PIPE);
}

int	ft_get_token_type_less_or_lessless(char *str, t_data *data, \
														size_t i, char *line)
{
	if ((*(str + 1)) != 0 && data->token->start_token_pos_exists == 0)
	{
		if ((*str == '<') && (*(str + 1) == '<'))
		{
			data->token->title = redir_heredoc;
			data->token->type = 7;
			return (LESSLESS);
		}
	}
	if (data->token->start_token_pos_exists != 0)
	{
		data->token->end_token_pos = i - 1;
		ft_get_token_content(data, data->token->start_token_pos, \
											data->token->end_token_pos, line);
		data->token->start_token_pos_exists = i;
		data->token->end_token_pos = i;
		if ((*str == '<') && (*(str + 1) == '<'))
		{
			data->token->title = redir_heredoc;
			data->token->type = 7;
			return (LESSLESS);
		}
	}
	data->token->title = redir_in;
	return (LESS);
}

int	ft_get_token_type_great_or_greatgreat(char *str, \
											t_data *data, size_t i, char *line)
{
	if (*(str + 1) && data->token->start_token_pos_exists == 0)
	{
		if ((*str == '>') && (*(str + 1) == '>'))
		{
			data->token->title = redir_append;
			data->token->type = 5;
			return (GREATGREAT);
		}
	}
	if (data->token->start_token_pos_exists != 0)
	{
		data->token->end_token_pos = i - 1;
		ft_get_token_content(data, data->token->start_token_pos, \
											data->token->end_token_pos, line);
		data->token->start_token_pos_exists = i;
		data->token->end_token_pos = i;
		if ((*str == '>') && (*(str + 1) == '>'))
		{
			data->token->title = redir_append;
			data->token->type = 5;
			return (GREATGREAT);
		}
	}
	data->token->title = redir_out;
	return (GREAT);
}

int	ft_get_token_type(char *str, t_data *data, size_t i, char *line)
{
	if (data->token->quoting_rule != single_quote \
								&& data->token->quoting_rule != double_quote)
	{
		if (*str == '>')
			return (ft_get_token_type_great_or_greatgreat(str, data, i, line));
		if (*str == '<')
			return (ft_get_token_type_less_or_lessless(str, data, i, line));
		if (*str == '|')
			return (ft_get_token_type_pipe(data->token, data, i, line));
	}
	return (0);
}
