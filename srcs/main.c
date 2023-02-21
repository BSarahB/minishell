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
TODO :refaire les canalisations et regler le unconditional move.
note personnelle : je ne respecte pas le pseudo code a la lettre, puisque j ai d abord fait l execution. je vais regler les derniers details et passer au parsing ensuite. 


-> valgrind --suppressions=ignore_rl_leaks --leak-check=full --show-reachable=yes --track-fds=yes ./my_minishell


*/

#include "minishell.h"


int main(int argc, char *argv[], char *envp[])
{
	//TODO gerer le -env (si retrait de l environnement dans la compilation)
	(void)argc;
	(void)argv;
	char	*line;
	t_cmd	*cmd;
	char 	**blocks;

	line = NULL;
	//while (1)
	//{
		line = readline(" ~ ");
		printf("input : %s\n", line);
		//TODO gerer les spaces : segfault et mettre le prompt apres return
		//TODO gerer return et remettre prompt
		//TODO gerer signaux CTRL +D segfault et CTRL +C dt relaunch nouveau prompt donc continuer la while 1
		//TODO gerer tab quand on click 1 fois cela ne doit rien faire apres pls fois cela affiche automatiquement Display all 3258 possibilities? (y or n)


		

		//TOTO INVALIDER LE PIPE SI echo '        |         ' ou "            | " -> ce doit etre une chaine de caracteres
		// ou si "ls | grep c" ->command not found :  gerer le bloc comme si c etait une commande qui n existe pas le PIPE est aussi INVALIDE
		// si 'ls | grep c' idem 

		//TODO gerer "blabla" ->command not found dans le execve $"blabla"
		//TODO : dequote les blocks et les envoyer comme des chaines de caractere. par ex. si "wc -l" ->l envoyer tel une chaine de caracteres, cela va envoyer a arg_and_cmds[wc -l] ce qui n executera pas la command ce sera execcve qui va s en charger et dire que command not found
	
		blocks = ft_split(line, '|'); //quand je split meme sil n y a pas de pipe je recupere qd meme 1 block . par ex "ls llaaaa"


//		
		//TODO : gerer les cas ou il n y a pas de PIPE existant : "ls "

		cmd = ft_struct_init(&cmd, 0, blocks);
		cmd->path_tab = ft_get_path(envp);
		ft_split_line_in_s_cmd(cmd, line, envp);
		ft_setting_redirections_and_pipes(cmd, envp);

	//}


	ft_free_struct_str(&line);
	//ft_free_tab(&(cmd->path_tab));
	//ft_free_tab(&(cmd->blocks));
	ft_free_struct_t_cmd(&cmd);
	return (0);

}
