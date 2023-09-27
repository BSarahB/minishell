
/*
 ************************************************************PSEUDO CODE MINISHELL***************************************************************


exemple pr gestion de commande : ls -la a* | grep c | head -n 5 | wc -l > outfile < infile  (pas besoin de gerer * wildcards)


Grammaire shell dans la forme Backus-Naur et
regles de grammaire et labels du Parser (3)  :

cmd[arg]* [| cmd[arg]*]*		[[> filename] [< filename] [>& filename] [>> filename] [>>& filename]]*			 [&]
[cmd_and_args]+[arg_list]			[io_m]	+	[io_m]	+	[io_modifier]	+	io_m   	+	io_m
  == [pipe_list]					==						[io_modifier_list]								 == background_opitonal

les elements decrits ci dessus sont dits "lablels"
src = write your own Ms hands on approch

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



Je reflechis au cas particulier de $VAR apres  export VAR="ls -la" ->  fonctionne execute la commande ls et met en option -la
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

											RULE:en appel simple $VAR->    1 SUBSTITUT d EXPAND quelque soit sa quoting rule va subir le trim and clear  +retokenize (d ou le cmd not found du 1 er element du subtitut) (si plusieurs espaces sont devant le dq on laissera 1 seul espace) MAIS PAS DE DEQUOTE(verifier si c est le cas pour tout)
											AVEC une CMD en tete le SUBSTITUT D EXPAND sera un arg ou option
											SI ECHO est la main cmd PAR CONTRE ON AURA LE SUBSTITUT EN BLOC , LE SUBSTITUT DE L EXPAND na pas de retokenization.
											RULE : ds 1 SUBSTITUT D EXPAND un pipe n a pas valeur de pipe, il est considere comme une [cmd] ou un [arg]
											exemples: export VAR="   5   esp   " et export VAR="\"   5   esp   \""
													: export VAR="   ls    -la" va bien fonctionner cela verifie le trim and clear
													puis on verifie que la cmd and args a bien lieu en introduisant une erreur :
													: export VAR="   ls    -jel" ->ls invalid option --j verifie la retokenisation
													:export VAR="jj -la" jj:command not found
													:export VAR="ld     -la" -la no such file or directory



											je continue de voir des exemples. je dois bien savoir quel est l ordre avant de commencer a ecrire le lexing
											export VAR="wc"
											ls | $VAR -l fonctionne tres bien. $VAR est substituee comme un wc COMMANDE
											!! apres un operateur | on considere que l expand qui suit est une commande.
											voyons si elle se complique
											export VAR="head -n 5"

											CEPENDANT quand la commande ECHO est la curr simple_cmd, alors, il faudra mettre l expand $VAR en tant qu option de la commande. donc LE SUBSTITUT de l expand est etokenize.

											on va donc determiner si simple_cmd est echo ou pas
echo -nnnn -nnn -n abc -> execve se charge de considerer -nnnn comme une option valide
echo -nnn -nnn abc -n -nna -naaa -nn -nn -nnn -nnnn -jfkefe kleflkfneife  CHOIX de la data structure : sera une liste chainee pour ne pas etre piege par le nombre d arguments , ni le nbr de commandes, et ne pas avoir besoin de les allouer au depart en etant bloque sur le nombre   ls -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l fonctionne tres bien dans bash, il faudra tout de meme mettre une limite je pense...?


EXPAND CASE:

ATTENTION $VAR$ -> en mode no quoting et whitespaces_separatorle $ colle a la variable ANNULE le trim and clear

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


ATTENTION $VAR$ -> le $ colle a la variable ANNULE le trim and clear et le retokenize egalement LA SUBSITUTION A QUAND MEME LIEU
ATTENTION AUX CAS PARTICULIERS A GERER: echo $VAR$ -> on ne trime &clear pas VAR. je ne sais pas pkoi. VAR etant suivi d un Caractere $ n est pas trime et clear.
										par ex VAR="   5   esp   "



										echo $VAR$ :   5   esp   $ --> le $ TRIM&CLEAR CANCELLED le RETOKENIZE CANCELLED AUSSI la substitution a bien lieu
plusieurs comportements					echo $VAR+
												 -
												 !
												 ?
												 [
												 ]
												 {
												 }
												 /
												 =
												 ~
												 ^
												 .
												 ,
												 %
												 @
												 # -->OK tous sont OK AUTHORIZE THE EXPAND ils respectent le TRIM&CLEAR , mettent 1 espace pour separer le prochain token +-!?[]{}

												**********
												caracteres_char ou nombres --> newline vide, l'expand de VARcaracteres_char n existe pas donc jute un \n saut de ligne vide
												_                --> newline vide, idem que pour caracteres_char

												**********
												\ ou | pipe ou ' ou "   --> $> mettent le prompt sur une nouvelle ligne en attente de lecture du processus

												**********
												$ -->annule le TRIM and CLear et annule le retokenize. on a un seul bloc : substitut$
												**********
												(
												)
												>
												<  -> pour ces 4 derniers caracteres:  bash: syntax error near unexpected token `newline'(pour le >, < ou >> )  ou `('

												*****
												& --> JE NE SAIS PAS

												NOTA BENE: tous les caracteres ci dessus dont ceux qui mettent un nouveau prompt ou annulent le trim ou provoquent des bash syntax error
												entre double quotes ou single quotes perdent tte valeur speciale, et ont qu un caractere litteral
															ex: echo "$'"  ->$'

dans le cas ou un expand n existe pas LETOKEN <EXPAND> EST SUPPRIME si il est une option

PROCEDURE:
<0> -> separation with whitepaces on fait les <token> : DELIMIT TOKEN
1   -> QUOTING RULES : 	DOUBLE_QUOTE? get position start quote end quote
						SINGLE_QUOTE? idem
						WHITESPACES_SEPARTOR_ENABLED or WHITESPACES_LITTERAL

	->EXPAND HERE?: yes/no
	->EXPAND AUTHORIZED : yes/no
			-> EXPAND EXISTS : yes/no (no == supprimer la partie EXPAND car token invalide : suppression du token de la liste chainee ou removal de sa partie)
				->EXPAND

Quoting rules applied: litteral value protected or clear? SQ rules DQ rules WSPaces_SEPARATOR rule->
2	-> WHITESPACES_SEPARATOR_ENABLED: TRIM&CLEAR
	->or WITESPACES_LITTERAL and CHAR_LITTERAL(' | etc ...) : inside DQ and SG bloc
3   -> QUOTE REMOVAL :yes/no



											echo + token-QUOTE vide , apres removal quote on a 0 mais le token est VALIDE:

											echo "" et echo ''  -> on prend le token <''> ou <"">  normalement
											on lui applique le mode quoting rules et a l etape de quote removal on lui retire les quotes.
											le token est valide : cela signifie que si ce token '' est suivi d un autre token comme echo "" bonjour result :  > bonjour -> un espace est la pour signifier que le token '' etait valide. et que nous aviosn bien une separation entre le token <''> et <bonjour>
											echo ''bonjour ->bonjour
											echo '' bonjour-> bonjour
											echo ''''''''bonjour->bonjour(on quote removal simplement le token <''''''''bonjour>)
											echo '' '' '' '' bonjour->    bonjour (4 espaces et bonjour) (1 espace pour une separation entre tokens valides)
											echo '' '' '' |cat -e ->   $ on a 3 espaces pour materialiser 3 separations de tokens valides meme si vides

											echo '' ou "" est similaire a echo "$VARE"-> <"$VARE"> -> <""> car lexpand n existe pas donc on le supprime. Quote removal-> <\0> et echo \0 fait comme echo "" ou echo '' -> le result est une newline vide

											EN REVANCHE :
											echo + token-EXPAND qui n existe pas, le token n est PAS VALIDE et sera supprime de la liste chainee. avant d etre envoye faire son parsing et remplir la data strcuture
											echo $VARA $VARA $VAR | cat -e
										  ->5 esp$   la $VAR est trim$clear les 2 token sont supprimes de la liste chainee lorsque VAR n existe pas
											$VARA $VAR
											echo $1 $2 $1$VAR : les 2 expands sont invalides supprimes de la liste chainee. dans le dernier token <$1$VAR> on va supprimer la partie de lexpand non valide et ne laisser que s exprimer $VAR qui sera trim&clear
										  ->5 esp  
											5cmd not found. donc le meme traitement s applique que juste en haut: 1 token- Expand qui n existe pas est supprime de la liste chainee.


savoir si on met le token a 0.


gerer le cas des quotes entremellees : 		echo "'$VAR'"        echo '"$VAR"'
c est la regle du first quoting rule found is equal to rule applied. le quote removal s appliquera seulement sur les quotes du quoting rule
											>'   5   esp   '	>"$VAR"		(pr le 2 eme exemple l expand n est pas authorized)




POUR DETERMINER LES FONCTIONS:
IL FAUDRA SAVOIR QUELS SONT LES CRITERES pour determiner le moment ou on decide de remplir le tableau d arguments.
il faut savoir comment identifier la commande et determiner ss quelles conditions on commence a remplir ses options ou arguments
IL faudra determiner les limites d un expand. aaa"$V"AR l expand s arrete au " . $VAR$ l expand s arrete au $ ou a un \0 .
$VARA n est pas valide. car l expand s arrete au \0 et englobe le A

*********************************GESTION DES COMPORTEMENTS*****************************************


VOICI TOUS LES TYPES DE TOKEN : on eut creer des define:
								WORD 1
								VARIABLE $VAR 2
								PIPE  3
								GREAT 4
								GREATGREAT 5
								LESS 6
								LESSLESS 7
								TERMINATE 8
								PARENTHESIS_LEFT 9
								PARENTHESIS_RIGHT 10
								AMPERSAND & 11
								GREAT_AND_AMPERSAND >& 12
								LESSGREAT <> 13


on pourrait peut etre aussi creer des define pour les returns
								RAS :											0
								Export /unset not a valid identifier :			1 export event not found , cd too many argumetns

								bash syntax error near unexpected token:		2  filename argument required , export/pwd/cd invalid option,
								Is a directory :								126
								command not found :								127
								no such file or directory						parfois 1 , 127 possible(Unset path + ls)

								RETURNS SUR LES SIGNAUX 						0/130/131



faire la difference entre un heredoc operationnel et le << ou >> bash synntax error

TOUT D ABORD:
ft_check_validity_of_line_input
on va d abord cheker la validite de la line input du user : line NULL? -> n existe pas (readine NULL)
															line VIDE? -> existe, malloquee peut etre, dedans il y a '\0'
															line only_whitespaces : que des espaces . ->nouveau prompt
															line commence par # -> on ignore toute la line  ->nouveau prompt
															line est  ':' ->nouveau prompt (ou : : : ) les : separes ppar un ou plusieurs espaces
															line est '!'  ->nnouveau prompt







LEXER:

ft_check_validity_of_quoting_rules_for_line_  : prendre en charge les guillemets et aostrophes incomplets ouverts (nombre impair)
			fr fonction qui des qu elle met la regle de quoting a 2 la remet a  0  et met a 1a regle de whitespace_separator
			si c est bon, pas de problem de quoting -> alors on peut tokenizer la line en faisant split sur les espaces
			ft_get_quoting_rules : evaluer quand le faire
			stocker dans chaque token le debut de la quote start_SINGLEQUOTE end_SINGLEQUOTE ou s_DQ e_DQ comme il peut y avoir plusieurs quoting successifs dans un token. sils sont entremeles, c est le premier quoting qui l emporte
			dans notre minishell on part quand meme du principe que les quotes seront fermees mais gerer les segfaults tt de meme attentionle progrmme ne doit pas crasher
TOKENISER la line en fonction des WHITESPACES SPEARATEURS ou des OPERANDS : | < > qui servent de delimitateurs aux tokens
ft_create_token devra retourner la liste chainee de tokens crees
ft_check_syntax_error(token)\
ft_check_operand_syntax_error -> ft_case_pipe par ex le | en debut de cmd line fait une syntax error,
ft_define_token_type



****AFFICHAGE DE NOUVEAU PROMPT:

>entree (\n): affiche nouveau prompt
>que des espaces   : TODO0 :CORRIGER LE SEGFAULT ICI
>que des tabulations : comportement automatique. affiche les fichiers et affiche le prompt TODO : s assurer du bon return
>:		TODO1 le : va dans l execution comme une simple cmd . il ne devrait pas  etre traite comme une simple cmd a executer son ccomportement normal est de ne faire rien et juste d afficher  un nouveu prompt





POSSIBILITE : envisager une fonction qui gere tous les cas particuliers de commandes avant l execve et compare la cmd avec ft strcmp -> strategie trop lourde
ces cas seraient: ft_compare_cmd_and_avoid_execve

****************************************************************GESTION DES COMPORTEMENTS DES CARACTERES A A VOLEE***************************************************************************

>!   : ATTENTION il faudra retourner 1 et mettre 1 nouveau prompt (prevoir une if cmd == ! avec ft strcmp par ex pour gerer les execptions d envoi a execve si je decide d envoyer le token a l execution                                                            ), si  !! envoyer a execve et faire la commande not found

on peut regrouper plutot : les erreurs de syntaxe pres dun token token ou `nouvelle ligne' inattendu ... pkoi newline que cela signifie il ? qu on attendait un fichier? ou un vrai parametre car le token est original ?

****STRATEGIE POUR LE bash: syntax error near unexpected token `newline' --> avec:  >		<		<<		>>		<>     newline quand on a affaire au motif original
			definir:																												 (ne pas oublier le return de  echo $?)

STRATEGIE POUR le bash: syntax error near unexpected token `TOKEN*1 ou *2 ou *3'    -> avec : token  *1 quand  le token est repete  n 1 unique exemplaire plusieurs fois avec des espaces dan la cmd line, ou 																							token *2 quand le motif est repete a partir de 4 fois et + pour GREATGREATGREATGREAT et + et
			laissons tomber pour le moment, les *3. faisons simple. soit on display newline, soit le motif original(! ou < ou <<) oou |

			definir:

on avait vu plus haut que avec echo:			<<
												>>
												(
												)
												>
												<  -> pour ces 6 derniers caracteres:  bash: syntax error near unexpected token `newline'(pour le >, < ou >> )  ou `('
MEME SI ECHO est mal ecrit, ou que la comande est mal ecrite  quelque soit ce qu on a on a : on a un bash syntax error pareil pour les autres tokens : echo <<<<< ou echo < < < <
-> le bash syntax error token << ou token < s applique ici quelque soit la commande ecrite... ecccho < met a meme bash syntax error
cela signifie que le bash regarde avant d executer echo ou n importe quelle autre commande le token en question
ft_check_syntax_error(token)\
ft_check_operand_syntax_error -> ft_case_pipe par ex le | en debut de cmd line fait une syntax error,


si ces tokens sont suivis d un token de type <WORD> alors ils jouent leur role de redirection. infile, outfile heredoc append etc.
donc il faut que le token qui suive notre token syntax error soit un token <TERMINATE>    -> <WORD> <numeros 4,5,6,7,9,10,13> <TERMINATE> --> `newline'

REMARQUE : tous ces tokens, SI ils sont entre DQ ou SQuoting ne constituent plus une erreur de syntax. mais une command not found "<" ou '|'

********************************************MON CODE*************************************************
TODO: cmd not found, mettre en place la suite du debogg de l execve (cf pipex)


SE DEFENDRE lors de la correction si on me met des {} , () , *, \,  



RAPPEL :  difference entre une string NULL qui n existe pas , n a pas d adresse malloc et une string vide qui a un \0 . mais qui peut tres bien avoir une adresse malloquee
*/


