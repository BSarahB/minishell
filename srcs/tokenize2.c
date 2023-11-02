/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 11:44:37 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/06/21 11:44:44 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_reset_positions2(t_data *data, size_t i)
{
	data->token->start_token_pos = i;
	data->token->start_token_pos_exists = 1;
}

void	ft_reset_positions(t_data *data)
{
	data->token->start_token_pos_exists = 0;
	data->token->end_token_pos = 0;
}

size_t	ft_char_after_ws_is_ope_notokn_noqr(size_t i, char *line, t_data *data)
{
	if (data->token->start_token_pos_exists == 0)
	{
		data->token->start_token_pos = i;
		data->token->end_token_pos = i;
		if (data->token->title == redir_append || \
										data->token->title == redir_heredoc)
		{
			data->token->end_token_pos = i + 1;
			ft_get_token_content(data, i, data->token->end_token_pos, line);
			i++;
		}
		else
			ft_get_token_content(data, i, i, line);
		ft_reset_positions(data);
		i++;
	}
	return (i);
}
