
#include "minishell.h"
/*on va d abord cheker la validite de la line input du user : line NULL? -> n existe pas (readine NULL)
															line VIDE? -> existe, malloquee peut etre, dedans il y a '\0'
															line only_whitespaces : que des espaces . ->nouveau prompt
															line commence par # -> on ignore toute la line  ->nouveau prompt
															line est  ':' ->nouveau prompt (ou : : : ) les : separes ppar un ou plusieurs espaces
															line est '!'  ->nnouveau prompt

                                                            */

void	ft_check_bash_syntax_error_caracteres_volee(char *line)
{
	//ft_syntax_error_newline
	printf("check_bash_syntax_error_newline : $> > \n $> < \n $> << \n $> >> \n <>");// > jusqua x2, < jusqu a x3
	printf("check_bash_syntax_error_token : $> > \n $> < \n $> << \n $> >> \n <>");// > x3, 


}


void	ft_check_input_cases_for_return_empty_prompt(char *line)
{
	(void)line;
	//ft_check_empty_line
    printf("check_input_cases_for_return_empty_prompt : $> entree \n");//soit line == "\0"
	//ft_check_comment_line
	printf("check_input_cases_for_return_empty_prompt : $> # \n");
	//ft_check_only_spaces_line
	printf("check_input_cases_for_return_empty_prompt : $> [que des espaces] ou [que des tabulations] (only spaces)\n");
	//ft_check_double_point
	printf("check_input_cases_for_return_empty_prompt : $>: (double point) $> :something  $>: espace anything\n"); //ATTENTION: $>: seul aff prompt  $>: espace quoiquece soit aff prompt    $>:cmdcollee    on a cmd not found   $>: : : : aff prompt  
	printf("check_input_cases_for_return_empty_prompt : $>! (seul)\n")//attention une commande apres ou colle 
}

void    ft_check_validity_of_line_input(char *line)
{
	(void)line;
	//ft_check_NULL_line
	printf("check validity of line input : $> NULL readline \n");//soit line == NULL

}
