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
#include <limits.h>

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


enum e_builtin // a renommer en function
{
	echo,
	cd,
	pwd,
	export,
	unset,
	env,
	exxit,
};



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
	int		nofile;
	int		ret;
	int 	pip[2];	
	int 	errnum;
} t_settings;

typedef struct s_settings_del
{
	size_t 	i;
	size_t 	j;
	size_t	k;
	size_t index;
} t_settings_del;

typedef struct s_listenvp
{
	char				*key_value;
	size_t				position;
	struct s_listenvp	*next;
	struct s_listenvp	*prev;

} t_listenvp;



typedef struct s_data_env
{
	struct s_listenvp	*lst_envp;
	struct s_listenvp	*lst_envp_d;
	int					flag_oldpwd_istoremove;

} t_data_env;

typedef struct s_expand
{
	int 	quoting_rule_adequate;
	int 	quoting_rule;
	size_t 	i;
	size_t 	j;
	int		flag_expand_here;
	int		flag_expand_in_token;
	size_t 	start_expand_pos;
	size_t	end_expand_pos;
	int		flag_dollar_quest;

} t_expand;

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
	int				tag_empty_cmd_before_DQ;
	int				tag_empty_cmd_after_DQ;
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
	size_t 	flag_empty_simpleCmd;
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
	int		export_no_option;
	int		export_solo;
	int		unset_no_option;
	int		env_no_option;
	int		echo_no_option;
	int		cd_no_option;
	int		exit_no_option;
	int		unset_solo;
	int		cd_solo;
	int		exit_solo;
	char	*oldpwd;
	char	*pwd;
	char 	*exit_str;
	int		exit_code;
	int		is_builtin;
	int		builtin_solo;
	int		builtin;
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
	t_list	 	*lst_token_retokenized;
//t_listenvp	*lst_envp;
	int			flag_head_list;
	int			flag_empty_head_list;
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
size_t 			ft_operator_glued_on_other_token(char *line, size_t i, t_data *data);
size_t 			ft_operator_not_glued_on_other_token(char *line, size_t i, t_data *data);
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
int				ft_parse_tokens_in_s_cmd(t_cmd *cmd, t_list *lst_token, t_data_env *data_env);
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


int				ft_setting_redirections_and_pipes(t_cmd *cmd, char *envp[], t_data *data, char *line, t_data_env *data_env); //, t_listenvp *lst_envp);
void 			ft_child_process(t_settings *set, t_cmd *cmd, char **envp_t, t_data *data, char *line, t_data_env *data_env);

