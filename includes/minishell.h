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
#include <stddef.h> //pr le size_t


// les define pour le type de token : devrais je choisir un enum comme pour les autres ca sera plus carre et laisser le define pour les echo $?
#define WORD 1
#define VARIABLE 2
#define PIPE 3
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
// LESSGREAT 14 ?

// les enum pour la fonction du token, est ce un operator, une command ou une redirection?
enum e_title // a renommer en function
{
	operator,
	command,
	main_command,
	option,
	redir_in,
	redir_out,
	redir_err, // TODO determiner stderr dans le token type
	redir_append,
	redir_heredoc,
	metacharacter,
	// necessaire args? et option? je ne pense pas
	// control operator :A token that performs a control function. It is a newline or one of the following: ‘||’, ‘&&’, ‘&’, ‘;’, ‘;;’, ‘;&’, ‘;;&’, ‘|’, ‘|&’, ‘(’, or ‘)’.
	// metacharacter A character that, when unquoted, separates words. A metacharacter is a space, tab, newline, or one of the following characters: ‘|’, ‘&’, ‘;’, ‘(’, ‘)’, ‘<’, or ‘>’.
	// operator A control operator or a redirection operator.(regarder la definition juste au dessus) See Redirections, for a list of redirection operators. Operators contain at least one unquoted metacharacter.
	// TODO decider si je choisis metacarachter ou operator comme nom
	// pour le pipe
	// token A sequence of characters considered a single unit by the shell. It is either a word or an operator.

};

enum e_quoting_rule
{
	whitespace_separator,
	single_quote,
	double_quote,
};

// description dun token qui fera partie de la liste chainee de tokens,
// je choisis une liste chainee car split sur whitespaces et recuperer un double tab ne me permet pas de retravailler les strings facilement , la liste chainee est plus flexible.
// par ailleurs pour determiner les caracteristiques de chaque token j ai besoin de definir plsueiurs variables, et c est dans la liste chainee (structure) que je vais pouvoir stocker ces infomrations et y acceder

typedef struct s_settings
{
	int 	fdin;
	int 	fdout;
	size_t 	i;
	size_t 	j;
	int		exit_status;
	int		savein;
	int 	saveout;
	
} t_settings;


typedef struct s_settings_del
{
//	int 	fdin;

	size_t 	i;
	size_t 	j;
	size_t	k;
	size_t index;

} t_settings_del;


typedef struct s_list
{
	char *content;
	size_t position;
	int type;
	int title;
	int quoting_rule;		// whitwspace_separator 0 ,single quote 1, d_quote 2
	int retokenize_allowed;// pour trim and clear et retokenizer un expand par ex/
	int expand_exists;
	struct s_list *next;
	struct s_list *prev;
	size_t start_token_pos;
	size_t start_token_pos_exists;
	size_t end_token_pos;
	size_t quoting_rule_adequate;
	int	tag_expand;

} t_list;

typedef struct s_data
{
	t_list *token;	   // lst_token
	t_list *lst_token; // lst
} t_data;

// description dune simple commande et ses arguments

typedef struct s_simpleCmd
{
	size_t nb_of_tokens_in_simpleCmd;
	size_t nb_of_redir_token;
	size_t nb_of_heredoc;
	size_t k;
	size_t nb_of_infile;
	size_t nb_of_outfile;
	size_t nb_of_errfile;
	size_t first_redir;
	size_t nb_of_outfile_before_nofile;
	size_t nb_of_errfile_before_nofile;
	size_t nofile;
	int number_of_arguments;
	int errnum;
	char **cmd_and_args;
	char **abs_cmd_and_args;
	char **infile;
	char **heredoc;
	char **outfile;
	char **errfile;
	int	 *append_track_index;
	int		*heredoc_track_index;
	size_t end_simpleCmd_pos;
} t_simpleCmd;

// description d une commande complete avec les multiples pipes eventuels et les IO redirections eventuels

