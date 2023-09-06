#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h> //pour open les 3 directives de preprocesseur
#include <sys/stat.h>
#include <fcntl.h> //pr pipe pr unlink
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
};

enum e_quoting_rule
{
	whitespace_separator,
	single_quote,
	double_quote,
};

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
	size_t 	i;
	size_t 	j;
	size_t	k;
	size_t index;

} t_settings_del;

typedef struct s_list
{
	char			*content;
	size_t			position;
	int				type;
	int				title;
	int				quoting_rule;		// whitwspace_separator 0 ,single quote 1, d_quote 2
	int				retokenize_allowed;// pour trim and clear et retokenizer un expand par ex/
	int				expand_exists;
	struct s_list	*next;
	struct s_list	*prev;
	size_t			start_token_pos;
	size_t			start_token_pos_exists;
	size_t			end_token_pos;
	size_t			quoting_rule_adequate;
	int				tag_ambigeous;

} t_list;

typedef struct s_data
{
	t_list	*token;	   // lst_token
	t_list	*lst_token; // lst
} t_data;
// description dune simple commande et ses arguments
typedef struct s_simpleCmd
{
	size_t	nb_of_tokens_in_simpleCmd;
	size_t	nb_of_redir_token;
	size_t	nb_of_heredoc;
	size_t	k;
	size_t	nb_of_infile;
	size_t	nb_of_outfile;
	size_t	nb_of_errfile;
	size_t	first_redir;
	size_t	nb_of_outfile_before_nofile;
	size_t	nb_of_errfile_before_nofile;
	size_t	nofile;
	int		number_of_arguments;
	int		errnum;
	char	**cmd_and_args;
	char	**abs_cmd_and_args;
	char	**infile;
	char	**heredoc;
	char	**outfile;
	char	**errfile;
	int		*append_track_index;
	int		*heredoc_track_index;
	size_t	end_simpleCmd_pos;
} t_simpleCmd;
// description d une commande complete avec les multiples pipes eventuels et les IO redirections eventuels
typedef struct s_cmd
{
	size_t		nb_of_simpleCmds;
	t_simpleCmd	**simpleCmds;
	char		**path_tab;
	char		*outputfile;
	char		*inputfile;
	char		*errfile;
	int			*heredocs_track_index;
	char		**heredocs;
	size_t		k;
	size_t		nb_of_heredocs;
	int			background;
	t_list	 	*lst_token;
	int			flag_head_list;
	size_t		nb_of_infile;
	size_t		nb_of_outfile;
	size_t		nb_of_errfile;
	int			line_count;
} t_cmd;
//utils
char 			**ft_get_abs_arguments(int i, char **blocks);
size_t 			ft_words_nbr(const char *s, char c);
char 			**ft_create_tab(char const *s, char c, size_t words_nbr);
char 			**ft_split(char const *s, char c);
char			*ft_strcpy(char *dst, char const *src);
char			*ft_init_string(size_t len);
char 			*ft_strcat(char *dest, char const *src);
char 			*ft_strjoin(char *s1, char const *s2);
char 			*ft_strndup(char *src, int n);
char 			*ft_update_string(char **str, char *new);
char			*ft_strdup(const char *s);
char			*ft_strcpy(char *dst, char const *src);
char			*ft_strdup(const char *s);
size_t			ft_strlen(const char *s);
void			ft_putstr_fd(char *s, int fd);
char			*ft_swap_ptr(char **token_content, char *blank_node);
void			*ft_memcpy(void *dst, const void *src, size_t n);
//update_free_memory
void			ft_free(t_cmd *cmd, t_list *lst_token, t_data *data, char *line);
void 			ft_free_struct_str(char **p);
void 			ft_free_tab(char ***tab);
void			ft_free_tab2(char ***tab);
void			ft_free_struct_int_tab(int **p);

