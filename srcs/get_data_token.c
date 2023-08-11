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
	token->content = ft_init_cstring(&((token)->content), size_content + 1, 0);
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

//si token nul -> on ne le rajoute pas a la liste
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
					   {
						   return (GREAT);
					   }
					   if (*str == '<')
					   {
						   return (LESS);
					   }
					   if ((*str == '>') && (*(str - 1) == '>')) // mettre str pour checker l element precedent TODO : proteger str d un index qui n exste pas
					   {										  // TODO risque de segfault a str index 0
						   return (GREATGREAT);
					   }
					   //LESSLESS << pour les heredocs : faire plus de recherches sur les heredocs
					   if ((*str == '&') && (*(str - 1) == '>')) // proteger egalement d un ouot of range se proteger de segfault en mettant la condition d existence
					   {
						   return (GREAT_AND_AMPERSAND);
					   }
					   if (*str == '|')
					   {
						   return (PIPE);
						   //		<|>,<|>,<|>   je me souviens plus pkoi j ai mis ceux la....CQFD
					   }
					   if (*str == '&')
					   {
						   return (AMPERSAND);
					   }
					   return (0);
}

int ft_get_token_type(char *str, t_list *token, t_data *data, size_t i, char *line)
{
	if ((*str == '>') && (*(str - 1) == '>'))
	{										  // TODO risque de segfault a str index 0
		return (GREATGREAT);
	}
	//LESSLESS << pour les heredocs : faire plus de recherches sur les heredocs
	if ((*str == '&') && (*(str - 1) == '>')) // proteger egalement d un ouot of range se proteger de segfault en mettant la condition d existence
	{
		return (GREAT_AND_AMPERSAND);
	}
	if (token->quoting_rule != single_quote && token->quoting_rule != double_quote)
	{
		if (*str == '>')
		{//si loperateur est colle au token ls>outfile
			if(data->token->start_token_pos_exists != 0)//on est colles a un token
			{
				data->token->end_token_pos = i - 1;
				ft_get_token_content(data, data->token->start_token_pos, data->token->end_token_pos, line);
				data->token->start_token_pos_exists = i;
				data->token->end_token_pos = i;
			}
			token->title = redir_out;
			return (GREAT);
		}
		if (*str == '<')
		{
			//si loperateur est colle au token ls|grep c
			if((*(str + 1)) != 0)
			{
				if ((*str == '<') && (*(str + 1) == '<'))
					{										
						return (LESSLESS);
					}
			}
			if(data->token->start_token_pos_exists != 0)//on est colles a un token
			{
				data->token->end_token_pos = i - 1;
				ft_get_token_content(data, data->token->start_token_pos, data->token->end_token_pos, line);
				data->token->start_token_pos_exists = i;
				data->token->end_token_pos = i;
			}
			token->title = redir_in;
			return (LESS);
		}
		if (*str == '|')
		{
			//si loperateur est colle au token ls|grep c
			if(data->token->start_token_pos_exists != 0)//on est colles a un token
			{
				data->token->end_token_pos = i - 1;
				ft_get_token_content(data, data->token->start_token_pos, data->token->end_token_pos, line);
				data->token->start_token_pos_exists = i;
				data->token->end_token_pos = i;
			}
			token->title = operator;
			return (PIPE);
			//		<|>,<|>,<|>   je me souviens plus pkoi j ai mis ceux la....CQFD
		}
		if (*str == '&')
		{
			return (AMPERSAND);
		}
	}
	return (0);
}

void ft_get_token_quoting_rule(char *str, t_list *lst_token, size_t i)
{
	// cette fonction va permettre de determiner quelle est la regle de quoting : double quoting single quoting ou whitespace_separator
	//  decrite ici separemment pour plus de visibilite mais sera intergree normalement et fondue dans la trim and clear
	// mettre un INTERRUPTEUR ICI qui definit la regle du quoting rule  whitespace_Separator 0 single quoting 1 double quoting2 par ex il faudrait
	// NB c est le premier quoting rule rencontre qui l emporte cf echo "'$VAR'" ou "'$VAR'"
	char c;

	c = str[i];
	if (lst_token->quoting_rule == 0 && c == '\"' && str[i + 1] != '\0')
		lst_token->quoting_rule = 2;
	else if (c == '\"' && lst_token->quoting_rule == 2)
	{
		lst_token->quoting_rule = 2;
		lst_token->quoting_rule_adequate = 1;
		if(str[i + 1] == '\0' || (str[i + 1] == ' ' || (str[i + 1] >= 9 && str[i + 1] <= 13)) || (ft_is_char_operand(&str[i + 1], lst_token) >= 3))
			{
				lst_token->end_token_pos = i;
			}
	}
	else if (lst_token->quoting_rule == 0 && c == '\'' && str[i + 1] != '\0')
		lst_token->quoting_rule = 1;
	else if (c == '\'' && lst_token->quoting_rule == 1)
	{
		lst_token->quoting_rule = 1;
		lst_token->quoting_rule_adequate = 1;
		if(str[i + 1] == '\0' || (str[i + 1] == ' ' || (str[i + 1] >= 9 && str[i + 1] <= 13)) || (ft_is_char_operand(&str[i + 1], lst_token) >= 3))
		{
			lst_token->end_token_pos = i;
		}
	}

	else if (lst_token->quoting_rule == 0 && str[i + 1] == '\0') // c est le cas de $> l[s]    ->[s] est checke dans la ft_get_token_quoting rule on verifie si la quoting rule  == 0 et que lindex suivant est un \0 alors cela signifie qu on a la fin d un token
	{
		lst_token->end_token_pos = i;
		if(c == '\"')
			lst_token->quoting_rule = 2;
		if(c == '\'')
			lst_token->quoting_rule = 1;
	}
	else if (lst_token->quoting_rule == 2 && str[i + 1] == '\0') // c est le cas de $> l[s]    ->[s] est checke dans la ft_get_token_quoting rule on verifie si la quoting rule  == 0 et que lindex suivant est un \0 alors cela signifie qu on a la fin d un token
	{
		lst_token->end_token_pos = i;
	}
}
//TODO : lexer << les heredocs
//TODO : comparer quand cat est en deniere simple command cat outfile22 ne marche pas .
//build-my_minishell-Desktop_GCC-Debug ls | wc -l | cat outfile22
//19
// build-my_minishell-Desktop_GCC-Debug ls | wc -l>outfile22 | cat outfile22
//pas de resultat on a le prompt directement
