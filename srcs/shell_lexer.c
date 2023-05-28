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
#define mode_tokenize_build 1

// copie n bytes dun bloc M de src (sans structuration) a la zone M dst.
// strcpy != memcpy : strcpy : copie --> \0 ou -->segfault  memcpy : copie tt le buffer (s arrete a n bytes)

// #include "libft.h"

void *ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char *s;
	unsigned char *d;

	s = (unsigned char *)src;
	d = (unsigned char *)dst;
	if (!src && !dst)
		return (NULL);
	while (n > 0)
	{
		*d = *s;
		d++;
		s++;
		n--;
	}
	printf("dst: <%s> apres memcpy\n", (unsigned char *)dst);
	return (dst);
}

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

void ft_get_token_content_lengh_for_malloc(t_list *lst_token, size_t start_token_pos, size_t end_token_pos)
{
	size_t size_content;

	size_content = end_token_pos - start_token_pos;

	// lst_token->content = malloc(sizeof(char*) * (size_content + 1));
	// TODO : lst_token->content init a zero ou terminate a NULL
	lst_token->content = ft_init_cstring(&((lst_token)->content), size_content + 1, 0);
}

void ft_get_token_content(t_list *lst_token, size_t start_token_pos, size_t end_token_pos, char *line)
{

  	ft_get_token_content_lengh_for_malloc(lst_token, start_token_pos, end_token_pos);
	lst_token->content = ft_memcpy(lst_token->content, &line[start_token_pos], end_token_pos - start_token_pos + 1);
	printf("content: <%s> \n", lst_token->content);
	printf("quoting rule: [%d] \n", lst_token->quoting_rule);
}

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

int ft_get_token_type(char *str, t_list *lst_token)
{

	(void)lst_token;
	// ici on a le choix de 1 a 13 entre le type de token auquel on a affaire : (define)
	// WORD 1, VARIABLE 2, PIPE  3 GREAT 4 GREATGREAT 5 LESS 6  LESSLESS 7 TERMINATE 8  PARENTHESIS_LEFT 9 PARENTHESIS_RIGHT 10 AMPERSAND 11 GREAT_AND_AMPERSAND 12 IGNORE 13
	// ici on va devoir update les positions de start et end token, car si on tombe sur un operator
	/*
	#define WORD 1
	#define VARIABLE 2
	#define PIPE  3
	#define GREAT 4
	#define GREATGREAT 5
	#define LESS 6
	#define LESSLESS 7
	#define TERMINATE 8
	#define PARENTHESIS_LEFT 9
	#define PARENTHESIS_RIGHT 10
	#define AMPERSAND 11
	#define GREAT_AND_AMPERSAND 12
	#define IGNORE 13
	//LESSGREAT 14 ?
	*/
	/*if (c == '\n')		{
		return NEWLINE;
	}
	*/

	/*si tabs et espaces [ \t]	{
		 Discard spaces and tabs
		 1,2,3,4,5,6,7,8,9,10...14
	}
	*/
	if (lst_token->quoting_rule != single_quote && lst_token->quoting_rule != double_quote)
	{
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
		// pour le mot on pourrait plutot faire par elimination
		/*
			[^ \t\n][^ \t\n]*	{
			 on assume que les noms de fichiers n ont que des characteres alphabetiques NB PB AVEC LE NOTOKEN RULE A PLACER EN DESSOUS CF NANI
			yylval.string_val = strdup(yytext);
			return WORD;
			il faudra retourner le mot.
			<ls>,<-la>,<a*>,<grep>,<c>,<head>,<-n5>, <wc>,<-l>,<outfile>,<infile>
		}
	*/
	}
	return (0);
}

