/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 15:10:59 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/02/07 15:11:02 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
 ************************************************************PSEUDO CODE MINISHELL***************************************************************


exemple pr gestion de commande : ls -la a* | grep c | head -n 5 | wc -l > outfile < infile  (pas besoin de gerer * wildcards)


Grammaire shell dans la forme Backus-Naur et
regles de grammaire et labels du Parser (3)  :

cmd[arg]* [| cmd[arg]*]*		[[> filename] [< filename] [>& filename] [>> filename] [>>& filename]]*			 [&]
[cmd_and_args]+[arg_list]			[io_m]	+	[io_m]	+	[io_modifier]	+	io_m   	+	io_m
  == [pipe_list]					==						[io_modifier_list]								 == background_opitonal

les elements decrits ci dessus sont dits "lablels"


 ***********************************************************SCHEMA GLOBAL DE LA STRATEGIE*******************************************************

1)input de characters					->  2)  LEXER/lex(vrai shell.l)==shell_lexer.c decrit expressions reg et return <TOKENS>	->  3)  PARSER/yacc traite shell.y) == shell_parser.c regles de grammaire et labels qui vont generer la table de commande						->	4) WILDCARD et ENVARS == vont generer la table de commande finale
1)prise de l input ls -la a*| grep c 	->	2)  on ft matcher la string:														->  3)  ex ds shell.y : arg_list: arg_list WORD{ ou autres labels: cmd_and_args ou pipe_list ou io_modifier_list ou background optional etc...
| head -n5 | wc -l > outfile < infile       <ls> <-la>																				ici	appel de fct inserer l argument a }
											<a*> <PIPE>																			***TABLE DE CMD : cmd(**)array de pointeurs sur struct + io_redirections***																					***TABLE DE CMD FINALE: final_cmd(**)***
											<grep> <c> <PIPE>											-struct simple_cmd -struct simple_cmd -struct simple_cmd -struct simple_cmd																							idem mais on vient detailler * le wildcard par ex.
											<head> <-n> <5> <PIPE>												(*)					(*)				(*)					(*)
											<wc> <-l>											    sont des struct[0]			  struct[1]		 struct[2]			 struct[3]
											<GREAT> <WORD>											qui pointent sur  arguments(**): des tableaux de pointeurs sur char(*)
											<LESS> <WORD>												-cmd_and_args(**) 		-cmd_and_args(**)		-cmd_and_args(**)	-cmd_and_args(**)	-io_redirections(**) 																->  	-arguments(**)
												ex ds shell.l : ">"{									l - a NULL			g c NULL			h - NULL		w - NULL		i o NULL																					l - a a NULL
														action:											s l *				r \0				e n 			c l 			n u (errfile)																				s l b d
														return GREAT;								   \0 a \0				e                   a \0		   \0 \0 			f t 																					   \0 a b a
												}												   char(*)\0				p 					d 								i f 																						 \0\0\0
												 : [^ \t\n] [^ \t\n]*{									  (*)			   \0				   \0								l i
													yyval.string_val = strdup(yytext); 																							e l
													return WORD;																											   \0 e
												}																															   (*)\0
																																												 (*)


->Ds le vrai shell il faut implementer la grammaire dans les fichiers shell.let shell.y pour permettre au parser d interpreter les lignes de commandes. Un shell fonctionne avec 2 outils UNIX: Lex et Yacc.
ces outils sont utilises pour implementer des compilateurs interpreteurs et preprocesseurs.
Dans le vrai shell le programme LEX traite le fichier shell.l et genere l analyseur lexical qui fait matcher les tokens
				et le programme yacc va traiter le fichier shell.y et genere le progamme parser

->5) La table de cmd finale est envoyee a l executor
												}												   char(*)\0				p 					d 								i f 																						 \0\0\0
												 : [^ \t\n] [^ \t\n]*{									  (*)			   \0				   \0								l i
													yyval.string_val = strdup(yytext); 																							e l
													return WORD;																											   \0 e
												}																															   (*)\0
																																												 (*)


