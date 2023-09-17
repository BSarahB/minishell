/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_and_retokenize.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 13:52:34 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/09/04 13:52:55 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
0/Delimiter le debut et a fin de l expand (cf fiche pour les regles QR 0 et QR 2)
0/A le debut de l expand : SI $ EST En QUOTING RULE == 0 : $ doit etre obligatoirement suivi:
-1) d un caractere alpha
-2) d un caractere num compris entre 0 et 9
-3) d un " apostrophe double rempli ou non
-4) d un ' apostrophe simple rempli ou non
-5) d un signe ? ou !
-6) d un signe parmi : ($ suivi immediatement de)
OUI expand: suivi d un des signes: {$(1 ou plusieurs),!, # -> seront substitues par a valeur ou par RIEN (on aura le return \n a l echo )
NON expand: suivi d un des signes: {+, ,(virgule) }, ], = , ~ }  -> ne donnent pas de valeurs a l expand. $ suivi d un de ces signes n aura pas de valeur expand. il aura son sens litteral de dollar
delimitateurs

[idem pour QR == 2 , (pas d expand en QR == 1)]

0/B la fin de l expand :  SI $ EST DANS LA QUOTING RULE == 0
-2) ds le cas d un caractere numerique : l expansion s arrete a ce moment la
-3) & 4) l expansion s arretera au "" ou ' (on n inclut dans la substitution que le $, et n le fera disparatra car ce sera un $ solo, il ne substituera a RIEN)
-5) l expansion s arrete au $ ! #

fin de l expand dans le cas de $ dans la QUTING RULE == 2
- des qu on rencontre un espace
-2) ds le cas d un caractere numerique : l expansion s arrete a ce moment la ex :  echo "$1232" L expansion s arretera des le 2
-5) l expansionn s arrete au ? ou !
-6) des que je rencontre un " ou '
-des qu on rencontre un autre $ ou les signes +#-?@= etc...
OUI expand: suivi d un des signes: {$(1 ou plusieurs),!, # -, }  -> seront substitues par a valeur ou par RIEN (on aura le return \n a l echo )
les signes suivant annulent la valeur de l expand
NON expand: suivi d un des signes: {+, ,(virgule) }, ], = , ~ }  -> ne donnent pas de valeurs a l expand. $ suivi d un de ces signes n aura pas de valeur expand. il aura son sens litteral de dollar
delimitateurs


***EXPAND ***

1/substitute
2/if found : copy in buffer if (not found || $ trouve seul): no copy
3/epur multiple spaces sauf dans les "bloc" ou 'bloc' -> on aura qu un seul espace la ou il y en avait  plusieurs et dans les "blocs" les espaces seront preserves.
4/Trim les spaces aux extremites X   X de la string du buffer
5/RETOKENIZE / si echo : PAS de retokenize
6/DEQUOTE(quote removal sauf dans les "bloc" ou 'bloc') (epurer la string des signes "" ou ' sauf quand ils sont dans  des "bloc" ou 'bloc')
*/

/*
void	ft_is_trim_and_clear_and_retokenize_token_allowed(t_list *lst_token)
{


}


char *ft_substitute(t_list *lst_token)
{
	char *expanded_content;
	expanded_content == NULL;

	//on va effectuer la substitution /expansion de notre VAR
	return(expanded_content);

}


void	ft_get_token_expansion(void)
{
}

*/

// void ft_get_scope_expand(void)
//{

//}
int ft_strcmp_char(char c1, char c2)
{
	return (c1 - c2);
}

int ft_is_expand_unvalidated(char **invalidators, char c)
{
	int i;

	i = 0;
	while (invalidators[i] != NULL)
	{
		if (ft_strcmp_char(invalidators[i][0], c) == 0)
			return (1);
		i++;
	}

	return (0);
}

int ft_isdigit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int ft_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}

int ft_is_alphanum(char c)
{

	if (ft_isalpha(c) || ft_isdigit(c))
		return (1);
	return (0);
}