typedef struct s_cmd
{
	size_t nb_of_simpleCmds;
	t_simpleCmd **simpleCmds;

	char 	**path_tab;
	//	char		**blocks; //original
	//	size_t		nb_of_blocks;//original

	char 	*outputfile;
	char	*inputfile;
	char	*errfile;
	int		*heredocs_track_index;
	char 	**heredocs;
	size_t k;
	size_t  nb_of_heredocs;
	// a voir si on met tous les char * dans un double tab io_redirections(**)
	int 	background;
	t_list 	*lst_token; // notre liste chainee de tokens
	int 	flag_head_list;
	size_t nb_of_infile;
	size_t nb_of_outfile;
	size_t nb_of_errfile;
	int		line_count;

} t_cmd;

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



void	ft_free(t_cmd *cmd, t_list *lst_token, t_data *data, char *line);
void ft_free_struct_str(char **p);
void ft_free_tab(char ***tab);
void ft_free_struct_t_cmd_only(t_cmd **cmd);
void ft_free_struct_t_cmd(t_cmd **cmd);
void ft_free_struct_t_simpleCmd(t_simpleCmd **simpleCmd);

void    ft_free_struct_t_data(t_data **data);
void    ft_free_struct_t_list_token(t_list **token);
void    ft_free_struct_t_settings(t_settings **set);
void    ft_free_struct_t_settings_del(t_settings_del **del);
void	ft_free_struct_t_list_lst_token(t_list **lst_token);


// void	ft_error_msg(char **argv);
// void	ft_error(char *const str);
void ft_putstr_fd(char *s, int fd);
// int		ft_check_close_error(int fd);
// int		ft_check_open_error(int fdin, int fdout);

t_cmd *ft_struct_init(t_cmd **cmd, char init_value, char **blocks); // original

// replacements of original functions for parsing lst token to data strcture
t_cmd *ft_struct_init_cmd(t_cmd **cmd, char init_value, t_list *lst_token);
t_data *ft_tokenize_line(char *line);
//t_list *ft_tokenize_line_b(char *line); // void pour commencer les tests
size_t ft_tokenize(char *str, char *line, size_t i, t_data *data);

int ft_split_tokens_in_s_cmd(t_cmd *cmd, char *line, char **envp, t_list *lst_token);
// int	ft_split_line_in_s_cmd(t_cmd *cmd, char *line, char **envp);

// void	ft_tokenize_line_to_lst(char *line);//original

t_simpleCmd *ft_struct_init_simpleCmd(t_simpleCmd **ptr, char init_value);
t_simpleCmd **ft_struct_array_init(t_simpleCmd **ptr, char init_value, size_t simpleCmds_nbr);
char **ft_get_path(char **envp);

int ft_execute_cmd(t_cmd *cmd, int i, char *envp[], t_settings *set);
int ft_setting_redirections_and_pipes(t_cmd *cmd, char *envp[], t_data *data, t_list *lst_token, char *line);

char *ft_init_cstring(char **str, size_t len, char init_value);
void *ft_memset(void *b, char c, size_t len);

void ft_insert_argument(char *argument);//TOREMOV
void fr_insert_simpleCmd(t_simpleCmd *simpleCmd);//TOREMOV

// lexer

void ft_check_prerequesite_of_line_input(char *line);
void ft_check_input_cases_for_return_empty_prompt(char *line);
int ft_check_bash_syntax_error_caracteres_volee(t_list *lst_token);

void ft_get_token_quoting_rule(char *str, t_list *lst_token, size_t i);
void ft_get_token_content(t_data *data, size_t start_token_pos, size_t end_token_pos, char *line);
// char	**ft_tokenize_line(char *line);
t_data *ft_tokenize_line(char *line); // void pour commencer les tests
char **ft_tokenize_line1(char *line);

t_list *ft_lstlast(t_list *lst);
void ft_lstadd_back(t_list **alst, t_list *new);
t_list *ft_lstnew_for_lst(t_data *data);
t_list *ft_lstnew_data_token(char *content);
t_list *ft_create_list(void);

size_t ft_char_is_whitespace(char *str, char *line, size_t i, t_data *data);
int ft_char_isnull_no_qr(char *line, size_t i, t_data *data);
int ft_get_token_type(char *str, t_list *token, t_data *data, size_t i, char *line);
size_t ft_char_is_operator(char *line, size_t i, t_data *data);
void ft_char_after_ws_token_exist_no_qr(char *line, size_t i, t_data *data);
size_t ft_char_after_ws_is_operator_no_token_exists_no_qr(size_t i, char *line, t_data *data);

