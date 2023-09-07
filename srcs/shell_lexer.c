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

size_t		ft_char_is_operator(char *line, size_t i, t_data *data)
{
	if(data->token->start_token_pos_exists == 0)//on n est pas colles a un token
		i = ft_operator_not_glued_on_other_token(line, i, data);
	else if(data->token->start_token_pos_exists != 0)//on est colles a un token
		i = ft_operator_glued_on_other_token(line, i, data);
	return(i);
}

void	ft_char_or_token_is_unique(char *line, size_t i, t_data *data)//char or token is unique // faudrait  il recuperer ici la end pos puisqu on sort du code des token et qu on a atteint un \0 ?
{
	if (data->token->end_token_pos == 0 && data->token->start_token_pos == 0 && data->token->start_token_pos_exists == 1)//todo verif si elimination
			{
				ft_get_token_content(data, data->token->start_token_pos, i, line);//on ne remet pas a 0 les compteurs start et end
				data->token->start_token_pos_exists = 0;
				data->token->end_token_pos = 0;
			}
}

size_t	ft_tokenize(char *str, char *line, size_t i, t_data *data)
{
	while (str[i])
	{//le 1 er caractere est un ESPACE
	 //le caractere est un ESPACE
		i = ft_char_is_whitespace(str,line, i, data);
		// on est soit sur le 1 ere caractere alphanumerique
		//soit on est issu de la while qui boucle sur les espaces en ayant break par ex ou du i++ general
		//soit sur un caractere qui suit et qui  n est pas un espace puisquil n a pas ete repris dans la boucle while precedente -> char  ou \0
		if (str[i] == 0)
		{
			ft_char_isnull_no_qr(line, i, data);
			break;
		}
		//cas dun char alphanum startant le token
		if ((data->token->quoting_rule == whitespace_separator) && (data->token->start_token_pos_exists == 0) && (!(ft_get_token_type(&str[i], data->token, data, i, line))) && (!(str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))))
		{
			data->token->start_token_pos = i;
			data->token->start_token_pos_exists = 1;
		}
		ft_get_token_quoting_rule(str, data->token, i);
		if (ft_get_token_type(&str[i], data->token, data, i, line)) //si on est sur un operator
			i = ft_char_is_operator(line, i, data);
//on a un token abouti ici donc on peut l imprimer. il vient soit de la ft get token quoting rule car le car qui suit est un \0 indiquant la fin du token par ex
		if (data->token->end_token_pos != 0)//ic i on a imprime l <l> , ou ""^  ou ls>>outfile pour tokeniser outfile
		{
			ft_get_token_content(data, data->token->start_token_pos, data->token->end_token_pos, line);//on ne remet pas a 0 les compteurs start et end?
			data->token->start_token_pos_exists = 0;
			data->token->end_token_pos = 0;
		}
	i++;
	}
	return (i);
}

t_data	*ft_trim_and_clear(char *line, t_data *data)
{
	char *str;
	size_t i;

	str = line;
	i = 0;
	i = ft_tokenize(str, line, i, data);
	ft_char_or_token_is_unique(line, i, data);
	if (data->token->end_token_pos != 0 && data->token->start_token_pos_exists == 1)
	{
		ft_get_token_content(data, data->token->start_token_pos,data->token->end_token_pos, line);//on ne remet pas a 0 les compteurs start et end?
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
	return(data);
}
