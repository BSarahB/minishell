/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_lexer2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 15:55:07 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/03/07 15:55:30 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_char_is_operator(char *line, size_t i, t_data *data)
{
	if (data->token->start_token_pos_exists == 0)
		i = ft_operator_not_glued_on_other_token(line, i, data);
	else if (data->token->start_token_pos_exists != 0)
		i = ft_operator_glued_on_other_token(line, i, data);
	return (i);
}