void ft_char_or_token_is_unique(char *line, size_t i, t_data *data); // char or token is unique // faudrait  il recuperer ici la end pos puisqu on sort du code des token et qu on a atteint un \0 ?
void ft_char_after_ws_isnull_token_exists_noqr(char *line, size_t i, t_data *data);

int ft_is_char_operand(char *str, t_list *lst_token);
void ft_get_token_content_lengh_for_malloc(t_list *token, size_t start_token_pos, size_t end_token_pos);

void *ft_memcpy(void *dst, const void *src, size_t n);
void *ft_memset(void *b, char c, size_t len);
int ft_strcmp(char *s1, char *s2);
size_t	ft_strlen(const char *s);


void ft_aff_list_ptr_sur_char_content(t_list *alst); // pour void		*content; de type char *
void ft_simplify_list(t_list *lst);

size_t ft_count_simpleCmds_nbr(t_list *lst_token);
int ft_parse_tokens_in_s_cmd(t_cmd *cmd, char *line, char **envp, t_list *lst_token);
t_data *ft_struct_init_data(t_data **data);
t_data *ft_struct_init_data2(t_data **data, t_list *lst_token, t_list *token);
t_settings	*ft_struct_init_settings(t_settings **set);
t_settings_del	*ft_struct_init_settings_del(t_settings_del **del);


// PARSING LST_TOKEN in SimpleCmd
void 	ft_count_nb_of_infile_in_simpleCmd(t_simpleCmd *simpleCmd);
void 	ft_count_nb_of_outfile_in_simpleCmd(t_simpleCmd *simpleCmd);
void 	ft_count_nb_of_errfile_in_simpleCmd(t_simpleCmd *simpleCmd);
void 	ft_count_nb_of_tokens_in_simpleCmd(t_list *start_lst_token, t_simpleCmd *simpleCmd);
t_list 	*ft_readjust_start_lst_token(t_list *start_lst_token, t_cmd *cmd, size_t i);
void	ft_get_end_simpleCmd_pos(t_cmd *cmd, t_simpleCmd *simpleCmd, t_list **dynamic_lst_token);
void	ft_aff_abs_cmd_and_args(t_cmd	*cmd);
void	ft_get_end_simpleCmd_pos(t_cmd *cmd, t_simpleCmd *simpleCmd, t_list **start_lst_token);
void	ft_malloc_redir_file_tabs_of_simpleCmd(t_simpleCmd *simpleCmd);
char **ft_malloc_outfile_tab(t_simpleCmd *simpleCmd);
char **ft_malloc_infile_tab(t_simpleCmd *simpleCmd);
char **ft_malloc_errfile_tab(t_simpleCmd *simpleCmd);
void	ft_count_nb_of_redir_token_in_simpleCmd(t_cmd *cmd, t_simpleCmd *simpleCmd, t_list *start_lst_token, size_t i);
void	ft_count_final_nb_of_tokens_in_simpleCmd(t_list *start_lst_token, t_simpleCmd *simpleCmd);

void	parse(t_list *lst, t_simpleCmd *simpleCmd, size_t i, t_cmd *cmd);
void	ft_lstdelone2(t_list **lst, t_simpleCmd *simpleCmd, size_t i, t_cmd *cmd);
void	ft_lstdelone(t_list **lst);

int		ft_malloc_and_parse_cmd_and_args_tab_of_simpleCmd(t_list *lst_token, t_simpleCmd *simpleCmd);
char	**ft_get_abs_argumentsb(char **abs_c_and_a);
void	ft_del_and_parse_redir_token_in_simpleCmd(t_list **alst, size_t index, t_list **lst_token, t_cmd *cmd);
int *ft_init_ctab(int **int_tab, size_t len, int init_value);
void ft_reconnect_lst_token(t_list *lst_token, size_t position);
t_list 	*ft_readjust_start_lst_token(t_list *start_lst_token, t_cmd *cmd, size_t i);


