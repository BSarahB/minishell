
#include "minishell.h"
/*on va d abord cheker la validite de la line input du user : line NULL? -> n existe pas (readine NULL)
															line VIDE? -> existe, malloquee peut etre, dedans il y a '\0'
															line only_whitespaces : que des espaces . ->nouveau prompt
															line commence par # -> on ignore toute la line  ->nouveau prompt
															line est  ':' ->nouveau prompt (ou : : : ) les : separes ppar un ou plusieurs espaces
															line est '!'  ->nouveau prompt

                                                            */

int		ft_strcmp(char *s1, char *s2)
{
	int i;

	i = 0;
	while(s1[i] != '\0' && s2[i] != '\0')
	{
		if(s1[i] != s2[i])
			return	(s1[i] - s2[i]);
		i++;
	}
	return (s1[i] - s2[i]);
}


int		ft_is_check_for_token(t_list *lst_token, char *str)
{
	t_list *tmp;

	tmp = lst_token;
	while(tmp)
	{
		
		if(ft_strcmp(tmp->content, str) == 0)
			return (1);
		tmp = tmp->next;
	}
	return(0);
}

int ft_check_double_points_token(t_list *tmp)
{
		if(tmp->next)
		{
			if (ft_is_check_for_token(tmp->next, "|") == 1)
				return(0);//TO DO on va exec la commande en attente
			else
					return (-12);	//pour retourner ensuite 0 apres transformation
		}
		if(tmp->prev)
		{
			if(ft_strcmp(tmp->prev->content, "!@") == 0)
				{
					ft_error("minishell: !@: event not found\n");
					return(-12);
				}
			if(ft_strcmp(tmp->prev->content, "|") != 0 && ft_strcmp(tmp->prev->content, "!") != 0) //&& (ft_strcmp(tmp->prev->content, "!") != 0))
				return(0);
			
		}
		return(-12);//pour return 0 je mets -12 
}



int ft_pipe(t_list *tmp)
{
	if(ft_strcmp(tmp->content, "|") == 0)
	{
		if(tmp->next == NULL)
		{
			ft_error_msg2b(tmp->content);
			return(2);
		}
		if((ft_strcmp(tmp->next->content, "|") == 0))
		{
			ft_error_msg2c(tmp->content);
			return(2);
		}
		if(tmp->next)
		{
			ft_error_msg2b(tmp->content);
			return(2);
		}
	}
	return(0);
}



int ft_chevron(t_list *tmp)
{
	if((ft_strcmp(tmp->content, "<") == 0) || (ft_strcmp(tmp->content, ">")) == 0 || (ft_strcmp(tmp->content, "<<") == 0) || (ft_strcmp(tmp->content, ">>") == 0))
	{
		if(tmp->next == NULL)
		{
			ft_error_msg2("`newline'");
			return(2);
		}
		if (ft_strcmp(tmp->next->content, "|") == 0)
		{
			ft_error_msg2b(tmp->next->content);
			return(2);
		}
		if((ft_strcmp(tmp->next->content, "<") == 0) || (ft_strcmp(tmp->next->content, ">")) == 0 || (ft_strcmp(tmp->next->content, "<<") == 0) || (ft_strcmp(tmp->next->content, ">>") == 0))
		{
			ft_error_msg2b(tmp->next->content);
			return(2);
		}
	}
	return(0);
}



int 	ft_exclamation(t_list *tmp)
{
	if((ft_strcmp(tmp->content, "!") == 0))
	{
		if(tmp->next == NULL)
			return(1);
	}
	return(0);
}



int		ft_check_bash_syntax_error_caracteres_volee(t_list *lst_token)
{
	t_list *tmp;

	if(lst_token == NULL)
		return(-13);//ici il ne faut pas interferer car on est censes renvoyer le echo $? precedentdonc ne rien venir modifier si -13
	if(*lst_token->content == '#') //cas de l entree
		return(-12);//echo $? 0
	tmp = lst_token;

	while(tmp)
	{
		if(ft_exclamation(tmp))
			return(1);
		if(ft_chevron(tmp))
			return(2);
		if(ft_pipe(tmp))
			return(2);
		if((ft_strcmp(tmp->content, ":") == 0))
			return(ft_check_double_points_token(tmp));
		if((ft_strcmp(tmp->content, "#") == 0))
			return(2);//TODO ft_modify_lst_token()
		tmp = tmp->next;
	}
	return(0);

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