COMMENTAIRES TODO dans main.c

	// TODO gerer le -env (si retrait de l environnement dans la compilation)
//	printf("input : %s\n", line);
		// TODO gerer les spaces : segfault et mettre le prompt apres return
		// TODO gerer return et remettre prompt
		// TODO gerer signaux CTRL +D segfault et CTRL +C dt relaunch nouveau prompt donc continuer la while 1
		// TODO gerer tab quand on click 1 fois cela ne doit rien faire apres pls fois cela affiche automatiquement Display all 3258 possibilities? (y or n)

		// TOTO INVALIDER LE PIPE SI echo '        |         ' ou "            | " -> ce doit etre une chaine de caracteres
		//  ou si "ls | grep c" ->command not found :  gerer le bloc comme si c etait une commande qui n existe pas le PIPE est aussi INVALIDE
		//  si 'ls | grep c' idem

		// TODO gerer "blabla" ->command not found dans le execve $"blabla"
		// TODO : dequote les blocks et les envoyer comme des chaines de caractere. par ex. si "wc -l" ->l envoyer tel une chaine de caracteres, cela va envoyer a arg_and_cmds[wc -l] ce qui n executera pas la command ce sera execcve qui va s en charger et dire que command not found
//
		// TODO : gerer les cas ou il n y a pas de PIPE existant : "ls "