//parse_redir_head_token.c
t_list	*ft_head_redir_token(t_list *curr, t_cmd *cmd, t_list *lst_token, t_settings_del *del);
void	ft_flag_head_list(t_cmd *cmd, t_settings_del *del);
t_settings_del	*ft_redir_in_head(t_list *curr, t_cmd *cmd, t_list *lst_token, t_settings_del *del);
t_settings_del *ft_redir_out_head(t_list *curr, t_cmd *cmd, t_list *lst_token, t_settings_del *del);
t_settings_del 	*ft_redir_err_head(t_list *curr, t_cmd *cmd, t_list *lst_token, t_settings_del *del);

//parse_redir_middle_token.c
t_list	*ft_middle_redir_token(t_list *curr, t_cmd *cmd, t_list *lst_token, t_settings_del *del);
t_settings_del *ft_redir_in_middle(t_list *curr, t_cmd *cmd, t_list *lst_token, t_settings_del *del);
t_settings_del *ft_redir_out_middle(t_list *curr, t_cmd *cmd, t_list *lst_token, t_settings_del *del);
t_settings_del *ft_redir_err_middle(t_list *curr, t_cmd *cmd, t_list *lst_token, t_settings_del *del);

//parse_heredoc.c 
void	ft_get_last_heredoc_position(t_cmd *cmd);
void	ft_tag_last_heredoc_in_infile(t_simpleCmd *simpleCmd);
int ft_get_max_heredoc_index(int *tab, int len);
int ft_get_last_heredoc_index(int *tab, int len);
void	ft_malloc_heredocs_of_cmd(t_cmd *cmd);
void	ft_count_nbr_of_heredocs_in_cmd_line(t_cmd *cmd);
char	**ft_malloc_heredocs_tab(t_cmd *cmd);

//del_and_parse.c
void	ft_lstdelone(t_list **lst);
void	ft_lstdelone2(t_list **lst, t_simpleCmd *simpleCmd, size_t i, t_cmd *cmd);
void	ft_parse(t_list *lst, t_simpleCmd *simpleCmd, size_t i, t_cmd *cmd);
void    ft_parse_redir_out(t_list *lst, t_simpleCmd *simpleCmd, size_t i);
void    ft_parse_redir_in(t_list *lst, t_simpleCmd *simpleCmd, size_t i);
void    ft_parse_redir_heredoc(t_list *lst, t_simpleCmd *simpleCmd, t_cmd *cmd);
void    ft_parse_redir_err(t_list *lst, t_simpleCmd *simpleCmd, size_t i);




int 	ft_check_close_error(int fd);
void	ft_error_msg(char *infile);
void 	ft_error_msg2(char *str);
void	ft_error_msg3(char *str);


void ft_error(char *const str);
void	ft_error_heredoc(char *const str, int line_count);

void	ft_restore_original_in_and_out(t_settings *set);
void	ft_redirect_input(t_settings *set);

//***1st simpleCmd:
void	ft_first_simpleCmd_w_infile(t_settings *set, t_cmd *cmd);
void    ft_outfiles_before_nofile(t_settings *set, t_cmd *cmd);
void    ft_create_pipe(t_settings *set);
void	ft_open_outfiles(t_settings *set, t_cmd *cmd);


//****regular simpleCmd:
void ft_regular_simpleCmd(t_settings *set, t_cmd *cmd);
void    ft_open_infiles(t_settings *set, t_cmd *cmd);


//*****last simpleCmd : 
void	ft_last_simpleCmd(t_settings *set, t_cmd *cmd);
void	ft_open_outfiles_in_last_but_not_first_simpleCmd(t_settings *set, t_cmd *cmd, int k);



//char	*ft_swap_ptr(char **token_content, char *blank_node);
char	*ft_strdup(const char *s);


//expand.c
int ft_get_token_quoting_rule2(char *str, size_t i, int *quoting_rule, int *quoting_rule_adequate);
int	ft_expand_exists(t_list *lst_token);
int ft_find_expand(t_list *lst_token);
void ft_tag_expand(t_list *lst_token);
void	ft_modify_lst_token(t_list *lst_token);

//heredoc.c
void	ft_heredoc_interaction(t_cmd *cmd, size_t i, int mode);



#endif