->Ds le vrai shell il faut implementer la grammaire dans les fichiers shell.l et shell.y pour permettre au parser d interpreter les lignes de commandes. Un shell fonctionne avec 2 outils UNIX: Lex et Yacc.
ces outils sont utilises pour implementer des compilateurs interpreteurs et preprocesseurs.
Dans le vrai shell le programme LEX traite le fichier shell.l et genere l analyseur lexical qui fait matcher les tokens
				et le programme yacc va traiter le fichier shell.y et genere le progamme parser

->5) La table de cmd finale est envoyee a 5: l executor
	l executor va rediriger les entrees, sorties standards sil y a des redirections a faire.




*** description dune simple commande et ses arguments ***

typedef struct s_simpleCmd{
	int numberOfArguments;
	char **cmd_and_args;
	void insertArgument(char *argument);
}				t_simpleCmd;

*** description d une commande complete avec les multiples pipes eventuels et les IO redirections eventuels ***

typedef struct s_cmd{
	int numberOfSimpleCmds;
	t_simpleCmd **simpleCmds;
	char *outfile;
	char *inputfile;
	char *errfile;
	//a voir si on met tous les char * dans un double tab IO_redirections(**)
	int background;

	void prompt();
	void execute();
	void insertSimpleCmd(t_simpleCmd *simpleCmd);
	static t_comd currentCmd;
	static t_simpleCmd *currentSimpleCmd;

}				t_cmd;

								*** ETAPES ***

1) mise en place du PROMPT ft_prompt()
	-pour recuperer en boucle l entree utilisateur
	-pr recuperer l entree actuelle

2)ANALYSER l entree du user
	-lire l entree du user
			-prendre l input de caracteres 1)
	-objectif : mettre l input de caracteres dans une table de commande
			-(LEXER 2) decouper l input de caracteres en TOKENS <> qui sont des constantes
					-il faut mettre en place des expressions regulieres qui vont definir ces TOKEN <>
					-puis faire matcher la string d inputs de caracteres avec les regles pour renvoyer des TOKENS <>
							-on envoie ensuite l input de la string qui a matche avec l expression reguliere au parser en executant le code qui return <TOKEN>
									-on renverra pour le token WORD la copie du match (yyval.string_val)
			-(PARSER 3) recoit les tokens et generer la table de commandes
					-on va mettre les tokens dans la table de commande donc parser les tokens dans une data structure grace aux labels de la grammaire Backus-Naur
					-cmd sera un (**) tableau de pointeurs sur structs. les structs seront chacune des simple_cmd. chacune des simple_cmd contiendra un (**) arguments qui contient
							-remplir chaque simple_cmd : on insere les arguments ds chaque struct simple_cmd : on va remplir des tokens WORD par ex dans arguments(**) avec une ft_insert_argument
							-remplir la table de commande cmd : on insere les simple_cmd dans la cmd complete cmd(**) : avec une ft_insert_simple_cmd
								-on decrira aussi le commande complete cmd(**) avec des multiples pipes et des redirections sil y en a.
	-4) paufiner la table de commande en table de commande finale
			-faire intervenir les sous systemes wildcards et envars pour detailler la table de commandes precedemment acquise.