void ft_get_token_quoting_rule(char *str, t_list *lst_token, size_t i)
{
	// cette fonction va permettre de determiner quelle est la regle de quoting : double quoting single quoting ou whitespace_separator
	//  decrite ici separemment pour plus de visibilite mais sera intergree normalement et fondue dans la trim and clear
	// mettre un INTERRUPTEUR ICI qui definit la regle du quoting rule  whitespace_Separator 0 single quoting 1 double quoting2 par ex il faudrait
	// NB c est le premier quoting rule rencontre qui l emporte cf echo "'$VAR'" ou "'$VAR'"
	//	(void)c;
	//	(void)lst_token;
	char c;

	c = str[i];
	if (lst_token->quoting_rule == 0 && c == '\"')
		lst_token->quoting_rule = 2;
	else if (c == '\"' && lst_token->quoting_rule == 2)
	{
		lst_token->end_token_pos = i;
		lst_token->quoting_rule = 2;
	}
	else if (lst_token->quoting_rule == 0 && c == '\'')
		lst_token->quoting_rule = 1;
	else if (c == '\'' && lst_token->quoting_rule == 1)
		lst_token->quoting_rule = 0;

	else if (lst_token->quoting_rule == 0 && str[i + 1] == '\0') // TODO verifier ici le cas
		lst_token->end_token_pos = i;
}
/*
t_list	*ft_list_init(t_list **token_list)
{

//j initialise mon premier maillon avec le content a NULL afin de pouvoir recuperer la data de ma structure t_list au fur et a mesure que je parcours char apres char
	*token_list = (t_list *)malloc(sizeof(t_list));
	if (!(*token_list))
		return (0);
	(*token_list)->content = NULL;
	(*token_list)->position = 0;
	(*token_list)->type = 0;
	(*token_list)->function = 0;
	(*token_list)->quoting_rule = 0;
	(*token_list)->retokenize_allowed = 0;
	(*token_list)->next = NULL:;
	(*token_list)->previous = NULL;
	return (*token_list);
}
*/
t_list *ft_lstlast(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void ft_lstadd_back(t_list **alst, t_list *new)
{
	if (new)
		new->next = NULL;
	if (!(*alst))
		*alst = new;
	else
		ft_lstlast(*alst)->next = new;
}

t_list *ft_lstnew(char *content)
{
	t_list *list;

	list = malloc(sizeof(t_list));
	if (!list)
		return (NULL);
	list->content = content;
	list->position = 0;
	list->end_token_pos = 0;
	list->start_token_pos = 0;
	list->start_token_pos_exists = 0;
	list->type = 0;
	list->function = 0;
	list->quoting_rule = 0; // on met a Whitespace_separator rule par defaut.
	list->retokenize_allowed = 0;
	list->next = NULL;
	list->previous = NULL;
	return (list);
}

t_list *ft_create_list_and_add_token(char *token_content) // question est ce que je cree une structure qui va contenir la tete de ma listee chainee ou est ce que je cree des le debut ma liste dans le main.c je pense que mettre ma tete de liste dans une variable globale serait meme interessant, puis je mettre en variable globale une structure qui contiendrait ce que je veux ....
//? cqfd je peux utiliser fT_lstaddback sans avoir besoin de generer la tete de liste, mais cela serait bien que j ai acces a la liste chainee d ou je veux. a voir
{
	t_list *new;
	char *content;
	t_list *alst;

	content = token_content;
	alst = NULL;
	new = ft_lstnew(content);
	ft_lstadd_back(&alst, new);
	return (alst);
}

// void	ft_trim_and_clear(char *line, t_list *token_list)

void ft_trim_and_clear(char *line)
{
	// char 	c;
	char *str;
	char *token_content;
	t_list *lst_token;
	size_t start_token_pos;
	//	size_t	end_token_pos;
	size_t i;

	str = line;
	i = 0;
	token_content = NULL;
	start_token_pos = 0;
	//	end_token_pos = 0;
	//(void)line;

	// je parcours ma line et en trimant et clearant les espaces je determine chaque <token> : 1 token est soit separe par un espace, soit separe par un operand ou un metacharacter lui meme separateur de token. par ex |
	// pour que trim and clear je dois etre en mode Whitespace_Separator, sinon .... mon espace n a pas de caractere special de separateur si je suis en quoting rule DQ ou SQ, ceci s applique egalement a tous les operands qui perdent toute valeur speciale
	// donc je parcours char par char et si " ou ' je dois signaler un quoting rule. il faudra etre sorti du quoting rule mode pour pouvoir separer les tokens en fonction des espaces ou des operands
	// je dois aussi determiner un start_token_pos et end_token_pos pour mon token puis je vais faire un ft strdup ou ndup pour dupliquer le token et remplir le content de mon maillon de la liste chainee
	// je dois donc considerer deja etre dans mon premier maillon de liste chainee ici. trouver une condition qui me permet de generer mon premier maillon et je renseignerai donc le quoting rule ici
	// remarque : si j ai ls -la >"outfile" | $VAR q: mon operator > GREAT doit il separer "outfile" ?OUI <>> <"outfile">
	// si je tombe sur un \0 ou en Whitespace_separator rule sur un espace/operand -> c est la fin de mon token --->mon end_token_pos sera recycle pour etre le depart de la recherche du prochain token
	//

	// condition: des que je tombe sur un char je lst_add_new_token ->je ne respecte pas cette donnee ici je cree directement le token
	// sont content sera NULL en attendant que .... on fixe tous les parametres de la structure

	lst_token = ft_create_list_and_add_token(token_content);
	while (str[i])
	{
		while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		{
			i++;
			if (str[i])
			{
				if ((lst_token->quoting_rule == whitespace_separator) && (lst_token->start_token_pos_exists == 0) && (!(str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))))
				{
					if (ft_get_token_type(&str[i], lst_token)) // 0 est return si pas de type
					{
						if(lst_token->start_token_pos_exists == 0)//on n est pas colles a un token
						{
							lst_token->start_token_pos = i;
							lst_token->end_token_pos = i;
							ft_get_token_content(lst_token,i, i, line);
							lst_token->start_token_pos_exists = 0;
							lst_token->end_token_pos = 0;
							i++;
						}
					}
					else{
						lst_token->start_token_pos = i;//lst->
						lst_token->start_token_pos_exists = 1;
						break;
						}
				}
				if (((lst_token->quoting_rule == whitespace_separator)) && (lst_token->start_token_pos_exists == 1))
				{
					lst_token->end_token_pos = i - 2;

                        if (lst_token->end_token_pos != 0)
							ft_get_token_content(lst_token, lst_token->start_token_pos, lst_token->end_token_pos, line);
					// i++;
					lst_token->start_token_pos_exists = 0;
					lst_token->end_token_pos = 0;
				}
			}
			if ((str[i] == 0) && (lst_token->quoting_rule == whitespace_separator) && (lst_token->start_token_pos_exists == 1)) // pas besoin d avvoir lui ici pusique la condition d entree est que str[i] existe....
			{
					lst_token->end_token_pos = i - 2; // si pas d espace, i -2 si un esapce avant
					if (lst_token->end_token_pos != 0)
						ft_get_token_content(lst_token, lst_token->start_token_pos, lst_token->end_token_pos, line);
						// i++;
					lst_token->start_token_pos_exists = 0;
					lst_token->end_token_pos = 0;
					break;
			}
		}
		// on n est pas sur un espace
		if (str[i] == 0)
		{
			if ((lst_token->quoting_rule == whitespace_separator) && (lst_token->start_token_pos_exists == 0))
			{
				break;
			}
			if ((lst_token->quoting_rule == whitespace_separator) && (lst_token->start_token_pos_exists == 1))
			{
				lst_token->end_token_pos = i - 1; // si pas d espace, i -2 si un esapce avant

				if (lst_token->end_token_pos != 0)
					ft_get_token_content(lst_token, lst_token->start_token_pos, lst_token->end_token_pos, line);
				// i++;
				lst_token->start_token_pos_exists = 0;
				lst_token->end_token_pos = 0;
				break;
				// si on avait un \0
			}
		}
		if ((lst_token->quoting_rule == whitespace_separator) && (lst_token->start_token_pos_exists == 0) && (!(ft_get_token_type(&str[i], lst_token))) && (!(str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))))
		{
			lst_token->start_token_pos = i;
			lst_token->start_token_pos = i;

			lst_token->start_token_pos_exists = 1;
		}
		ft_get_token_quoting_rule(str, lst_token, i);
		if (ft_get_token_type(&str[i], lst_token)) // 0 est return si pas de type
			{
				if(lst_token->start_token_pos_exists == 0)//on n est pas colles a un token
				{
					lst_token->start_token_pos = i;
					lst_token->end_token_pos = i;
					ft_get_token_content(lst_token,i, i, line);
					lst_token->start_token_pos_exists = 0;
					lst_token->end_token_pos = 0;
					//i++;
				}
				else if(lst_token->start_token_pos_exists != 0)//on est colles a un token
				{
					lst_token->end_token_pos = i - 1;
					ft_get_token_content(lst_token, lst_token->start_token_pos, lst_token->end_token_pos, line);
					lst_token->start_token_pos_exists = i;
					lst_token->end_token_pos = i;
					ft_get_token_content(lst_token,i, i, line);
					lst_token->start_token_pos_exists = 0;
					lst_token->end_token_pos = 0;
					//i++;
				}
			}
	
	
		//		ft_get_token_function(c, lst_token);

		if (lst_token->end_token_pos != 0)//ic i on a imprime legrep"
			{
				ft_get_token_content(lst_token, lst_token->start_token_pos, lst_token->end_token_pos, line);//on ne remet pas a 0 les compteurs start et end?
				lst_token->start_token_pos_exists = 0;
				lst_token->end_token_pos = 0;
			}
		i++;
	}

	// je verifie pour chaque char  :  la rule, le type, la function, le content,

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
}

