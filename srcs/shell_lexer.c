/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_lexer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 15:55:07 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/02/07 15:55:30 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
ici equivalent du fichier shell.l : simple analyseur lexical pour le shell

 LEXER/lex(vrai shell.l)==shell_l.c decrit [^"expressions regulieres"]*  et return les <TOKENS>
 on va parcourir notre string line de l input et faire matcher la string avec les "expressions regulieres"
%{
#include <string.h>
#include "y.tab.h"
%}
%%

si retour a la ligne \n 		{
	return NEWLINE;
}

si tabs et espaces [ \t]	{
	 Discard spaces and tabs
	 1,2,3,4,5,6,7,8,9,10...14
}


si on tombe sur ">"		{
	return GREAT;
	1
}
si on tombe sur "<"		{
	return LESS;
	1
}
">>"		{
	return GREATGREAT;
}
">&"	{
	return GREATANDPERSAND
}
si on tombe sur "|" {
	return PIPE;
	<|>,<|>,<|>
}
"&"	{
	return AMPERSAND;
}
[^ \t\n][^ \t\n]*	{
	 on assume que les noms de fichiers n ont que des characteres alphabetiques NB PB AVEC LE NOTOKEN RULE A PLACER EN DESSOUS CF NANI
	yylval.string_val = strdup(yytext);
	return WORD;
	il faudra retourner le mot.
	<ls>,<-la>,<a*>,<grep>,<c>,<head>,<-n5>, <wc>,<-l>,<outfile>,<infile>
}
%%
*/

