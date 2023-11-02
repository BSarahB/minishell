/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_data_token.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 11:46:56 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/06/21 11:47:00 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_get_token_content_lengh_for_malloc(t_list *token, \
								size_t start_token_pos, size_t end_token_pos)
{
	size_t	size_content;

	size_content = end_token_pos - start_token_pos;
	token->content = ft_init_cstring(&((token)->content), \
													size_content + 1, '\0');
}

void	ft_get_token_content(t_data *data, \
					size_t start_token_pos, size_t end_token_pos, char *line)
{
	t_list	*new;

	if (data->token->quoting_rule_adequate == 0 \
									&& data->token->quoting_rule != 0)
	{
		printf("CAUTION bash error : quoting rule inadequate");
		printf(" \" or \' incomplete, quoting must be CLOSED\n");
	}
	ft_get_token_content_lengh_for_malloc(data->token, \
										start_token_pos, end_token_pos);
	data->token->content = ft_memcpy(data->token->content, \
				&line[start_token_pos], end_token_pos - start_token_pos + 1);
	new = ft_lstnew_for_lst(data);
	ft_lstadd_back(&(data->lst_token), new);
	data->token->quoting_rule = 0;
	data->token->quoting_rule_adequate = 0;
	data->token->title = -1;
}

int	ft_is_char_operand(char *str, t_list *lst_token)
{
	(void)lst_token;
	if (*str == '>')
		return (GREAT);
	if (*str == '<')
		return (LESS);
	if ((*str == '>') && (*(str - 1) == '>'))
		return (GREATGREAT);
	if (*str == '|')
		return (PIPE);
	return (0);
}