void			ft_set_fdin_for_first_simpleCmd(t_settings *set, t_cmd *cmd);
void			ft_redirect_input(t_settings *set, t_cmd *cmd);
void			ft_redirect_output(t_settings *set);
void			ft_save_in_and_out(t_settings *set);
void			ft_restore_original_in_and_out(t_settings *set);
void			ft_open_outfiles_in_last_but_not_first_simpleCmd(t_settings *set, t_cmd *cmd, int k);
void			ft_open_outfiles(t_settings *set, t_cmd *cmd);
void			ft_free_in_child(t_cmd *cmd, t_data *data,char *line);
//EXPAND
t_expand		*ft_struct_init_expand(t_expand **exp);
int 			ft_get_token_quoting_rule2(char *str, size_t i, int *quoting_rule, int *quoting_rule_adequate);
void			ft_tag_ambigeous_for_redir(t_list *lst_token);
int 			ft_is_expand_here(t_list *lst_token, char *str, char *buffer, char *envp[]);
char 			*ft_get_scope_expand(size_t end_expand_pos, size_t start_expand_pos, char *str, int *flag_expand_here);
int 			ft_is_expand_to_substitute_redir(t_list *lst_token);
int 			ft_tag_ambiguous_redir(t_list *lst_token);
int				ft_expand_exists(t_list *lst_token);
char			*ft_dequote(char *str);
void			ft_modify_lst_token(t_list *lst_token);
void 			ft_expand_and_retokenize(t_list *lst_token, char *envp[]);
int 			ft_is_expand_to_substitute(t_list *lst_token, char *envp[]);
int				ft_is_expand_unvalidated(char **invalidators, char c);
int				ft_strcmp_char(char c1, char c2);
void   			ft_free_struct_t_expand(t_expand **exp);
char 			*ft_substitute(char *expand, char *envp[]);
char			*ft_get_var(char **envp, char *expand);
int 			ft_get_token_quoting_rule3(char *str, size_t i, int *quoting_rule, int *quoting_rule_adequate, char *buffer_dequote, int *j);
t_list			*ft_retokenize_and_dequote_token_1(t_cmd *cmd, t_list *start_lst_token, t_simpleCmd *simpleCmd);
t_data 			*ft_retokenize_and_dequote_token_2(t_cmd *cmd, t_list *start_lst_token, t_simpleCmd *simpleCmd, t_data *data2);
void			ft_lstdelone_beta(t_list *lst);
void			ft_aff_list_ptr_sur_char_content2(t_list *alst); // pour void		*content; de type char *
t_list 			*ft_lstnew_for_lst_retokenized2(t_list *token);
t_list 			*ft_lstnew_for_lst_retokenized(t_list *start_lst_token_retokenized);
t_list 			*ft_lstfind(t_list *alst, int position);
int				ft_malloc_and_parse_cmd_and_args_tab_of_simpleCmd_from_retokenized_lst(t_list *lst_token_retokenized, t_simpleCmd *simpleCmd);
void			ft_count_final_nb_of_tokens_in_simpleCmd_retokenized(t_list *start_lst_token, t_simpleCmd *simpleCmd);
void			ft_aff_list_ptr_sur_char_content3(t_list *alst);
t_list 			*ft_lstnew_for_lst2(t_data *data);
t_list 			*ft_lstnew_for_lst_token_copy(t_list *tmp);

//export
void			ft_aff_listenv_ptr_sur_char_content(t_listenvp *alst); // pour void		*content; de type char *
void			ft_lstadd_back_envp(t_listenvp **alst, t_listenvp *new);
t_listenvp 		*ft_lstlast_envp(t_listenvp *lst);
t_listenvp		*ft_lstnew_for_lst_envp(char *str);

