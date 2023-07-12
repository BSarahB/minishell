
#include "minishell.h"
/*on va d abord cheker la validite de la line input du user : line NULL? -> n existe pas (readine NULL)
															line VIDE? -> existe, malloquee peut etre, dedans il y a '\0'
															line only_whitespaces : que des espaces . ->nouveau prompt
															line commence par # -> on ignore toute la line  ->nouveau prompt
															line est  ':' ->nouveau prompt (ou : : : ) les : separes ppar un ou plusieurs espaces
															line est '!'  ->nouveau prompt

                                                            */
void	ft_check_bash_syntax_error_caracteres_volee(char *line)
{
	(void)line;
	//ft_syntax_error_newline



	//printf("check_bash_syntax_error_newline : $> > \n $> < \n $> << \n $> >> \n <>");//le token oiginal est comme newline< jusqu a x3 <LESSLESS><LESS>

	//ft_syntax_error_token





	//printf("check_bash_syntax_error_token : $> > \n $> < \n $> << \n $> >> \n <>");// > x3, a partir de <GREATGREAT> <GREAT> et a partir de <LESSLESS><LESSLESS> on ne garde que le token en retenue soit  << soit >> soit | soit ||
	//attention tous les motifs // \\ etc sont envoyes comme des commandes a l executio : cmd not found

}

void	ft_check_input_cases_for_return_empty_prompt(char *line)
{
	//ici on doit analyser les tokens 
	(void)line;
	//ft_check_comment_line //le # doit etre un caractere qui demande de ne plus mettre de tokens et d ignorer la suite considerons le comme un operand ignore il clot immediatement la liste chainee#define IGNORE 13 par contre, si on est en mode quoting SINGLE ou DOUBLE QUOTIng # est au sns litteral il perd son pouvoir de caractere special donc ATTENTION



//	printf("check_input_cases_for_return_empty_prompt : $> # \n"); ->il s agira soit du premiere caractere de line, soit d un debut de token $            #ls
//a savoir, egalement, tout ce quil y a avant # sexecute normalement. $ls -la#| wc -l  --> le ls -la va s executer mais pas ce quil y a derriere #


	//ft_check_double_point
	//ICI SI le token seul de la liste chainee est (car on veut gerer $>            :) est <:> ou <!> ou on return prompt empty 



	//printf("check_input_cases_for_return_empty_prompt : $>: (double point) $> :something  $>: espace anything\n"); //ATTENTION: $>: seul aff prompt  $>: espace quoiquece soit aff prompt    $>:cmdcollee    on a cmd not found   $>: : : : aff prompt
	//printf("check_input_cases_for_return_empty_prompt : $>! (seul)\n");//attention une commande apres ou colle
	//
	//
}

void    ft_check_prerequesite_of_line_input(char *line)
{
	//PREREQUESTITE N implique pas l extraction du token. les autres cas de motifs impliquent  
	(void)line;



	//ft_check_NULL_line PREREQUESITE

	

	//printf("check validity of line input : $> NULL readline \n");//soit line == NULL
	//ft_check_only_spaces_line PREREQUESITE


	//printf("check_input_cases_for_return_empty_prompt : $> [que des espaces] ou [que des tabulations] (only spaces)\n");
	//ft_check_empty_line PREREQUESITE



	// printf("check_input_cases_for_return_empty_prompt : $> entree \n");//soit line == "\0" -->return empty prompt

//ft_check_comment_line //le # doit etre un caractere qui demande de ne plus mettre de tokens et d ignorer la suite considerons le comme un operand ignore il clot immediatement la liste chainee#define IGNORE 13 par contre, si on est en mode quoting SINGLE ou DOUBLE QUOTIng # est au sns litteral il perd son pouvoir de caractere special donc ATTENTION
//ici on check seulement le 1 er carcatere de line . on laissera si $                      # gerer les erreurs de token plus tard
}
