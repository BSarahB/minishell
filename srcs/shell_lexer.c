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




//QUESTION TRES IMPORTANTE :  est ce que je tokenise la line ou chaque block? je prefere travailler sur les blocks car j ai deja organise le code de l executor et que c etait mon idee premiere. j espere ne pas faire fausse route et retomber sur le meme resultat que si j avais lexe et parse la line d un coup du debut a la fin
char *ft_split_block(char *block, const char *delim)
{
	char *token;


	return(token);
}



char *token;
token = ft_split_block(blocks[i], "delimitateurs") //ou split voir car je n ai pas envie de malloc je malloc deja bcp pour les doubles tabs cmd_and_Args et abs de la structure simpleCmd , je voudrais juste les y envoyer apres avoir parse les BLOCKS

//si infile est dans le 1 er block ou le dernier on considerera infile comme etant l infile de la cmd entiere(general_infile) et pas d une simpleCmd. 
//si infile est dans un block intermediaire, on le considerera infile intermediaire (sub_infile)
//l outfile dune last simpleCmd est un general_outfile. 
//l outfile dune premiere ou intermediaire simpleCmd sera considere comme un sub_outfile
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