5)EXECUTOR eecuter la commande stockee dans la table de commande ft_execute()
		-la table de commande est prete
			pour executer la commande qui s y trouve :  details de ttes les etapes: sauvegardes, redirections, creation des processus chaque simple_cmd associee a un processus et des pipes
		le principe : C EST DANS LE PROCESSUS PARENT QU ON VA S OCCUPER DE METTRE EN PLACE TOUS LES PIPES ET LES REDIRECTIONS AVANT DE CREER LES PROCESSUS ENFANTS(== nos simple_cmd)
		  AINSI, LES ENFANTS HERITERONT DE TOUTE LA MISE EN PLACE AU NIVEAU DES REDIRECTIONS
		  0)SAUVEGARDES tmpin et tmpout des entrees et sorties par defaut stdin et stdout dans tmpin et tmpout.
		 1)PARAMETRAGE DE L INPUT INITIAL:
			-s occuper de infile : 2 cas: Existe t -il ou non? 	-si oui:			< infile -> open(infile, O_READ) , on va vouloir lire depuis infile comme si infile > a
																-si non: 		stdin sera simplement l entree sauvegardee dans tmpin
			donc fdin = open() ou fdin = dup(tmpin)
			***fd3 = dup(fd1)***dup() cree un nouveau fd (le + proche et libre) qui est relie a un objet de fichier ouvert (par ex la console) deja existant ici puisque tmpin pointe deja sur la console,
				on duplique le pointeur contenu ds fd1 et on le met dans fd3(creant le fd le + proche et libre)
					-> la primitive DUP utilisé pour SAUVEGARDES de stdin stdout avant les redirections.
		2)jeu des REDIRECTIONS: == rediriger l input et l output. (on redirige les vrais stdin et stdout car ce sont les vraies input et output que l ordi va utiliser)
					1)on set fdin et fdout 2) on redirige input stdin 0 et stdout 1 avec dup2.(ATTENTION : pour rediriger input stdin 0  et output stdout 1 avec dup2() on doit d abord avoir set/parametré fdin et fdout)
			on vient iterer sur le nombre de simple_comd :  pr chaque simple_cmd on va faire le jeu des redirections
					2 cas:


							si derniere commande : rediriger sur outfile ou sur la sortie standard, on set fdout soit a outfile sil existe, soit sur stdout par defaut. on redirigera ensuite.
							si pas derniere commande : objectif:    redirige la sortie sur un pipe ou un fichier
																		-on cree le tube pipe:
			***int fdpipe[2] et pipe(fdpipe)***  pipe est un appel systeme qui va creer 2 fd qui vont pointer vers 2 objets de fichiers INTERCONNECTES.
					pip[1] ecrivain va ecrire dans pip[0] lecteur qui pourra lire dans pip[1]
																		ainsi : 3 possibilites: -console ou outfile
																								-pip[1]
		3)detail de l execution :
			***FORK()*** est un appel systeme UNIX pr creer 1 nouveau processus, copie du processus parent


NOTES PERSONELLES:
je suis deja en train de devier de mon pseudo code et me rattache a ce que j ai deja fait avec pipex.
je continue , je verrai bien. je ne vais pas me mettre d obstacles la ou il n y en a pas encore. nb: ce n est pas comme ca qu on fait le pseudo code il faut partir du + general puis entrer dans le detail,  moi j entre direct dans le detail, c est un reflexe. pff bon .
idee a proposer a 42: faire un mois d integration pour apprendre aux debutants les outils et la methodologie du pseudo code. aussi leur faire decouvrir les testeurs.
j ai mis 5h a initialiser correctement(?) ma structure (mon double tab de pointeurs sur structures).... quelle .... plaie .... les bases sont a consolider.
je nai pas du tt fait le lexer. pour l instant j avance je fais ce qui me donne du resultat pour rentrer dans le bain et m approprier le projet un peu plus.
je n ai tjrs pas free certaines variables. a faire ce soir. imperativemnt .¸ ohhh
les pipes semblent marcher correctement, je devrais m occuper des outfiles et infiles ainsi que des outfiles et infiles dans un block(donc dans une simpleCmd)
TODO : demande de l aide pour debogger les processus enfants, je n arrive pas a entrer dans thread enfant. je ne sais pas comment faire, j avais les threads dans pipex,la jene les vois pas.
TODO sinon fessee free ce qu il reste a free


