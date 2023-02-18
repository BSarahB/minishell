/*
* shell.l : simple analyseur lexical pour le shell
*/

%{

#include <string.h>
#include "y.tab.h"

%}

%%

\n 		{
	return NEWLINE;
}

[ \t]	{
	/* Discard spaces and tabs */
}

">"		{
	return GREAT;
}

"<"		{
	return LESS;
}

">>"		{
	return GREATGREAT;
}

">&"	{
	return GREATANDPERSAND
}

"|" {
	return PIPE;
}

"&"	{
	return AMPERSAND;
}

[^ \t\n][^ \t\n]*	{
	/* on assume que les noms de fichiers n ont que des characteres alphabetiques NB PB AVEC LE NOTOKEN RULE A PLACER EN DESSOUS CF NANI*/
	yylval.string_val = strdup(yytext);
	return WORD;
}

%%
