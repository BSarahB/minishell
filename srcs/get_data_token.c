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

void ft_get_token_content_lengh_for_malloc(t_list *token, size_t start_token_pos, size_t end_token_pos)
{
	size_t size_content;

	size_content = end_token_pos - start_token_pos;
	token->content = ft_init_cstring(&((token)->content), size_content + 1, '\0');
}

void ft_get_token_content(t_data *data, size_t start_token_pos, size_t end_token_pos, char *line)
{
	t_list *new;

	if (data->token->quoting_rule_adequate == 0 && data->token->quoting_rule != 0)
	{
		printf("CAUTION bash error : quoting r  ule inadequate \" or \' incomplete, quoting must be CLOSED\n");
	}
	ft_get_token_content_lengh_for_malloc(data->token, start_token_pos, end_token_pos);
	data->token->content = ft_memcpy(data->token->content, &line[start_token_pos], end_token_pos - start_token_pos + 1);
	new = ft_lstnew_for_lst(data);
	ft_lstadd_back(&(data->lst_token), new);
	data->token->quoting_rule = 0;
	data->token->quoting_rule_adequate = 0;
	data->token->title = -1;
}

int ft_is_char_operand(char *str, t_list *lst_token)
{
	(void)lst_token;
	if (*str == '>')
		return (GREAT);
	if (*str == '<')
		return (LESS);
	if ((*str == '>') && (*(str - 1) == '>'))
		return (GREATGREAT);
	if ((*str == '&') && (*(str - 1) == '>'))
		return (GREAT_AND_AMPERSAND);
	if (*str == '|')
		return (PIPE);
	if (*str == '&')
		return (AMPERSAND);
	return (0);
}

void ft_get_token_quoting_rule(char *str, t_list *lst_token, size_t i) //NORMEME >25 lignes
{
	char c;

	c = str[i];
	if((lst_token->quoting_rule == 1 || lst_token->quoting_rule == 2) && lst_token->quoting_rule_adequate == 1)
	{
		lst_token->quoting_rule = 0;
		lst_token->quoting_rule_adequate = 0;
	}

	if (lst_token->quoting_rule == 0 && c == '\"' && str[i + 1] != '\0')
		{
			lst_token->quoting_rule = 2;
		}

	else if (c == '\"' && lst_token->quoting_rule == 2 && lst_token->quoting_rule_adequate == 0)
	{
		lst_token->quoting_rule = 2;
		lst_token->quoting_rule_adequate = 1;
		if (str[i + 1] == '\0' || (str[i + 1] == ' ' || (str[i + 1] >= 9 && str[i + 1] <= 13)) || (ft_is_char_operand(&str[i + 1], lst_token) >= 3))
			lst_token->end_token_pos = i;
	}

	else if (lst_token->quoting_rule == 2 && c == '\"' && str[i + 1] != '\0')
		{
			lst_token->quoting_rule = 2;
			if(lst_token->quoting_rule_adequate == 1)
				lst_token->quoting_rule_adequate = 0;
		}
			
	else if (lst_token->quoting_rule == 0 && c == '\'' && str[i + 1] != '\0')
		{
			lst_token->quoting_rule = 1;
		}
	
			
	else if (c == '\'' && lst_token->quoting_rule == 1 && lst_token->quoting_rule_adequate == 0)
	{
		lst_token->quoting_rule = 1;
		lst_token->quoting_rule_adequate = 1;
		if (str[i + 1] == '\0' || (str[i + 1] == ' ' || (str[i + 1] >= 9 && str[i + 1] <= 13)) || (ft_is_char_operand(&str[i + 1], lst_token) >= 3))
			lst_token->end_token_pos = i;
	}
	else if (lst_token->quoting_rule == 1 && c == '\'' && str[i + 1] != '\0')
	{
		lst_token->quoting_rule = 2;
		if(lst_token->quoting_rule_adequate == 1)
			lst_token->quoting_rule_adequate = 0;
	}

	else if (lst_token->quoting_rule == 0 && str[i + 1] == '\0') // c est le cas de $> l[s]    ->[s] est checke dans la ft_get_token_quoting rule on verifie si la quoting rule  == 0 et que lindex suivant est un \0 alors cela signifie qu on a la fin d un token
	{
		lst_token->end_token_pos = i;
		if (c == '\"')
			lst_token->quoting_rule = 2;
		if (c == '\'')
			lst_token->quoting_rule = 1;
	}
	else if (lst_token->quoting_rule == 2 && str[i + 1] == '\0') // c est le cas de $> l[s]    ->[s] est checke dans la ft_get_token_quoting rule on verifie si la quoting rule  == 0 et que lindex suivant est un \0 alors cela signifie qu on a la fin d un token
		lst_token->end_token_pos = i;
}