void 			ft_free_struct_t_cmd_only(t_cmd **cmd);
void			ft_free_struct_t_cmd(t_cmd **cmd);
void 			ft_free_struct_t_simpleCmd(t_simpleCmd **simpleCmd);
void  			ft_free_struct_t_data(t_data **data);
void  			ft_free_struct_t_list_token(t_list **token);
void   			ft_free_struct_t_settings(t_settings **set);
void   			ft_free_struct_t_settings_del(t_settings_del **del);
void			ft_free_struct_t_list_lst_token(t_list **lst_token);
t_cmd 			*ft_struct_init(t_cmd **cmd, char init_value, char **blocks); // original
// replacements of original functions for parsing lst token to data strcture
t_cmd 			*ft_struct_init_cmd(t_cmd **cmd, t_list *lst_token);
t_data 			*ft_tokenize_line(char *line);
size_t			ft_tokenize(char *str, char *line, size_t i, t_data *data);
int 			ft_split_tokens_in_s_cmd(t_cmd *cmd, char *line, char **envp, t_list *lst_token);
t_simpleCmd 	*ft_struct_init_simpleCmd(t_simpleCmd **ptr);
t_simpleCmd		**ft_struct_array_init(t_simpleCmd **ptr, char init_value, size_t simpleCmds_nbr);
char 			**ft_get_path(char **envp);
int				ft_execute_cmd(t_cmd *cmd, int i, char *envp[], t_settings *set);
int 			ft_setting_redirections_and_pipes(t_cmd *cmd, char *envp[], t_data *data, t_list *lst_token, char *line);
char 			*ft_init_cstring(char **str, size_t len, char init_value);
void 			*ft_memset(void *b, char c, size_t len);
void			*ft_memset2(void *b, int n, size_t len);
int				*ft_init_ctab(int **int_tab, size_t len, int init_value);
void			ft_aff_abs_cmd_and_args(t_cmd	*cmd);
// lexer
void			ft_check_prerequesite_of_line_input(char *line);
void			ft_check_input_cases_for_return_empty_prompt(char *line);
int 			ft_check_bash_syntax_error_caracteres_volee(t_list *lst_token);
void 			ft_get_token_quoting_rule(char *str, t_list *lst_token, size_t i);
void 			ft_get_token_content(t_data *data, size_t start_token_pos, size_t end_token_pos, char *line);
t_data 			*ft_tokenize_line(char *line); // void pour commencer les tests
char 			**ft_tokenize_line1(char *line);
t_list 			*ft_lstlast(t_list *lst);
void			ft_lstadd_back(t_list **alst, t_list *new);
t_list 			*ft_lstnew_for_lst(t_data *data);
t_list			*ft_lstnew_data_token(char *content);
t_list 			*ft_create_list(void);
void			ft_simplify_list(t_list *lst);
void			ft_aff_list_ptr_sur_char_content(t_list *alst); // pour void		*content; de type char *
size_t			ft_char_is_whitespace(char *str, char *line, size_t i, t_data *data);
int 			ft_char_isnull_no_qr(char *line, size_t i, t_data *data);
int 			ft_get_token_type(char *str, t_list *token, t_data *data, size_t i, char *line);
int				ft_get_token_type_great_or_greatgreat(char *str, t_list *token, t_data *data, size_t i, char *line);
int				ft_get_token_type_less_or_lessless(char *str, t_list *token, t_data *data, size_t i, char *line);
int				ft_get_token_type_pipe(t_list *token, t_data *data, size_t i, char *line);
size_t 			ft_char_is_operator(char *line, size_t i, t_data *data);
void 			ft_char_after_ws_token_exist_no_qr(char *line, size_t i, t_data *data);
size_t 			ft_char_after_ws_is_operator_no_token_exists_no_qr(size_t i, char *line, t_data *data);
void 			ft_char_or_token_is_unique(char *line, size_t i, t_data *data); // char or token is unique // faudrait  il recuperer ici la end pos puisqu on sort du code des token et qu on a atteint un \0 ?
void 			ft_char_after_ws_isnull_token_exists_noqr(char *line, size_t i, t_data *data);
int 			ft_is_char_operand(char *str, t_list *lst_token);
void 			ft_get_token_content_lengh_for_malloc(t_list *token, size_t start_token_pos, size_t end_token_pos);
void			*ft_memcpy(void *dst, const void *src, size_t n);
void			*ft_memset(void *b, char c, size_t len);
int 			ft_strcmp(char *s1, char *s2);
size_t			ft_strlen(const char *s);
size_t			ft_count_simpleCmds_nbr(t_list *lst_token);
t_data 			*ft_struct_init_data(t_data **data);
t_data 			*ft_struct_init_data2(t_data **data, t_list *lst_token, t_list *token);
t_settings		*ft_struct_init_settings(t_settings **set);
t_settings_del	*ft_struct_init_settings_del(t_settings_del **del);
// PARSING LST_TOKEN in SimpleCmd
int				ft_parse_tokens_in_s_cmd(t_cmd *cmd, t_list *lst_token);
t_list			*ft_lst_first(t_list *lst);
t_list 			*ft_readjust_start_lst_token(t_list *start_lst_token, t_cmd *cmd, size_t i);
void			ft_del_and_parse_redir_token_in_simpleCmd(t_list **alst, size_t index, t_list **lst_token, t_cmd *cmd);
void 			ft_reconnect_lst_token(t_list *lst_token, size_t position);