Les 3FD ouverts standards quand exit sont tt a faits normaux. et ne constituent pas une erreur avec le --track-fd=yes(option valgrind)
note personnelle : je ne respecte pas le pseudo code a la lettre, puisque j ai d abord fait l execution. je vais regler les derniers details et passer au parsing ensuite.


-> valgrind --suppressions=ignore_rl_leaks --leak-check=full --show-reachable=yes --track-fds=yes ./my_minishell
note personnelle : avoir fait l execution d abord m aide un peu a voir ce que execve prend en charge. mais je pense que le parsing en premier lieu aurait ete bien plus judicieux. ... surtout que je vois quil y a des manieres de faire avec arbre et tri recursif plus securise que la maniere dont je fais avec la boucle sur les simple commandes....
j ai peur d avoir fait tout mon systeme d execution pour rien....
je ne suis pas certaine de sa solidite

je passe maintenant au parsing. je vais tokeniser mes elements.
j avais procede en faisant des blocs de pipe . mais qd j en ai parle a une collegue elle m a decouragee. depuis 3 jours je suis dans l semoule j ai le doute. j ai envie de faire mon idee mais j ai peur que ce ne soit pas operationnel a la fin . j imerais plutot parser mes blocs . je pense que c est possible. mais j ai peur que cela ne marche pas.

ok quoi quil en soit.
PARSING: de bloc ou de line entiere a choisir a la fin.

la principale concern ici semble etre le qutoing. le signle quoting et le double quoting.
-le quoting sert a ce que le shell N INTERPRETE PAS LES METACARATERES. quoter demande au shell de prendre les metacaracteres au sens LITTERAL
  -> l anti SLASH \ dit au shell : le caractere qui est apres toi tu ne l interpretes PAS. prends le au sens LITTERAL
  les metacaracteres du shell sont :
  *  est un metacaractere role wildcard,
  $  est un metacaractere role appelle key=value ,
  (l antislash \ est lui meme un metacaractere)
-> l ESPACE dans le bash est considere comme un SEPARATEUR il ne est pas vu comme un caractere, role quil interprete est : separateur entre les tokens.
les espaces ne sont pas interpretes (par ex avec echo) et donc ne sortent pas dans le renderer SAUF SI
 un antislash est devant un espace : cela  va demander au shell de l interpreter au sens LITTERAL comme un espace et donc de le CONSERVER dans le renderer
-> l APOSTROPHE DIT au shell : DANS MON BLOC tu n interpretes RIEN , sauf moi meme ' qd tu me revois pour fermer MON BLOC.
ainsi :  tout ce qui est entre 'n est pas interprete'
-> PAREIL pour les GUILLEMETS sauf que entre GUILLEMETS le SHELL DEVRA INTEPRETER LE $(subtilite des guillemets ou les variables continuent d etre interpretees) il n y a que le $qui consrrve son caractere de metacaractere
entre apostrophes l espace sera dans un bloc ou il sera pris au sens LITTERAL pour un veritable espace
->les BACKQUOTES dit au shell de inserer LEXECUTION a l interieur . shell, suppose que ce qui est entre backquote est une commande et execute le bloc entre backquote
ces histoires de quoting sont relatives au commandes ECHO
echo backquotes
echo double_quotes
echo single_quotes


- annonce une OPTION



D apres le ***GNU BASH***  https://www.gnu.org/savannah-checkouts/gnu/bash/manual/bash.html#Shell-Syntax
-le shell ignore le symbole de commentaire '#'et le reste de la ligne

***ORDRE A SUIVRE POUR TRAITER LES INFORMATIONS COMME BASH:***

1)lecture de l input depuis 3 sources :
	1-lecture depuis fichier(script shell)
	2-depuis une string comme 1 argument en invoquant bash avec l option -c (ASK)
	3-depuis l input du user stdin entree clavier

2)casse l input en mots et operateurs en obeisssants aux regles de QUOTING decrites dans la section quoting.
	-> les tokens sont separes par des metacaracteres
	->on effectue l EXPANSION des ALIAS ICI durant cette etape
	->LE **********QUOTING*********
			-