// 3 april spent the day having headaches and trying to make right choice and buy tickets. god I havent spent a good day at all. those teeths. anyway I hope to spend a bettet eveneing I need to work this out and start fresh tomorrow is another day. I ll come back here to write down if I was able to be favorite not outsider







                                                   								LEXER.C
//void ft_trim_and_clear(char *line)
// je parcours ma line et en trimant et clearant les espaces je determine chaque <"token> : 1 token est soit separe par un espace, soit separe par un operand ou un metacharacter lui meme separateur de token. par ex |
	// pour que trim and clear je dois etre en mode Whitespace_Separator, sinon .... mon espace n a pas de caractere special de separateur si je suis en quoting rule DQ ou SQ, ceci s applique egalement a tous les operands qui perdent toute valeur speciale
	// donc je parcours char par char et si " ou ' je dois signaler un quoting rule. il faudra etre sorti du quoting rule mode pour pouvoir separer les tokens en fonction des espaces ou des operands
	// je dois aussi determiner un start_token_pos et end_token_pos pour mon token puis je vais faire un ft strdup ou ndup pour dupliquer le token et remplir le content de mon maillon de la liste chainee
	// je dois donc considerer deja etre dans mon premier maillon de liste chainee ici. trouver une condition qui me permet de generer mon premier maillon et je renseignerai donc le quoting rule ici
	// remarque : si j ai ls -la >"outfile" | $VAR q: mon operator > GREAT doit il separer "outfile" ?OUI <>> <"outfile">
	// si je tombe sur un \0 ou en Whitespace_separator rule sur un espace/operand -> c est la fin de mon token --->mon end_token_pos sera recycle pour etre le depart de la recherche du prochain token
	//

	// condition: des que je tombe sur un char je lst_add_new_token ->je ne respecte pas cette donnee ici je cree directement le token
	// sont content sera NULL en attendant que .... on fixe tous les parametres de la structure