void			ft_free_struct_t_list_lst_envp(t_listenvp **lst_envp);
t_listenvp		*ft_get_lst_envp2(char **envp);
void			ft_aff_tab_envp(char **tab);
char 			**ft_lst_to_tab(t_listenvp *lst_envp);
size_t 			ft_count_keys_in_lst_envp(t_listenvp *lst_envp);
t_data_env		*ft_struct_init_data_env(t_data_env **data_env);
void    		ft_free_struct_t_data_env(t_data_env **data_env);
int 			ft_export_x(char **envp);
int 			ft_isalpha(int c);
int				ft_isalnum(int c);
int 			ft_isdigit(char c);
void			ft_check_export(t_cmd *cmd, t_list *start_lst_token_retokenized, t_simpleCmd *simpleCmd, t_data_env *data_env);
void			ft_aff_list_envp_sur_char_content(t_listenvp *alst);// pour void		*content; de type char *
void			ft_aff_list_envp_d_sur_char_content(t_listenvp *alst);// pour void		*content; de type char *
t_listenvp		*ft_add_double_quote_to_envp_d(t_listenvp *lst_envp_d);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
int				ft_is_var_already_in_lst_envpd2(t_listenvp *lst_envp, char *str);
void			ft_check_unset(t_cmd *cmd, t_list *start_lst_token_retokenized, t_simpleCmd *simpleCmd, t_data_env *data_env);
int				ft_unset_value_is_valid(t_list *tmp);
void			ft_remove_var_from_lst_envp(t_list *tmp, t_data_env *data_env);
void			ft_remove_var_from_lst_envp_d(t_list *tmp, t_data_env *data_env);
int				ft_is_var_to_unset_already_in_lst_envp(t_data_env *data_env, char *str);
int				ft_is_var_to_unset_already_in_lst_envp_d(t_data_env *data_env, char *str);
t_listenvp		*ft_delete_node(t_listenvp *alst, char *str);
int				ft_count_n_for_key(char *str);
void			ft_check_env(t_cmd *cmd, t_list *start_lst_token_retokenized, t_simpleCmd *simpleCmd, t_data_env *data_env);
void			ft_check_echo(t_cmd *cmd, t_list *start_lst_token_retokenized, t_simpleCmd *simpleCmd, t_data_env *data_env);
void			ft_error_msg4(char *str);
void			ft_error_msg5(char *infile);
void			ft_check_cd(t_cmd *cmd, t_list *start_lst_token_retokenized, t_simpleCmd *simpleCmd, t_data_env *data_env);
void			ft_cd_no_option(t_data_env *data_env, char *str, t_simpleCmd *simpleCmd);
char			*get_var_in_lst_envp_for_cd(t_data_env *data_env, char *str);
char			*ft_strchr(const char *s, int c);
int				ft_check_is_space(char *str);
int				ft_is_space(int c);
void			ft_update_oldpwd_and_pwd_in_lst_envp(t_data_env *data_env, char *cwd);
void			ft_add_var_oldpwd_to_lst_envp_d_for_cd_dash(char *str, t_listenvp *lst_envp_d);
t_listenvp 		*ft_lstfind_content(t_listenvp *alst, char *keyval);
int 			ft_is_var_to_unset_already_in_lst_envp(t_data_env *data_env, char *str);
int 			ft_is_var_to_unset_already_in_lst_envp_d(t_data_env *data_env, char *str);
int 			ft_is_var_already_in_lst_envp_d(t_listenvp *lst_envp_d, char *str);
void			ft_update_oldpwd_and_pwd_in_lst_envp_for_cd_dash_dash(t_data_env *data_env, char *cwd, t_simpleCmd *simpleCmd);
int				ft_change_directory_for_cd_dash_dash(t_data_env *data_env, char *new_path, t_simpleCmd *simpleCmd);
t_listenvp 		*ft_lstfind_content(t_listenvp *alst, char *keyequal);
//void			ft_cd_option_dot(t_data_env *data_env, t_simpleCmd *simpleCmd);
//int			ft_change_directory_for_cd_dot(t_data_env *data_env, char *cwd, t_simpleCmd *simpleCmd);
void			ft_cd_option_slash(t_data_env *data_env, char *str, t_simpleCmd *simpleCmd);
void			ft_check_pwd(t_cmd *cmd, t_list *start_lst_token_retokenized, t_simpleCmd *simpleCmd, t_data_env *data_env);
int 				ft_get_pwd(t_data_env *data_env, t_simpleCmd *simpleCmd);
//exit
void				ft_check_exit(t_cmd *cmd, t_list *start_lst_token_retokenized, t_simpleCmd *simpleCmd, t_data_env *data_env);

int 				ft_exit_is_arg_valid(char *str, t_simpleCmd *simpleCmd);
unsigned long long	ft_atoi_modulo(char *str, t_simpleCmd *simpleCmd);
int 				ft_numeric_arg_required_msg(t_simpleCmd *simpleCmd, char *str);
void				ft_too_many_arg_msg(t_simpleCmd *simpleCmd);
//env
void				ft_error_msg6(char *infile);

//exit code
t_listenvp  *ft_get_exit_status(t_listenvp **alst, char *key, int exit_status);
void				ft_check_tab_envp_removed(char **tab);

//Signaux
void 				handler_sigint(int num);
void				ft_set_exit_code_in_lst_envp(void *lst_envp, int flag);


#endif