3)Parser les tokens == parser  == remplir la data structure de cmd
	->soit en simple command, soit en compound command (il en existe 4 types de commandes composees : Group: {...;} par ex:$ { echo info1; echo info2; } >logfile , Subshell: (...) is similar to a group except that the commands are run in subshell environment. This means that variable assignments do not survive after the subshell completes. As an example:

$ a=0; (a=10; echo "inside=$a"); echo "outside=$a"
inside=10
outside=0, ou encore Test Command: Bash's advanced form of the test command, [[...]], can include several tests. Tests are separated by && or ||:

 )
	->une simple commande est une sequence de mots separee par des blancs et est terminee par un des tokens operateurs de controle \n (newline) ou '||' ; && & ;; , ;& ;;& | |& ( ou )
		l exit statut d une simple command est donne par waitpid function ou  128+n si la commande est terminee par le signal n --> je ne comprends pas car c est 0 en general le echo $? pr voir l exit status _de une simple commande
 4)SHELL EXPANSION -> on va effectuer expansions :	1) brace {} : bash$ echo a{d,c,b}e $ade ace abe ou mkdir /usr/local/src/bash/{old,new,dist,bugs}
																: dans les accolades ts les caracteres speciaux sont conserves, le Double Quote conserve sa qualite(== demande a shell de prendre au sens litteral sauf $ qui maintient son caractere de PARAMETRE)
																l accolade ne disparait pas sauf si elle est derriere le dollar que la VARIABLE PARAMETRE existe ou non, accolade est viree. echo ${NO} ou echo ${USER} ->je me demande si c est gere par echo ou si il faut remove les accolades manuellement avant d envoyer. je dois tester cette commande dans pipex
 TODO: tester ./pipex --> echo ${USER} et voir si les accolades sautent ou pas
 si elles sautent cela signifie que execve s en occupera pas besoin de retirer les accolades dans le traitement de l expansion

													2)tild ~  :  ~/foo cest $HOME/foo
													~+/foo cest $PWD/foo et
													~-/foo c est ${OLDPWD-'~-'}/foo


													3)extension des parametres du shell genre: ${parametre} le dollar introduit : parametre d expansion, commande de subsitution ou expansion arithmetic
													 ici je ne comprends pas les exemples donnes avec ${parameter:+word} puisque je ne recupere pas vraiment le eme rendu. cela correspond t il a une genre de substitution de la var?? a creuser
													 on a	${parameter:-word}
															${parameter:=word}
															${parameter:?word}
															${parameter:+word}
															${parameter:offset}
															${parameter:offset:length}

   je ne comprends pas tout ici								${parameter@operator}              ?


													4)substitution de commande : voir video youtube https://www.youtube.com/watch?v=-1w7jLsuwPU
													5)Arithmetic expansion    : $(( expression ))  https://www.youtube.com/watch?v=r30SIIHBkCw
													6)Substitution de processus  La substitution de processus permet de faire référence à l'entrée ou à la sortie d'un processus à l'aide d'un nom de fichier. Il prend la forme de
														<( liste ) ou >( liste )
														cf : >(list)<(list)<>/dev/fd
													7)Word Splitting  cqfd Note that if no expansion occurs, no splitting is performed.
													8)l extension du nom de fichier  Filename Expansion
													9)Quote Removal : After the preceding expansions, all unquoted occurrences of the characters ‘\’, ‘'’, and ‘"’ that did not result from one of the above expansions are removed.


********L ORDRE de la SYNTAXE********* :

separer les token en fonction des regles de quoting,
appliquer le quoting et alias,
parser en simple command et compound command,
appliquer l expansion shell dan l ordre :
			{},
			 ~,
			parametre $,
			substitution commande,
			expansion arithmetique,
			substitution de processus ,
			word splitting,
			expansion de nom de fichier,
			quote removal.


