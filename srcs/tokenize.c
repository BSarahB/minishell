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

size_t	ft_char_after_ws_is_operator_no_token_exists_no_qr(size_t i, char *line, t_data *data)
{

	if(data->token->start_token_pos_exists == 0)//l operateur n est pas colle a un token deja existant ex: ls  >outfile
	{
		data->token->start_token_pos = i;
		data->token->end_token_pos = i;
		//TODO
		if(data->token->title == redir_append)
			{
				data->token->end_token_pos = i + 1;
				ft_get_token_content(data,i, data->token->end_token_pos, line);//ou i + 1
				i++;
			}
		else
			ft_get_token_content(data,i, i, line);//TODO creer le cas de l operateur a 2 caracteres << >> &> etc et faire le saut de i++ correspondant
		data->token->start_token_pos_exists = 0;
		data->token->end_token_pos = 0;
		i++;
	}
	return(i);
}

void	ft_char_after_ws_isnull_token_exists_noqr(char *line, size_t i,t_data *data)
{
	data->token->end_token_pos = i - 2; // si pas d espace, i -2 si un esapce avant
	//if (lst_token->end_token_pos != 0)
		ft_get_token_content(data, data->token->start_token_pos, data->token->end_token_pos, line);

	data->token->start_token_pos_exists = 0;
	data->token->end_token_pos = 0;
	//lst_token->tokenized = 1;
}

void	ft_char_after_ws_token_exist_no_qr(char *line, size_t i, t_data *data)
{
	if ((data->token->quoting_rule == whitespace_separator || data->token->quoting_rule_adequate == 1) && (data->token->start_token_pos_exists == 1))
	{
		data->token->end_token_pos = i - 2; //on definit l index end du token existant.

		//	if (lst_token->end_token_pos != 0)
				ft_get_token_content(data, data->token->start_token_pos, data->token->end_token_pos, line);//on imprime le token
		// i++; //pas d incrementation car on vient d imprimer le token existant et l index sur lequel nous nous trouvons actuellement est le depart du nouveau token, nous devrons donc determiner sil s agit dun pipe ou d une char classique  ou d un espace egalement
		data->token->start_token_pos_exists = 0; // on reinitialise tout a 0 car on ne sait pas si le depart du nouveau token demarre ici, nous pourrions tout aussi bien etre sur un espace, auquel cad, nous allons donc refaire la boucle et avancer
		data->token->end_token_pos = 0;
	}
}

int		ft_char_isnull_no_qr(char *line, size_t i, t_data *data)
{
	if ((data->token->quoting_rule == whitespace_separator) && (data->token->start_token_pos_exists == 0))
		return(0);
	if ((data->token->quoting_rule == whitespace_separator) && (data->token->start_token_pos_exists == 1))
	{
		data->token->end_token_pos = i - 1; // si pas d espace, i -2 si un esapce avant

		if (data->token->end_token_pos != 0)
			ft_get_token_content(data, data->token->start_token_pos, data->token->end_token_pos, line);

		data->token->start_token_pos_exists = 0;
		data->token->end_token_pos = 0;
	}
	if ((data->token->quoting_rule == double_quote || single_quote) && (data->token->start_token_pos_exists == 1))
	{
		data->token->end_token_pos = i - 1; // si pas d espace, i -2 si un esapce avant

		if (data->token->end_token_pos != 0)
			ft_get_token_content(data, data->token->start_token_pos, data->token->end_token_pos, line);

		data->token->start_token_pos_exists = 0;
		data->token->end_token_pos = 0;
	}
	return(0);
}

size_t ft_char_is_whitespace(char *str, char *line, size_t i, t_data *data)
{
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
	{
		i++;
		//le char exists
		//ft_char_after_ws_exists()
		if (str[i])
		{
			//1)char after white space no token exists yet no qr no space
			//on est sur  1 caractere qui suit un espace dans un TOKEN QUI NEXISTE PAS ENCORE DONC NEUTRE(PAS DE QUOTING) et on n est pas colles a un token qui a deja un depart ,
			//ft_char_after_ws_is_no_ws_in_notoken_exist_noqr() NO TOKEN EXISTING_no_QR char_is_NOSPACe

			//ft_char_after_ws_is_no_ws_token_noexist_no_qr(line, i, lst_token);
			if ((data->token->quoting_rule == whitespace_separator) && (data->token->start_token_pos_exists == 0) && (!(str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))))
			{//char_after_ws_is_operator_no_token_exists_no_qr
				if (ft_get_token_type(&str[i], data->token, data, i, line)) // si le char est un operateur (| < > etc &) on le considere DIRECT Comme un TOKEN et on l imprime. //TODO creer le cas de l operateur a 2 caracteres << >> &> etc
					i = ft_char_after_ws_is_operator_no_token_exists_no_qr(i, line, data);
				else{ // on est sur un char non operateur qui suit un espace (no token no qr)
					data->token->start_token_pos = i;//
					data->token->start_token_pos_exists = 1;
					break;
					}
			}
			//char after white space no QR, 1 TOKEN EXISTS (un token a deja start)
			ft_char_after_ws_token_exist_no_qr(line, i, data);
		}
		//le char est \0 apres l espace
		//char after espace qui est un \0 avec 1 TOKEN EXISTANT ls \0 pas de QR -> l espace precedent signifie donc la fin d un token
		if ((str[i] == 0) && (data->token->quoting_rule == whitespace_separator || data->token->quoting_rule_adequate == 1) && (data->token->start_token_pos_exists == 1))
		{
			ft_char_after_ws_isnull_token_exists_noqr(line, i, data);
			break;
		}
	}
	return(i);
}