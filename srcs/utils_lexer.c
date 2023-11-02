/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lexer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 12:55:01 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/09/07 12:55:15 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_operator_glued_on_other_token(char *line, size_t i, t_data *data)
{
	if (data->token->title == redir_append \
		|| data->token->title == redir_heredoc)
	{
		data->token->end_token_pos = i + 1;
		ft_get_token_content(data, i, data->token->end_token_pos, line);
		i++;
	}
	else
		ft_get_token_content(data, i, i, line);
	data->token->start_token_pos_exists = 0;
	data->token->end_token_pos = 0;
	return (i);
}

size_t	ft_operator_not_glued_on_other_token(char *line, size_t i, t_data *data)
{
	data->token->start_token_pos = i;
	data->token->end_token_pos = i;
	if (data->token->title == redir_append \
		|| data->token->title == redir_heredoc)
	{
		data->token->end_token_pos = i + 1;
		ft_get_token_content(data, i, data->token->end_token_pos, line);
		i++;
	}
	else
		ft_get_token_content(data, i, i, line);
	data->token->start_token_pos_exists = 0;
	data->token->end_token_pos = 0;
	return (i);
}