DRAFTS to help between functions
	/*main
	//LE LEXING DEVRAIT ETRE FAIT DE MANIERE SEQUENTIELLE car :
		//ON NE VA PAS POUVOIR SPLIT SUR whitespaces car si on a un operateur comme echo b |cat-e on veut que le pipe soit un token a part
		//ANSI le lexing doit se faire de MANIERE SEQUENTIELLE  meme un "    " ca ne fonctionnera pas
//utils
		// gardons cette fonction pour ne pas faire crasher le code en attendant de fr la vraie tokenize line operationnelle qui redistribue les tokens dans la liste chainee directement et pas dans un double tab
	// la liste chainee est vraiment justifiee car on a dans la cadre des expand, besoin de REtokenizer encore une fois avec le TRIM and ClEAR. on doit pouvoir manier les token de maniere flexible. un tableau serait tres galere a modifier et remodifier
// parcourir la line de maniere sequentielle puisque les quoting rules vont determiner les qualites des caracte speciaux et des operands, aussi nous avons besoin de passer les espaces pour trouver un token dans le cas de nos gestion de comportement de caracteres a la volee char on the fly ou motifs
	// ex de <$VAR> et <ls> <|> <$VAR>
	// 1
	// j initialise ma liste chainee en mettant tout a 0
	// ft_list_init(&token_list);
	// en envoyant l adress de token_list, on viendra modifier directement en memoire sa valeur, donc pas bsoin de recuperer la structure token_list a la sortie de la fonction, on peut librement utiliser token_list dans un appel de fonction
	// ft_trim_and_clear(line, token_list);

	// reflechir a la meilleure option entre initialiser des le debut la liste chainee avec le 1 er maillon
	// ou le faire dans la trim and clea
	// copie n bytes dun bloc M de src (sans structuration) a la zone M dst.
// strcpy != memcpy : strcpy : copie --> \0 ou -->segfault  memcpy : copie tt le buffer (s arrete a n bytes)
	/ft_get_token_type : // mettre str pour checker l element precedent TODO : proteger str d un index qui n exste pas
	parser.c :
	/*3)  PARSER/yacc traite shell.y) == shell_parser.c on va essayer de mettre des regles de grammaire et labels qui vont generer la table de commande
labels: cmd_and_args, arg_list ou pipe_list ou io_modifier_list ou background optional etc..
Grammaire shell dans la forme Backus-Naur et
regles de grammaire et labels du Parser (3)  :
cmd[arg]* [| cmd[arg]*]*		[[> filename] [< filename] [>& filename] [>> filename] [>>& filename]]*			 [&]
[cmd_and_args]+[arg_list]			[io_m]	+	[io_m]	+	[io_modifier]	+	io_m   	+	io_m
  == [pipe_list]					==						[io_modifier_list]								 == background_opitonal

les elements decrits ci dessus sont dits "lablels"
ci dessous a quoi ressemble un fichier shell.y
goal: command_list;
arg_list:
		arg_list WORD
		| *empty
			;
cmd_and_args:
		WORD arg_list
			;
pipe_list:
		pipe_list PIPE cmd_and_args
		|cmd_and_args
		;

io_modifier:
		GREATGREAT WORD
		| GREAT WORD
		| GREATGREAGAMPERSAND WORD
		| GREATAMPERSAND WORD
		| LESS WORD
		;

io_modifier_list:
		io_modifier_list io_modifier
		|*empty
		;
background_optional:
		AMPERSAND
		|*empty
		;

command_line:
		pipe_list io_modifier_list background_optional NEWLINE
		| NEWLINE *accept empty command line
		| error NEWLINE{yyerrok}
		;
		*error recovery
command_list:
		command_list command_line
		; *command loop
		*/