void 			ft_count_nb_of_infile_in_simpleCmd(t_simpleCmd *simpleCmd);
void 			ft_count_nb_of_outfile_in_simpleCmd(t_simpleCmd *simpleCmd);
void 			ft_count_nb_of_errfile_in_simpleCmd(t_simpleCmd *simpleCmd);
void 			ft_count_nb_of_tokens_in_simpleCmd(t_list *start_lst_token, t_simpleCmd *simpleCmd);
void			ft_aff_abs_cmd_and_args(t_cmd	*cmd);
void			ft_get_end_simpleCmd_pos(t_cmd *cmd, t_simpleCmd *simpleCmd, t_list **start_lst_token);
void			ft_malloc_redir_file_tabs_of_simpleCmd(t_simpleCmd *simpleCmd);
char 			**ft_malloc_outfile_tab(t_simpleCmd *simpleCmd);
char 			**ft_malloc_infile_tab(t_simpleCmd *simpleCmd);
char 			**ft_malloc_errfile_tab(t_simpleCmd *simpleCmd);
char			**ft_malloc_heredoc_tab(t_simpleCmd *simpleCmd);
void			ft_count_nb_of_redir_token_in_simpleCmd(t_cmd *cmd, t_simpleCmd *simpleCmd, t_list *start_lst_token, size_t i);
void			ft_count_final_nb_of_tokens_in_simpleCmd(t_list *start_lst_token, t_simpleCmd *simpleCmd);
t_list			*ft_count_nb_of_redir_err_in_sCmd(t_simpleCmd *simpleCmd, t_list *tmp);
t_list			*ft_count_nb_of_redir_out_in_sCmd(t_simpleCmd *simpleCmd, t_list *tmp);
t_list			 *ft_count_nb_of_redir_append_in_sCmd(t_simpleCmd *simpleCmd, t_list *tmp);
void			parse(t_list *lst, t_simpleCmd *simpleCmd, size_t i, t_cmd *cmd);
void			ft_lstdelone2(t_list **lst, t_simpleCmd *simpleCmd, size_t i, t_cmd *cmd);
void			ft_lstdelone(t_list **lst);
int				ft_malloc_and_parse_cmd_and_args_tab_of_simpleCmd(t_list *lst_token, t_simpleCmd *simpleCmd);
char			**ft_get_abs_argumentsb(char **abs_c_and_a);
void			ft_del_and_parse_redir_token_in_simpleCmd(t_list **alst, size_t index, t_list **lst_token, t_cmd *cmd);
int 			*ft_init_ctab(int **int_tab, size_t len, int init_value);
void 			ft_reconnect_lst_token(t_list *lst_token, size_t position);
t_list 			*ft_readjust_start_lst_token(t_list *start_lst_token, t_cmd *cmd, size_t i);
//parse_redir_head_token.c
t_list			*ft_head_redir_token(t_list *curr, t_cmd *cmd, t_list *lst_token, t_settings_del *del);
void			ft_flag_head_list(t_cmd *cmd, t_settings_del *del);
t_settings_del	*ft_redir_in_head(t_list *curr, t_cmd *cmd, t_list *lst_token, t_settings_del *del);
t_settings_del 	*ft_redir_out_head(t_list *curr, t_cmd *cmd, t_list *lst_token, t_settings_del *del);
t_settings_del 	*ft_redir_err_head(t_list *curr, t_cmd *cmd, t_list *lst_token, t_settings_del *del);
//parse_redir_middle_token.c
t_list			*ft_middle_redir_token(t_list *curr, t_cmd *cmd, t_list *lst_token, t_settings_del *del);
t_settings_del 	*ft_redir_in_middle(t_list *curr, t_cmd *cmd, t_list *lst_token, t_settings_del *del);
t_settings_del 	*ft_redir_out_middle(t_list *curr, t_cmd *cmd, t_list *lst_token, t_settings_del *del);
t_settings_del *ft_redir_err_middle(t_list *curr, t_cmd *cmd, t_list *lst_token, t_settings_del *del);
//parse_heredoc.c 
void			ft_get_last_heredoc_position(t_cmd *cmd);
void			ft_tag_last_heredoc_in_infile(t_simpleCmd *simpleCmd);
int 			ft_get_max_heredoc_index(int *tab, int len);
int 			ft_get_last_heredoc_index(int *tab, int len);
void			ft_malloc_heredocs_of_cmd(t_cmd *cmd);
void			ft_count_nbr_of_heredocs_in_cmd_line(t_cmd *cmd);
char			**ft_malloc_heredocs_tab(t_cmd *cmd);
//del_and_parse.c
void			ft_lstdelone(t_list **lst);
void			ft_lstdelone2(t_list **lst, t_simpleCmd *simpleCmd, size_t i, t_cmd *cmd);
void			ft_parse(t_list *lst, t_simpleCmd *simpleCmd, size_t i, t_cmd *cmd);
void			ft_parse_redir_out(t_list *lst, t_simpleCmd *simpleCmd, size_t i);
void   			ft_parse_redir_in(t_list *lst, t_simpleCmd *simpleCmd, size_t i);
void   			ft_parse_redir_heredoc(t_list *lst, t_simpleCmd *simpleCmd, t_cmd *cmd);
void    		ft_parse_redir_err(t_list *lst, t_simpleCmd *simpleCmd, size_t i);
int				ft_check_open_error(int fdin, int fdout, t_cmd *cmd);
int 			ft_check_close_error(int fd);
void			ft_error_msg(char *infile);
void 			ft_error_msg2(char *str);
void			ft_error_msg3(char *str);
void 			ft_error(char *const str);
void			ft_error_heredoc(char *const str, int line_count);
int				find_length(int n);
char			*ft_itoa(int n);
void			ft_restore_original_in_and_out(t_settings *set);
void			ft_redirect_input(t_settings *set);