int ft_is_expand_here(char *str)
{
	int quoting_rule_adequate;
	int quoting_rule;
	int i;
	// int i_save;
	char *invalidators[] = {"+", ",", "}", "]", "~", "=", NULL};
	// char *delimitators[] = {"#", "]", "{", "}",  "-", "+", "?", "@", "", NULL}; //vont determiner la FIN de l expand

	int flag_expand_here;
	int start_expand_pos;
	int end_expand_pos;

	i = 0;
	flag_expand_here = 0;
	quoting_rule_adequate = 0;
	quoting_rule = 0;
	start_expand_pos = 0;
	end_expand_pos = 0;
	while (str[i])
	{
		quoting_rule_adequate = ft_get_token_quoting_rule2(str, i, &quoting_rule, &quoting_rule_adequate);
		// i_save = i;
		if (flag_expand_here == 1)
		{
			if (quoting_rule == 0)
			{
				
				if (str[i -1] == '$' && ft_isdigit(str[i]) == 1)
				{
					//$2000
					end_expand_pos = i;
					flag_expand_here = 0;

				}
				else if (str[i] == '\"' || str[i] == '\'')
				{
					//$"VAR"
					end_expand_pos = i - 1;
					flag_expand_here = 0;

				}
				else if (str[i] == '$' && str[i - 1] != '$')
				{
					//$VAR$
					end_expand_pos = i - 1;
					flag_expand_here = 0;

				}
				else if(str[i] == '$' && str[i -1] == '$' && str[i + 1] == '\0')
				{
					//$VAR$$\0
					end_expand_pos = i;
					flag_expand_here = 0;

				}	
				else if (ft_is_alphanum(str[i]) == 0)
				{
					//$VAR+
					//si stri == $ && str[i -1] != $
					if(!(str[i] == '$' && str[i -1] == '$')) // cs de $VAR$$$
						{
							end_expand_pos = i - 1;
							flag_expand_here = 0;
						}

				}
				else if (str[i + 1] == '\0') //TODO
				{
					end_expand_pos = i ;
					flag_expand_here = 0;

				}		
			}
			if(quoting_rule == 2)
			{
				//- des qu on rencontre un espace "$VAR l"

				if(str[i - 1] != '$' && (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13)))
				{
					end_expand_pos = i - 1;
					flag_expand_here = 0;


				}
				//-2) ds le cas d un caractere numerique : l expansion s arrete a ce moment la ex :  echo "$1232" L expansion s arretera des le 2
				else if (str[i -1] == '$' && ft_isdigit(str[i]) == 1)
				{
					//"$2000"
					end_expand_pos = i;
					flag_expand_here = 0;

				}
				else if (str[i] == '\"' || str[i] == '\'')
				{
					//$"VAR"
					end_expand_pos = i - 1;
					flag_expand_here = 0;

				}

				else if (str[i] == '$' && str[i - 1] != '$')
				{
					//"$VAR$"
					end_expand_pos = i - 1;
					flag_expand_here = 0;

				}
				else if(str[i] == '$' && str[i -1] == '$' && str[i + 1] == '\0')
				{
					//$VAR$$\0
					end_expand_pos = i;
					flag_expand_here = 0;

				}	

				//-5) l expansionn s arrete au ? ou !
				else if (ft_is_alphanum(str[i]) == 0)
				{
					//$VAR+
					if(!(str[i] == '$' && str[i -1] == '$')) // cs de $VAR$$$
						{
							end_expand_pos = i - 1;
							flag_expand_here = 0;
						}
				}
			}
		}
		
		if (str[i] == '$' && quoting_rule != 1 && flag_expand_here != 1) //&& que $ n est pas suivi de '\0' ->suivi de \0 signifie que ce n est pas un expand , mais simplement un caractere $
		{
			flag_expand_here = 1;
			if (str[i + 1] == '\0' || ft_is_expand_unvalidated(invalidators, str[i + 1]) == 1) //|| str[i + 1] ==  '\"')//cas du ls >VAR$
				flag_expand_here = 0;
			if (flag_expand_here == 1)
			{
				// ft_get_scope_expand();
				start_expand_pos = i;
			}
		}
		i++;
		if (quoting_rule_adequate == 1)
			quoting_rule_adequate = 0;
	}
	printf("start_expand_pos = %d, end_expand_pos = %d \n", start_expand_pos, end_expand_pos);
	if (flag_expand_here == 0)
		return (0);
	else
		return (1);
}

int ft_is_expand_to_substitute(t_list *lst_token)
{
	char *str;
	int i;
	int i_save;
	char *buffer;
	// char *backup_content;

	buffer = ft_init_string(1096);
	str = lst_token->content;
	// backup_content = ft_strdup(lst_token->content);

	i = 0;
	i_save = i;
	if (str == NULL)
		return (0);
	if (ft_strcmp(lst_token->content, "$") == 0)
		{
			free(buffer);
			return (0);
		}
	if (ft_is_expand_here(str) == 1)
	{
		printf("expand is here\n");

		/*while(str[i]) //ft_expand_authorized
		{


			quoting_rule_adequate = ft_get_token_quoting_rule2(str, i, &quoting_rule, &quoting_rule_adequate);
			i_save = i;
			if(str[i] == '$' && quoting_rule != 1) //&& que $ n est pas suivi de '\0' ->suivi de \0 signifie que ce n est pas un expand , mais simplement un caractere $
				{
					if(str[i + 1] == '\0') //|| str[i + 1] ==  '\"')//cas du ls >VAR$
						{
							printf("$ is ending the token");
							return(0);
						}
					ft_get_scope_expand(lst_token);
			//	if(ft_expand_exists(lst_token) == 1)
			//	{

			//	}
			//	else
			//		return(0);
				}
			else{
				buffer[i_save] = str[i];
			}
			i++;
			if(quoting_rule_adequate == 1)
				quoting_rule_adequate = 0;

		}

		printf("buffer : %s\n, buffer");
		*/
	}

	free(buffer);
	return (0);
}

void ft_expand_and_retokenize(t_list *lst_token)
{
	t_list *tmp;

	tmp = lst_token;
	if (tmp == NULL)
		return;
	while (lst_token)
	{

		ft_is_expand_to_substitute(lst_token);

		// ft_dequote(lst_token);

		lst_token = lst_token->next;
	}
	lst_token = tmp;
	return;
}