*****POUR LE PROJET 42 cela donne : *****

separer les token en fonction des regles de quoting,
appliquer le quoting rules
parser en simple command,
appliquer l expansion shell dans l ordre :
			 ~,
			parametre $,
			quote removal.
execution
(je dois verifier si je fais d abord le parsing ou d abord les expansions et le quote removal)
il y a aussi la suppression des redirections


TODO : avec les fichiers txt des commandes a gerer, commencer a figurer avec exemples et applications
note personnelle : pa contente de ma journee car j ai eu a gerer des choses personnelles. je dois rapidement effectuer les exemples concrets pour me figurer l ordre du lexing et parsing.
pour pouvoir ecrire le pseudo code du lexer et parser rapidemment car le temps passe tres vite.

 https://harm-smits.github.io/42docs/projects/minishell

Pour tokeniser line: == lexer

on parcourt la line
pour chaque caractere on va verifier:
					Au tout debut on va ecremer les espaces et tabulations
					si on tombe sur un char double quote , ou simple quote on change la quoting rule d_q ou s_q. sinon on met la quoting rule en mode Off
							-si double quote : on se preoccupe de rien, on parcourt la string jusqu a tomber sur une autre double_quote. tout le bloc entre "  " est considere comme un seul token et sera enregistre en tant que tel
							si quoting rules est en mode off : les ESPACES ont valeur de separateurs de token. donc il faudra parcourir les epsaces sans aucun pb jusqu a tomber sur soit un operateur soit un caractere																		on aurait pu avoir ici aussi la rule du caractere d echapement  mais cela n est pas demande dans le sujet donc... surtt ne pas chercher a faire les bonus ou autre puisque je suis deja bien lente
					 on verifie pour chaque caractere si le caractere est un espace ou un operateur. si cest le cas de l espace : cela signifie que nous avons atteint la fin d un token et si on rencontre un operateur cela signifie egalement  que nous avons atteint la fin d un token



Je reflechis au cas particulier de $VAR apres  export VAR="ls -la" -> fonctionne execute la commande ls et met en option -la
											 ="| cat -e" -> execute la commande | et ne la trouve pas no commund found
											= "ls -la -djd" -> no valid option
											="ls -la wc1 wc2" -> wc1 et wc2 ni such file or directory
											= "ls | wc -l" ici  -> pareil | et wc sont no such file or directory
											= "ls | cat -e" -> -e no valid option pour la commande ls
											TOUT ceci signifie que : quand on a $VAR et que la 1 ere commande n est pas un echo, mais ici un ls par ex ou un | le reste des elements doit etre
											integre comme des options pour cette commande. on aura donc a remplir cmd_and_args[0] = ls et cmd_amd_args[i++] avec <-la> <-djd> ou <cat> <-e>
											on utilisera split pour remplir le double tab et remplir cmd_and_args
											a lexpand, ls -la fonctionne.
											mais les pipes n ont pas de valeur d operateur.

											je continue de voir des exemples. je dois bien savoir quel est l ordre avant de commencer a ecrire le lexing
											export VAR="wc"
											ls | $VAR -l fonctionne tres bien. $VAR est substituee comme un wc COMMANDE
											apres un operateur | on considere que l expand qui suit est une commande.
											voyons si elle se complique
											export VAR="head -n 5"

											CEPENDANT quand la commande ECHO est la curr simple_cmd, alors, il faudra mettre l expand $VAR en tant qu option de la commande.
											on va donc determiner si simple_cmd est echo ou pas
echo -nnnn -nnn -n abc -> execve se charge de considerer -nnnn comme une option valide
echo -nnn -nnn abc -n -nna -naaa -nn -nn -nnn -nnnn -jfkefe kleflkfneife  CHOIX de la data structure : sera une liste chainee pour ne pas etre piege par le nombre d arguments , ni le nbr de commandes, et ne pas avoir besoin de les allouer au depart en etant bloque sur le nombre