//***1st simpleCmd:
void			ft_first_simpleCmd_w_infile(t_settings *set, t_cmd *cmd);
int				ft_set_fdin_for_first_s_cmd_w_infile(t_settings *set, t_cmd *cmd, int flag_random_heredoc);
void   			ft_outfiles_before_nofile(t_settings *set, t_cmd *cmd);
void    		ft_create_pipe(t_settings *set);
void			ft_open_outfiles(t_settings *set, t_cmd *cmd);
//****regular simpleCmd:
void			ft_regular_simpleCmd(t_settings *set, t_cmd *cmd);
void    		ft_open_infiles(t_settings *set, t_cmd *cmd);
int     		ft_set_fdin_for_regular_simpleCmd(t_settings *set, t_cmd *cmd, int flag_random_heredoc);

//*****last simpleCmd : 
void			ft_last_simpleCmd(t_settings *set, t_cmd *cmd);
void			ft_open_outfiles_in_last_but_not_first_simpleCmd(t_settings *set, t_cmd *cmd, int k);
int 			ft_set_fdin_in_last_but_not_first_simpleCmd(t_settings *set, t_cmd *cmd, int flag_random_heredoc);
void 			ft_set_fdin_error_msg(t_settings *set, t_cmd *cmd);

//expand.c
int 			ft_get_token_quoting_rule2(char *str, size_t i, int *quoting_rule, int *quoting_rule_adequate);
int				ft_expand_exists(t_list *lst_token);
int 			ft_find_expand(t_list *lst_token);
void 			ft_tag_expand(t_list *lst_token);
int				 ft_check_expand(t_list *lst_token);
void			ft_modify_lst_token(t_list *lst_token);
//heredoc.c
void			ft_heredoc_interaction(t_cmd *cmd, size_t i, int mode);
int 			ft_open_heredoc_hidden_file(int mode, int fd);
void 			ft_get_EOF(t_cmd *cmd, size_t i, char *line_heredoc, int fd);
void			ft_recursiv(t_cmd *cmd, size_t i, int fd);
void			ft_fill_heredocument(int fd, char *line_heredoc);
int 			ft_last_heredoc(t_cmd *cmd, char *line_heredoc, size_t i);
void			ft_add_history_and_free_rl(char *line_heredoc);
void			ft_heredoc_input_is_null(t_cmd *cmd, size_t i);
int				ft_exit_status(pid_t last_pid, t_settings *set);
//setting_redicrections exec
int				ft_setting_redirections_and_pipes(t_cmd *cmd, char *envp[], t_data *data, t_list *lst_token, char *line);
void			ft_child_process(t_settings *set, t_cmd *cmd, char *envp[], int ret, t_data *data, t_list *lst_token, char *line);
void			ft_set_fdin_for_first_simpleCmd(t_settings *set, t_cmd *cmd);
void			ft_redirect_input(t_settings *set);
void			ft_redirect_output(t_settings *set);
void			ft_save_in_and_out(t_settings *set);
void			ft_restore_original_in_and_out(t_settings *set);
void			ft_open_outfiles_in_last_but_not_first_simpleCmd(t_settings *set, t_cmd *cmd, int k);
void			ft_open_outfiles(t_settings *set, t_cmd *cmd);



#endif