/* //											PSEUDO CODE	PARSING_LST_TOKEN
si simpleCmd_nbr > 1

1)j identifie la position du premier operateur qui signifie la fin de la simpleCmd : la end_simpleCmd_pos
ft_parse_lst_token_in_simpleCmds_and_cmd
(utiliser lst_token+i (i sera la position de l operateur...)? ou un lst_token_tmp)


2/je determine le nbre de infile errfile outfile ds  ma simpleCmd (attention si infile appartient a la last_simpleCmd alors il faudra le rattacher a la premiere commande)
ft_count_nb_of_redir_token_in_simpleCmd : {
	ft_count_nb_of_infile_in_simpleCmd
	ft_count_nb_of_outfile_in_simpleCmd
	ft_count_nb_of_errfile_in_simpleCmd
}

3/ft_malloc_redir_file_tabs_of_simpleCmd

4/ft_parse_redir_token_in_simpleCmd
{
	ft_parse_infile_in_simpleCmd
	ft_parse_outfile_in_simpleCmd
	ft_parse_errfile_in_simpleCmd
}
5/ft_delete redir_token_from_lst_token
6/compter le nombre de tokens dans la simpleCmd ft_count_final_nb_of_token_in_simpleCmd

Avec l info de simpleCmds[i]->nb_of_outfile/infile/errfile on pourra malloc les double **tab de redirections. (infile**) (outfile**) (errfile**)
7/mallocs : ft_malloc_cmd_and_args_tab_of_simpleCmd
8/tant que lst_token->position != operator_pos
		ft_parse_token_in_cmd_and_args
		sinon lst->next && i++(pour passer a la simpleCmd suivante)


Si simpleCmd == 1
alors pas de condition pour operator_pos
on y va directement sur les fonctions de parsing
*/
//LIBRAIRIE
==27127==ASan runtime does not come first in initial library list; you should either link runtime to your application or manually preload it with LD_PRELOAD.
est une erreur qui dit quil faut mettre le flag -lasan avant -lreadline
CFLAGS = -Wall -Wextra -Werror -I $(INCLUDE_DIR) -fsanitize=address -fsanitize=leak
Dans Le MAkefile(42)
$(NAME):	$(OBJS)
	$(CC) -o $@ $^ -lasan -lreadline  #-lasan == dynamique == necessite installation sur ordi VS -static-libasan == STATIC == inclus dans l executable ==pas de necessite d installation sur l ordi pour verfier $ldconfig -p  pour lister les librairies de l ordi
	au niveau des extensions :
	static c est .a
	dynamic c est .so (shared object?)
