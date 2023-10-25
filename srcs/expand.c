/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 11:53:45 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/09/04 11:53:59 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_get_token_quoting_rule3(char *str,size_t i, int *quoting_rule, int *quoting_rule_adequate, char *buffer_dequote, int *j)
{	
	(void)buffer_dequote;
	(void)i;
	char c;

	c = str[i];
	//AU TOUT DEBUT
	if (*quoting_rule_adequate == 0 && *quoting_rule == 0 && c == '\'' && str[i + 1]!= '\0')
		*quoting_rule = 1;
	else if (*quoting_rule_adequate == 0 && *quoting_rule == 0 && c == '\"' && str[i + 1]!= '\0')
		*quoting_rule = 2;


	//venir cloturer la QR
	else if (*quoting_rule_adequate == 0 && *quoting_rule == 2 && c == '\"' && str[i + 1] != '\0')
		{
			*quoting_rule = 0;
			*quoting_rule_adequate = 1;
		}
	else if (*quoting_rule_adequate == 0 && *quoting_rule == 1 && c == '\'' && str[i + 1] != '\0')
		{
			*quoting_rule = 0;
			*quoting_rule_adequate = 1;
		}


	//Deamrrer un nouveau paquet-> en situatio de demarrage a nouveau le constat est  : QR == 0 et QRA ==1
	else if (*quoting_rule_adequate == 1 && *quoting_rule == 0 && c == '\"' && str[i + 1] != '\0')
		{
			*quoting_rule = 2;
			*quoting_rule_adequate = 0;
		}
	else if (*quoting_rule_adequate == 1 && *quoting_rule == 0 && c == '\'' && str[i + 1] != '\0')
		{
			*quoting_rule = 1;
			*quoting_rule_adequate = 0;
		}
	//Fermer le Second Paquet

	//fermer le dernier quote avant \0
	else if(str[i + 1] == '\0' && *quoting_rule_adequate == 0 && *quoting_rule == 2 && str[i] == '\"')
	{
		*quoting_rule_adequate = 1;
		*quoting_rule = 0;
	}
	else if(str[i + 1] == '\0' && *quoting_rule_adequate == 0 && *quoting_rule == 1 && str[i] == '\'')
	{
		*quoting_rule_adequate = 1;
		*quoting_rule = 0;
	}
	else
	{
		buffer_dequote[*j] = c;
		*j = *j + 1;
	}
return(*quoting_rule_adequate);
}
int ft_get_token_quoting_rule2b(char *str, size_t i, int *quoting_rule, int *quoting_rule_adequate)
{	
	char c;

	c = str[i];

	
	if (*quoting_rule == 0 && c == '\"' && str[i + 1] != '\0')
		*quoting_rule = 2;
	else if (c == '\"' && *quoting_rule == 2)
	{
		*quoting_rule = 2;
		if(*quoting_rule_adequate == 1)
			{
				*quoting_rule_adequate = 0;
				*quoting_rule = 0;
			}
		else
			*quoting_rule_adequate = 1;
	}
	else if (*quoting_rule == 0 && c == '\'' && str[i + 1] != '\0')
		*quoting_rule = 1;
	else if (c == '\'' && *quoting_rule == 1)
	{
		*quoting_rule = 1;
		if(*quoting_rule_adequate == 1)
			{
				*quoting_rule_adequate = 0;
				*quoting_rule = 0;
			}
		else
			*quoting_rule_adequate = 1;

		/*
		*quoting_rule = 0;
		*quoting_rule_adequate = 1;
		*/
	
	}
	else if (*quoting_rule == 0 && str[i + 1] == '\0') // c est le cas de $> l[s]    ->[s] est checke dans la ft_get_token_quoting rule on verifie si la quoting rule  == 0 et que lindex suivant est un \0 alors cela signifie qu on a la fin d un token
	{
		if (c == '\"')
			*quoting_rule = 2;
		if (c == '\'')
			*quoting_rule = 1;
	}
return(*quoting_rule_adequate);
}


int ft_get_token_quoting_rule2(char *str, size_t i, int *quoting_rule, int *quoting_rule_adequate)
{	
	char c;

	c = str[i];

	
	if (*quoting_rule == 0 && c == '\"' && str[i + 1] != '\0')
		*quoting_rule = 2;
	else if (c == '\"' && *quoting_rule == 2)
	{
		*quoting_rule = 2;
		if(*quoting_rule_adequate == 1)
			*quoting_rule_adequate = 0;
		else
			*quoting_rule_adequate = 1;
	}
	else if (*quoting_rule == 0 && c == '\'' && str[i + 1] != '\0')
		*quoting_rule = 1;
	else if (c == '\'' && *quoting_rule == 1)
	{
		*quoting_rule = 1;
		if(*quoting_rule_adequate == 1)
			*quoting_rule_adequate = 0;
		else
			*quoting_rule_adequate = 1;

		/*
		*quoting_rule = 0;
		*quoting_rule_adequate = 1;
		*/
	
	}
	else if (*quoting_rule == 0 && str[i + 1] == '\0') // c est le cas de $> l[s]    ->[s] est checke dans la ft_get_token_quoting rule on verifie si la quoting rule  == 0 et que lindex suivant est un \0 alors cela signifie qu on a la fin d un token
	{
		if (c == '\"')
			*quoting_rule = 2;
		if (c == '\'')
			*quoting_rule = 1;
	}
return(*quoting_rule_adequate);
}
