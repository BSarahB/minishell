#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h> //pour open les 3 directives de preprocesseur
#include <sys/stat.h>
#include <fcntl.h> //pr pipe
#include <errno.h>
#include <sys/wait.h> //pour le waitpid
#include <signal.h>

//les define pour le type de token : 
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

//les enum pour la fonction du token, est ce un operator, une command ou une redirection?
enum e_function
{
	command,
	metacharacter,
	operator,
	redirection,
	//necessaire args? et option? je ne pense pas
	//control operator :A token that performs a control function. It is a newline or one of the following: ‘||’, ‘&&’, ‘&’, ‘;’, ‘;;’, ‘;&’, ‘;;&’, ‘|’, ‘|&’, ‘(’, or ‘)’.
			//metacharacter A character that, when unquoted, separates words. A metacharacter is a space, tab, newline, or one of the following characters: ‘|’, ‘&’, ‘;’, ‘(’, ‘)’, ‘<’, or ‘>’.
			//operator A control operator or a redirection operator.(regarder la definition juste au dessus) See Redirections, for a list of redirection operators. Operators contain at least one unquoted metacharacter.
//TODO decider si je choisis metacarachter ou operator comme nom 
	//pour le pipe
	//token A sequence of characters considered a single unit by the shell. It is either a word or an operator.
	
};

//description dun token qui fera partie de la liste chainee de tokens,
//je choisis une liste chainee car split sur whitespaces et recuperer un double tab ne me permet pas de retravailler les strings facilement , la liste chainee est plus flexible. 
//par ailleurs pour determiner les caracteristiques de chaque token j ai besoin de definir plsueiurs variables, et c est dans la liste chainee (structure) que je vais pouvoir stocker ces infomrations et y acceder



typedef struct s_list
{
	char 	*content;
	int		position;
	int 	type;
	int 	function;
	int 	quoting_rule;// whitwspace_separator 0 ,single quote
	int		retokenize_allowed;//pour trim and clear et retokenizer un expand par ex/ 
	struct	s_list *next;
	struct 	s_list *previous;

} t_list;


// description dune simple commande et ses arguments


typedef struct s_simpleCmd
{

	int number_of_arguments;
	int errnum;
	char **cmd_and_args;
	char **abs_cmd_and_args;

} t_simpleCmd;

// description d une commande complete avec les multiples pipes eventuels et les IO redirections eventuels

typedef struct s_cmd
{

	size_t 		nb_of_simpleCmds;
	t_simpleCmd **simpleCmds;

	char 		**path_tab;
	char		**blocks;
	size_t		nb_of_blocks;

	char		 *outfile; //-> sera rempli par le token de redirection
	char 		*inputfile;
	char 		*errfile;
	// a voir si on met tous les char * dans un double tab io_redirections(**)
	int 		background;
	t_list		 *lst_token; // notre liste chainee de tokens

} t_cmd;

// t_cmd *currentCmd;

// exec

int ft_split_line_in_s_cmd(t_cmd *cmd, char *line, char **envp);
char **ft_get_abs_arguments(int i, char **blocks);

size_t ft_words_nbr(const char *s, char c);

char **ft_create_tab(char const *s, char c, size_t words_nbr);
char **ft_split(char const *s, char c);
char *ft_strcpy(char *dst, char const *src);
char *ft_init_string(size_t len);
char *ft_strcat(char *dest, char const *src);
char *ft_strjoin(char *s1, char const *s2);
char *ft_strndup(char *src, int n);
char *ft_update_string(char **str, char *new);

void ft_free_struct_str(char **p);
void ft_free_tab(char ***tab);
void ft_free_struct_t_cmd_only(t_cmd **cmd);
void ft_free_struct_t_cmd(t_cmd **cmd);
void ft_free_struct_t_simpleCmd(t_simpleCmd **simpleCmd);

// void	ft_error_msg(char **argv);
// void	ft_error(char *const str);
void ft_putstr_fd(char *s, int fd);
// int		ft_check_close_error(int fd);
// int		ft_check_open_error(int fdin, int fdout);

t_cmd		*ft_struct_init(t_cmd **cmd, char init_value, char **blocks);
t_simpleCmd *ft_struct2_init(t_simpleCmd **ptr, char init_value);
t_simpleCmd **ft_struct_array_init(t_simpleCmd **ptr, char init_value, size_t simpleCmds_nbr);
char **ft_get_path(char **envp);

int ft_execute_cmd(t_cmd *cmd, int i, char *envp[]);
void ft_setting_redirections_and_pipes(t_cmd *cmd, char *envp[]);

char *ft_init_cstring(char **str, size_t len, char init_value);
char *ft_init_cstring(char **str, size_t len, char init_value);
void *ft_memset(void *b, char c, size_t len);

void ft_insert_argument(char *argument);
void fr_insert_simpleCmd(t_simpleCmd *simpleCmd);

// lexer

void 	ft_check_prerequesite_of_line_input(char *line);
void	ft_check_input_cases_for_return_empty_prompt(char *line);
void	ft_check_bash_syntax_error_caracteres_volee(char *line);


char **ft_tokenize_line(char *line);

#endif