les flags -fsanitize=address -fsanitize=leak sont dans la compilation
et la librairie est dans le linkage des fichiers .o

 #-lasan == dynamique == necessite installation sur ordi VS -static-libasan == STATIC == inclus dans l executable ==pas de necessite d installation sur l ordi pour verfier $ldconfig -p  pour lister les librairies de l ordi


***pseudo code EXPANSION EXPAND***	
pour none et echo : les memes etapes

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

****Note pour 5/ et 6/
NOTE TRES IMPORTANTE :  TOUTE QUOTE ISSUE DE LEXPAND N A AUCUNE VALEUR DE QUOTING RULE ET NE SERA PAS DEQUOTE. SES QUOTES NE PROTEGENT EN RIEN LES ESPACES DE LA RETOKENIZE: SI UN ESPACE SE TROUVE ENTRE DES QUOTES DE L EXPAND ON RETOKENIZE SANS PITIE EN CASSANT LE BLOC. 
CE SONT LES QUOTES DE LA QUOTING RULE (QUI ONT VALEUR DE QR ET QUI PROTEGENT LE CARACTERE LITTERAL)



SE DEFENDRE A LA CORRECTION : ASK
 ls >outfile | wc >outfile
 ~ cat outfile a un comportement indefini. si le correcteur insiste : inverser les cmds : wc >o | ls >o pour lui montrer le mis & match

