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
#define mode_lexing_build 1


/*
void	ft_is_trim_and_clear_and_retokenize_token_allowed(t_list *lst_token)
{
	//ici chopper la position end_expand_pos et verifier ensuite le caractere suivant
	//on peut mettre un int dans la strcuture : int retokenize_allowed =1 ou 0;


	// SI DOUBLE QUOTING RULE: trim_and_clear_and_retokenize_token CANCELLED NOT ALLOWED car le dernier caractere perd son pouvoir special d autoriser ou de cancel
	//IL FAUT ETRE EN WS_S RULE
	//et verifier le caractere suivant la fin de la VARIABLE a expand : +-!?[]{}/=~^.,% @ # ---> OK on allow le retokenize
	//si $VAR$ $ est le caractere suivant, alors tokenize n est PAS ALLOWED
	//si le caractere cloturant la VAR est compris dans le DQ alors.... on allow PAS le retokenize SAUF si ce carcatere est un $ alors, cela ne change rien ON ALLOWE PAS le retokenize
	//return(0) ou (1) pour fr la bool. completer +tard
	//caracteres_char ou nombres --> newline vide dans le terminal
* ou | pipe ou ' ou "   --> $> mettent le prompt sur une nouvelle ligne en attente de lecture du processus

												**********
												$ -->annule le TRIM and CLear. voir si cela rajoute un espace entre les 2 tokens ou pas.
												**********
												(
												)
												>
												<  -> pour ces 4 derniers caracteres:  bash: syntax error near unexpected token `newline'(pour le >, < ou >> )  ou `('

												*****
												& --> JE NE SAIS PAS


}
*/
/*
char *ft_substitute(t_list *lst_token)
{
	char *expanded_content;
	expanded_content == NULL;

	//on va effectuer la substitution /expansion de notre VAR
	return(expanded_content);

}
*/
// dans le cas d un double quoting rule ou w_separator rule on etablit l expansion
/*
void	ft_get_token_expansion(char c, t_list *lst_token)
{
	char *expanded_content;

	ft_is_expand_here(c, lst_token);
	ft_is_expand_authorized(c, lst_token);
	ft_does_expand_exist(c, lst_token);
	//on  updatera lst_token->content en recuperant le result lst_token->content = ft_expand_and_substitute();
	ft_is_trim_and_clear_and_retokenize_token_allowed(lst_token);//on va debord verifier si le trim and clear retokenize a venir est allowed avant de faire l expansion substitution pour ne pas galerer
	expanded_content = ft_substitute(lst_token);
}
*/


/*
void	ft_get_token_function(char c,t_list *lst_token)
{
	(void)c;
	(void)lst_token;
	//enum e_function :  soit le token sera une command un operator ou metacharacter ou soit  une redirection, sinon alors sil n est pas cote tel on saura que c est un argument ou une option assujetie a la <command>
	//pour determiner sil s agit dune commande il faudra comparer les maillons precedents et suivants
	//si le <token> est precede par un maillon avec un <token> qui est deja une command, notre token ne sera pas determine avec une function command
	//GESTION du cas ou le previous token est <echo>

}
*/



void	ft_char_is_operator(char *line, size_t i, t_data *data)
{
	if(data->token->start_token_pos_exists == 0)//on n est pas colles a un token
	{
		data->token->start_token_pos = i;
		data->token->end_token_pos = i;
		ft_get_token_content(data,i, i, line);
		data->token->start_token_pos_exists = 0;
		data->token->end_token_pos = 0;
		//i++;
	}
	else if(data->token->start_token_pos_exists != 0)//on est colles a un token
	{
		//data->token->end_token_pos = i - 1;
		//ft_get_token_content(data, data->token->start_token_pos, data->token->end_token_pos, line);
		//data->token->start_token_pos_exists = i;
		//data->token->end_token_pos = i;
		ft_get_token_content(data,i, i, line);
		data->token->start_token_pos_exists = 0;
		data->token->end_token_pos = 0;
		//i++;
	}
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
	{
		//le 1 er caractere est un ESPACE
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
			ft_char_is_operator(line, i, data);
		//		ft_get_token_function(c, lst_token);
//on a un token abouti ici donc on peut l imprimer. il vient soit de la ft get token quoting rule car le car qui suit est un \0 indiquant la fin du token par ex
		if (data->token->end_token_pos != 0)//ic i on a imprime l <l> , ou ""^
			{
				ft_get_token_content(data, data->token->start_token_pos, data->token->end_token_pos, line);//on ne remet pas a 0 les compteurs start et end?
				data->token->start_token_pos_exists = 0;
				data->token->end_token_pos = 0;
				//TODO remettre start to a 0
			}
		i++;
	}
	return (i);
}
t_list	*ft_trim_and_clear(char *line, t_data *data)
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
	// je verifie pour chaque char  :  la rule, le type, la function, le content,
	//TODO
	// apres avoir determine et delimite notre token dans son content on va pouvoir effectuer l expansion si necessaire
	// ft_get_token_expansion(c, lst_token);

	// une fois qu on a termine de delimiter notre token et qu on a effectue son expand, on va s occuper de le RETOKENIZER au besoin :  2 conditions pour retokenizer le token :
	//  1/SI ET SEULEMENT SI on est en WS_Separator rule : on derva RETOKENIZER le token et 2/SI ET SEULEMENT SI on a pas un caractere qui vient annuler le trim and clear et retokenisation
	// par ex :  $VAR$ le $ a la fin vient annuler le retokenize donc si export VAR="       5       esp    " $VAR$ sera command not found en bloc$(donc pas de retokenization) puisque $VAR$est un token donc on subistitue lexpand mais le $ de fin reste evidemment.
	// echo $VAR$ : on aura un bloc non trime et non clear
	// donc on refait un tour de trim and clear

	//	if (lst_token->retokenize_token_allowed == 1 && lst_token->quoting_rule == 0 == whitespace_separator rule)//verifier le caractere qui suit la substitution et les 2 conditions ci dessus
	//		ft_trim_and_clear_and_retokenize_token(lst_token);
	// else : quote removal
	//	ft_quote_removal : yes si quoting rule == 1 ou 2 soit sq ou dq
	return (data->lst_token);
}

t_list	*ft_tokenize_line_to_lst_b(char *line)
{
	 t_data	*data;
	data = ft_struct_init_data(&data);
	data->lst_token = ft_trim_and_clear(line, data);
	//ft_retrim_and_clear()
	//ft_retokenize()
	return(data->lst_token);
}

t_list *ft_tokenize_line_b(char *line)
{
	t_list *lst_token;
	lst_token = NULL;

	if (mode_lexing_build == 0)
	{
		(void)lst_token;
		ft_tokenize_line1(line);
	}
	else
		lst_token = ft_tokenize_line_to_lst_b(line);
	return (lst_token);
}