EXPAND CASE:
fiche du cas de l expand faite : cas de l expand *ALONE: I $VAR  trim + clear/ 1er elem est cmd reste est options de ce 1 er elem
														II '$VAR' SQuote : expand not authorized / tout ce qui est dans le bloc est litteral - whitespace litteral / expand est 1 bloc cmd/l expand n a aucune valeur juste litteral pour ses caracteres
														III "$VAR" DQUOTE: expand authorized / whitespace_ltteral / expand est 1 bloc cmd
														-> ce qui est entre QUOTES est 1 BLOC.
,
								 cas de l expand *APRES ECHO: 	echo $VAR(suit la loi I), trim+clear / expand est 1 bloc-1option sos l egide de la cmd echo
																echo '$VAR'(II) expand NOT authorized - all litteral whitespaces litteral
																echo "$VAR" (III) expand authorized - all litteral whitespaces litteral

								 cas de l expand APRES COMMANDE: (ls par ex) : ls $VAR     : l expand est  1bloc-1option, puisquil est sous l egide dune cmd (ls) && il repond aux regles de I , II ou III

								 cas de l expand APRES | : l expand est 1 bloc cmd. il repond aux regles de I , II ou III

								 dans LExpand, les | ou > redirections n ont pas de valeur speciale, ils sont juste au sesn litteral et servent de cmd ou d option.
*/

#include "minishell.h"

void ft_void(void)
{
	printf("void\n");
}
int main(int argc, char *argv[], char *envp[])
{
	// TODO gerer le -env (si retrait de l environnement dans la compilation)
	(void)argc;
	(void)argv;
	char *line;
	t_cmd *cmd;
	char **blocks;

	line = NULL;
	while (1)
	{
		signal(SIGQUIT, SIG_IGN);
		line = readline(" ~ ");
		if (!line)
		{
			ft_putstr_fd("no line quit \n", 2);
			break;
		}
		printf("input : %s\n", line);
		// TODO gerer les spaces : segfault et mettre le prompt apres return
		// TODO gerer return et remettre prompt
		// TODO gerer signaux CTRL +D segfault et CTRL +C dt relaunch nouveau prompt donc continuer la while 1
		// TODO gerer tab quand on click 1 fois cela ne doit rien faire apres pls fois cela affiche automatiquement Display all 3258 possibilities? (y or n)

		// TOTO INVALIDER LE PIPE SI echo '        |         ' ou "            | " -> ce doit etre une chaine de caracteres
		//  ou si "ls | grep c" ->command not found :  gerer le bloc comme si c etait une commande qui n existe pas le PIPE est aussi INVALIDE
		//  si 'ls | grep c' idem

		// TODO gerer "blabla" ->command not found dans le execve $"blabla"
		// TODO : dequote les blocks et les envoyer comme des chaines de caractere. par ex. si "wc -l" ->l envoyer tel une chaine de caracteres, cela va envoyer a arg_and_cmds[wc -l] ce qui n executera pas la command ce sera execcve qui va s en charger et dire que command not found

		blocks = ft_split(line, '|'); // quand je split meme sil n y a pas de pipe je recupere qd meme 1 block . par ex "ls llaaaa"

		//
		// TODO : gerer les cas ou il n y a pas de PIPE existant : "ls "

		cmd = ft_struct_init(&cmd, 0, blocks);
		cmd->path_tab = ft_get_path(envp);
		ft_split_line_in_s_cmd(cmd, line, envp);
		ft_setting_redirections_and_pipes(cmd, envp);
		printf("after setting redir \n");
		ft_free_struct_str(&line);
		ft_free_struct_t_cmd(&cmd);
		ft_void();
	}

	// ft_free_tab(&(cmd->path_tab));
	// ft_free_tab(&(cmd->blocks));
	ft_free_struct_t_cmd(&cmd);
	return (0);
}