<infile ls >outfile | <<A <<B wc -l <infile <<C  >outfile  IDEM qu au dessus outfile n a pas ete ecrase par le wc -l qui prendrait normalement le <<C comme infile (inverser les commandes pour tester)



//VERIFIER 
// TODO : comparer quand cat est en deniere simple command cat outfile22 ne marche pas CF PEER DEMANDER CE QUILS EN PENSENT EUX
// build-my_minishell-Desktop_GCC-Debug ls | wc -l | cat outfile22
// 19
//  build-my_minishell-Desktop_GCC-Debug ls | wc -l>outfile22 | cat outfile22
// pas de resultat on a le prompt directement


//TODO as a test
heredoc
<<A bash
>echo bonjour
>exit 12
>A
va display bonjour
et echo $? donner 
12

BUILT IN
~ ~ | ls
~: command not found: on devrait avoir:  bash: /mnt/nfs/homes/mbenmesb: Is a directory (probablement a implementer avec cd)


//NORMEME
void ft_get_token_quoting_rule(char *str, t_list *lst_token, size_t i) //NORMEME >25 lignes
get_token_type.c



//FACTORIZE
get_token_type.c ->il y a un parametre en trop

 //EXPAND
 >$Q >BEBE | ls -> on n aura pas le fichier BEBE cree
 >BEBE >$Q | ls -> BEBE sera cree
 le compprtement d un outfile ambiguous est le meme qu un nofile


 //EXPAND : 
 les " ', issues de l expansion n ont aucune valeur de quoting rule ni de separateur. Pour eviter de dequote une quote de l expansion ou de retokenizer sur un mauvais espace induit en erreur par une mauvaise quoting rule issue de l expansion, il faut absolument recuperer le scope de l expand, et garder en memoire dans la strcuture l index j du buffer attribue lors de la substitution, puis lors de l epur et puis lors de trim -> il faudra update l index au fur et a mesure........
 