void ft_tokenize_line_to_lst(char *line)
{
	// t_list	*token_list;
	// parcourir la line de maniere sequentielle puisque les quoting rules vont determiner les qualites des caracte speciaux et des operands, aussi nous avons besoin de passer les espaces pour trouver un token dans le cas de nos gestion de comportement de caracteres a la volee char on the fly ou motifs
	// ex de <$VAR> et <ls> <|> <$VAR>
	// 1
	// j initialise ma liste chainee en mettant tout a 0

	// ft_list_init(&token_list);
	// en envoyant l adress de token_list, on viendra modifier directement en memoire sa valeur, donc pas bsoin de recuperer la structure token_list a la sortie de la fonction, on peut librement utiliser token_list dans un appel de fonction
	// ft_trim_and_clear(line, token_list);

	// reflechir a la meilleure option entre initialiser des le debut la liste chainee avec le 1 er maillon
	// ou le faire dans la trim and clear

	ft_trim_and_clear(line);

	// return la lst_token
}

char **ft_tokenize_line1(char *line)
{
	char **token_tab;
	int i;

	i = 0;
	token_tab = ft_split(line, ' ');
	while (token_tab[i])
	{
		printf("token <%s>\n", token_tab[i]);
		i++;
	}
	return (token_tab);
}
void ft_tokenize_line(char *line)
{
	// gardons cette fonction pour ne pas faire crasher le code en attendant de fr la vraie tokenize line operationnelle qui redistribue les tokens dans la liste chainee directement et pas dans un double tab
	// la liste chainee est vraiment justifiee car on a dans la cadre des expand, besoin de REtokenizer encore une fois avec le TRIM and ClEAR. on doit pouvoir manier les token de maniere flexible. un tableau serait tres galere a modifier et remodifier
	if (mode_tokenize_build == 0)
	{
		ft_tokenize_line1(line);
	}
	else
	{
		ft_tokenize_line_to_lst(line);
	}
}

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
