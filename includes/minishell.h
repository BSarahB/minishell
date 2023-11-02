/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 22:59:29 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/31 22:59:45 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/wait.h>
# include <signal.h>
# include <stddef.h>
# include <limits.h>

# define WORD 1
# define VARIABLE 2
# define PIPE 3
# define GREAT 4
# define GREATGREAT 5
# define LESS 6
# define LESSLESS 7
# define TERMINATE 8
# define PARENTHESIS_LEFT 9
# define PARENTHESIS_RIGHT 10
# define AMPERSAND 11
# define GREAT_AND_AMPERSAND 12
# define IGNORE 13

# define IN_PROMPT 1
# define IN_HD 2
# define HD_STOP 3
# define IN_S_CMD 4

enum e_builtin
{
	echo,
	cd,
	pwd,
	export,
	unset,
	env,
	exxit,
};

enum e_title
{
	operator,
	command,
	main_command,
	option,
	redir_in,
	redir_out,
	redir_err,
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
	int		fdin;
	int		fdout;
	size_t	i;
	size_t	j;
	int		exit_status;
	int		savein;
	int		saveout;
	int		nofile;
	int		ret;
	int		pip[2];
	int		pip_exists;
	int		errnum;
}	t_settings;

typedef struct s_settings_del
{
	size_t	i;
	size_t	j;
	size_t	k;
	size_t	index;
}	t_settings_del;

typedef struct s_listenvp
{
	char				*key_value;
	size_t				position;
	struct s_listenvp	*next;
	struct s_listenvp	*prev;

}	t_listenvp;

typedef struct s_data_env
{
	struct s_listenvp	*lst_envp;
	struct s_listenvp	*lst_envp_d;
	int					flag_oldpwd_istoremove;
	int					exit_parent;

}	t_data_env;

typedef struct s_expand
{
	int		quoting_rule_adequate;
	int		quoting_rule;
	size_t	i;
	size_t	j;
	int		flag_expand_here;
	int		flag_expand_in_token;
	size_t	start_expand_pos;
	size_t	end_expand_pos;
	int		flag_dollar_quest;
	int		flag_dollar_to_remove;
}	t_expand;

typedef struct s_list
{
	char			*content;
	size_t			position;
	int				type;
	int				title;
	int				quoting_rule;
	int				retokenize_allowed;
	int				expand_exists;
	struct s_list	*next;
	struct s_list	*prev;
	size_t			start_token_pos;
	size_t			start_token_pos_exists;
	size_t			end_token_pos;
	size_t			quoting_rule_adequate;
	int				tag_ambigeous;
	int				tag_empty_cmd_before_dq;
	int				tag_empty_cmd_after_dq;
}	t_list;

typedef struct s_data
{
	t_list	*token;
	t_list	*lst_token;
}	t_data;

typedef struct s_simpleCmd
{
	size_t	nb_of_tokens_in_simplecmd;
	size_t	flag_empty_simplecmd;
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
	size_t	end_simplecmd_pos;
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
	char	*exit_str;
	int		exit_code;
	int		is_builtin;
	int		builtin_solo;
	int		builtin;
	int		out_denied;
}	t_simpleCmd;

typedef struct s_cmd
{
	size_t		nb_of_simplecmds;
	t_simpleCmd	**simplecmds;
	char		**path_tab;
	char		*outputfile;
	char		*inputfile;
	char		*errfile;
	int			*heredocs_track_index;
	char		**heredocs;
	size_t		k;
	size_t		nb_of_heredocs;
	int			background;
	t_list		*lst_token;
	t_list		*lst_token_retokenized;
	int			flag_head_list;
	int			flag_empty_head_list;
	size_t		nb_of_infile;
	size_t		nb_of_outfile;
	size_t		nb_of_errfile;
	int			line_count;
}	t_cmd;

typedef struct s_param_sr
{
	t_cmd		*cmd;
	char		**envp_t;
	t_data		*data;
	char		*line;
	t_data_env	*data_env;

}	t_param_sr;

typedef struct s_param_qr2
{
	char	*str;
	size_t	i;
	int		*quoting_rule;
	int		*quoting_rule_adequate;
	int		*j;

}	t_param_qr2;

typedef struct s_param_m
{
	char		*line;
	t_cmd		*cmd;
	t_list		*lst_token;
	t_data		*data;
	int			exit_status;
	int			exit_syntax_error;
	int			exit_syntax;
	int			flag_save_envp;
	char		**envp_t;
	char		**envp_tab;
	t_data_env	*data_env;

}	t_param_m;

typedef struct s_param_b
{
	t_list	*tmp;
	int		flag_cd;

}	t_param_b;

typedef struct s_param_e
{
	t_list	*tmp;
	int		flag_export;

}	t_param_e;

typedef struct s_param_u
{
	t_list	*tmp;
	int		flag_unset;

}	t_param_u;

typedef struct s_param_en
{
	t_list	*tmp;
	int		flag_env;

}	t_param_en;

typedef struct s_param_utils
{
	t_listenvp	*tmp;
	int			n;
	char		*new_key_val;
	char		*key;
	char		*key_input;

}	t_param_utils;

typedef struct s_param_epur
{
	int	i;
	int	j;
	int	flag;
	int	quoting_rule;
	int	quoting_rule_adequate;

}	t_param_epur;

typedef struct s_param_var
{
	char	**var_content;
	char	*var;
	char	equal[2];

}	t_param_var;

typedef struct s_param_dbquotes
{
	char	*buffer_quoted;
	int		flag_equal;
	int		i;
	int		j;

}	t_param_dbquotes;

typedef struct s_param_parse
{
	size_t	k;
	char	**c_and_a;
	char	**abs_c_and_a;

}	t_param_parse;

typedef struct s_param_countsp
{
	size_t	k;
	size_t	simplecmd_nbr;
	t_list	*tmp;
	t_list	*new;

}	t_param_countsp;

typedef struct s_param_sub
{
	t_list	*lst_token;
	char	*str;
	char	*buffer;
	size_t	i;

}	t_param_sub;

char				**ft_get_abs_arguments(int i, char **blocks);
size_t				ft_words_nbr(const char *s, char c);
char				**ft_create_tab(char const *s, char c, size_t words_nbr);
char				**ft_split(char const *s, char c);
char				*ft_strcpy(char *dst, char const *src);
char				*ft_init_string(size_t len);
char				*ft_strcat(char *dest, char const *src);
char				*ft_strjoin(char *s1, char const *s2);
char				*ft_strndup(char *src, int n);
char				*ft_update_string(char **str, char *new);
char				*ft_strdup(const char *s);
char				*ft_strcpy(char *dst, char const *src);
char				*ft_strdup(const char *s);
size_t				ft_strlen(const char *s);
void				ft_putstr_fd(char *s, int fd);
char				*ft_swap_ptr(char **token_content, char *blank_node);
void				*ft_memcpy(void *dst, const void *src, size_t n);

void				ft_free(t_cmd *cmd, t_list *lst_token, t_data *data, \
															char *line);
void				ft_free_struct_str(char **p);
void				ft_free_tab(char ***tab);
void				ft_free_tab2(char ***tab);
void				ft_free_struct_int_tab(int **p);

void				ft_free_struct_t_cmd_only(t_cmd **cmd);
void				ft_free_struct_t_cmd(t_cmd **cmd);
void				ft_free_struct_t_simplecmd(t_simpleCmd **simpleCmd);
void				ft_free_struct_t_data(t_data **data);
void				ft_free_struct_t_list_token(t_list **token);
void				ft_free_struct_t_settings(t_settings **set);
void				ft_free_struct_t_settings_del(t_settings_del **del);
void				ft_free_struct_t_list_lst_token(t_list **lst_token);
t_cmd				*ft_struct_init(t_cmd **cmd, char init_value, \
															char **blocks);

t_cmd				*ft_struct_init_cmd(t_cmd **cmd, t_list *lst_token);
t_data				*ft_tokenize_line(char *line);
size_t				ft_tokenize(char *str, char *line, size_t i, t_data *data);
int					ft_split_tokens_in_s_cmd(t_cmd *cmd, char *line, \
								char **envp, t_list *lst_token);
t_simpleCmd			*ft_struct_init_simplecmd(t_simpleCmd **ptr);
t_simpleCmd			**ft_struct_array_init(t_simpleCmd **ptr, \
									char init_value, size_t simpleCmds_nbr);
char				**ft_get_path(char **envp);
int					ft_execute_cmd(t_cmd *cmd, int i, \
										char *envp[], t_settings *set);
int					ft_execve_join(t_cmd *cmd, char **envp, \
										char **abs_cmd_and_args);

char				*ft_init_cstring(char **str, size_t len, char init_value);
void				*ft_memset(void *b, char c, size_t len);
void				*ft_memset2(void *b, int n, size_t len);
int					*ft_init_ctab(int **int_tab, size_t len, int init_value);
void				ft_aff_abs_cmd_and_args(t_cmd	*cmd);

int					ft_check_bash_syntax_error_caracteres_volee( \
														t_list *lst_token);
void				ft_get_token_quoting_rule(char *str, t_list *lst_token, \
																size_t i);
void				ft_get_token_content(t_data *data, size_t \
							start_token_pos, size_t end_token_pos, char *line);
t_data				*ft_tokenize_line(char *line);
char				**ft_tokenize_line1(char *line);
t_list				*ft_lstlast(t_list *lst);
void				ft_lstadd_back(t_list **alst, t_list *new);
t_list				*ft_lstnew_for_lst(t_data *data);
t_list				*ft_lstnew_data_token(char *content);
t_list				*ft_create_list(void);
void				ft_simplify_list(t_list *lst);
void				ft_aff_list_ptr_sur_char_content(t_list *alst);
size_t				ft_char_is_whitespace(char *str, char *line, \
												size_t i, t_data *data);
int					ft_char_isnull_no_qr(char *line, size_t i, t_data *data);
int					ft_get_token_type(char *str, t_data *data, \
														size_t i, char *line);
int					ft_get_token_type_great_or_greatgreat(char *str, \
										t_data *data, size_t i, char *line);
int					ft_get_token_type_less_or_lessless(char *str, \
										t_data *data, size_t i, char *line);
int					ft_get_token_type_pipe(t_list *token, t_data *data, \
													size_t i, char *line);
size_t				ft_char_is_operator(char *line, size_t i, t_data *data);
size_t				ft_operator_glued_on_other_token(char *line, size_t i, \
															t_data *data);
size_t				ft_operator_not_glued_on_other_token(char *line, \
											size_t i, t_data *data);
void				ft_char_after_ws_token_exist_no_qr(char *line, \
										size_t i, t_data *data);
size_t				ft_char_after_ws_is_ope_notokn_noqr(size_t i, \
											char *line, t_data *data);
void				ft_char_or_token_is_unique(char *line, \
												size_t i, t_data *data);
void				ft_char_after_ws_isnull_token_exists_noqr(char *line, \
												size_t i, t_data *data);
int					ft_is_char_operand(char *str, t_list *lst_token);
void				ft_get_token_content_lengh_for_malloc(t_list *token, \
							size_t start_token_pos, size_t end_token_pos);
void				*ft_memcpy(void *dst, const void *src, size_t n);
void				*ft_memset(void *b, char c, size_t len);
int					ft_strcmp(char *s1, char *s2);
size_t				ft_strlen(const char *s);
size_t				ft_count_simplecmds_nbr(t_list *lst_token);
t_data				*ft_struct_init_data(t_data **data);
t_data				*ft_struct_init_data2(t_data **data, \
										t_list *lst_token, t_list *token);
t_settings			*ft_struct_init_settings(t_settings **set);
t_settings_del		*ft_struct_init_settings_del(t_settings_del **del);
void				ft_get_token_quoting_rule(char *str, \
												t_list *lst_token, size_t i);

int					ft_parse_tokens_in_s_cmd(t_cmd *cmd, \
									t_list *lst_token, t_data_env *data_env);
t_list				*ft_lst_first(t_list *lst);
t_list				*ft_readjust_start_lst_token(t_list *start_lst_token, \
													t_cmd *cmd, size_t i);
void				ft_del_parse_redir_token_in_scmd(t_list **alst, \
								size_t index, t_list **lst_token, t_cmd *cmd);
void				ft_reconnect_lst_token(t_list *lst_token, size_t position);
void				ft_count_nb_of_infile_in_simpleCmd(t_simpleCmd *simpleCmd);
void				ft_count_nb_of_outfile_in_simpleCmd(t_simpleCmd *simpleCmd);
void				ft_count_nb_of_errfile_in_simpleCmd(t_simpleCmd *simpleCmd);
void				ft_count_nb_of_tokens_in_simpleCmd(\
							t_list *start_lst_token, t_simpleCmd *simpleCmd);
void				ft_aff_abs_cmd_and_args(t_cmd	*cmd);
void				ft_get_end_simplecmd_pos(t_cmd *cmd, \
						t_simpleCmd *simpleCmd, t_list **start_lst_token);
void				ft_malloc_redir_file_tabs_of_simplecmd(\
												t_simpleCmd *simpleCmd);
char				**ft_malloc_outfile_tab(t_simpleCmd *simpleCmd);
char				**ft_malloc_infile_tab(t_simpleCmd *simpleCmd);
char				**ft_malloc_errfile_tab(t_simpleCmd *simpleCmd);
char				**ft_malloc_heredoc_tab(t_simpleCmd *simpleCmd);
void				ft_nb_of_redir_token_in_scmd(t_cmd *cmd, \
					t_simpleCmd *simpleCmd, t_list *start_lst_token, size_t i);
void				ft_count_final_nb_of_tokens_in_simplecmd(\
							t_list *start_lst_token, t_simpleCmd *simpleCmd);
t_list				*ft_count_nb_of_redir_err_in_scmd(\
										t_simpleCmd *simpleCmd, t_list *tmp);
t_list				*ft_count_nb_of_redir_out_in_scmd(\
										t_simpleCmd *simpleCmd, t_list *tmp);
t_list				*ft_count_nb_of_redir_append_in_scmd(\
										t_simpleCmd *simpleCmd, t_list *tmp);
void				parse(t_list *lst, t_simpleCmd *simpleCmd, \
													size_t i, t_cmd *cmd);
void				ft_lstdelone2(t_list **lst, \
								t_simpleCmd *simpleCmd, size_t i, t_cmd *cmd);
void				ft_lstdelone(t_list **lst);
int					ft_malloc_and_parse_cmd_and_args_tab_of_simplecmd(\
								t_list *lst_token, t_simpleCmd *simpleCmd);
char				**ft_get_abs_argumentsb(char **abs_c_and_a);
void				ft_del_parse_redir_token_in_scmd(t_list **alst, \
								size_t index, t_list **lst_token, t_cmd *cmd);
int					*ft_init_ctab(int **int_tab, size_t len, int init_value);
void				ft_reconnect_lst_token(t_list *lst_token, size_t position);
t_list				*ft_readjust_start_lst_token(t_list *start_lst_token, \
									t_cmd *cmd, size_t i);

t_list				*ft_head_redir_token(t_list *curr, t_cmd *cmd, \
								t_list *lst_token, t_settings_del *del);
void				ft_flag_head_list(t_cmd *cmd, t_settings_del *del);
t_settings_del		*ft_redir_in_head(t_list *curr, t_cmd *cmd, \
									t_list *lst_token, t_settings_del *del);
t_settings_del		*ft_redir_out_head(t_list *curr, t_cmd *cmd, \
									t_list *lst_token, t_settings_del *del);
t_settings_del		*ft_redir_err_head(t_list *curr, t_cmd *cmd, \
									t_list *lst_token, t_settings_del *del);

t_list				*ft_middle_redir_token(t_list *curr, t_cmd *cmd, \
								t_list *lst_token, t_settings_del *del);
t_settings_del		*ft_redir_in_middle(t_list *curr, t_cmd *cmd, \
									t_list *lst_token, t_settings_del *del);
t_settings_del		*ft_redir_out_middle(t_list *curr, t_cmd *cmd, \
									t_list *lst_token, t_settings_del *del);
t_settings_del		*ft_redir_err_middle(t_list *curr, t_cmd *cmd, \
									t_list *lst_token, t_settings_del *del);

void				ft_get_last_heredoc_position(t_cmd *cmd);
void				ft_tag_last_heredoc_in_infile(t_simpleCmd *simpleCmd);
int					ft_get_max_heredoc_index(int *tab, int len);
int					ft_get_last_heredoc_index(int *tab, int len);
void				ft_malloc_heredocs_of_cmd(t_cmd *cmd);
void				ft_count_nbr_of_heredocs_in_cmd_line(t_cmd *cmd);
char				**ft_malloc_heredocs_tab(t_cmd *cmd);

void				ft_lstdelone(t_list **lst);
void				ft_lstdelone2(t_list **lst, t_simpleCmd *simpleCmd, \
												size_t i, t_cmd *cmd);
void				ft_parse(t_list *lst, t_simpleCmd *simpleCmd, \
														size_t i, t_cmd *cmd);
void				ft_parse_redir_out(t_list *lst, t_simpleCmd *simpleCmd, \
																	size_t i);
void				ft_parse_redir_in(t_list *lst, t_simpleCmd *simpleCmd, \
																	size_t i);
void				ft_parse_redir_heredoc(t_list *lst, \
										t_simpleCmd *simpleCmd, t_cmd *cmd);
void				ft_parse_redir_err(t_list *lst, \
											t_simpleCmd *simpleCmd, size_t i);
int					ft_check_open_error(int fdin, int fdout, t_cmd *cmd);
int					ft_check_close_error(int fd);
void				ft_error_msg(char *infile);
void				ft_error_msg2(char *str);
void				ft_error_msg3(char *str);
void				ft_error(char *const str);
void				ft_error_heredoc(char *const str, int line_count);
int					find_length(int n);
char				*ft_itoa(int n);
void				ft_restore_original_in_and_out(t_settings *set);

void				ft_first_simplecmd_w_infile(t_settings *set, t_cmd *cmd);
int					ft_set_fdin_for_first_s_cmd_w_infile(t_settings *set, \
										t_cmd *cmd, int flag_random_heredoc);
void				ft_outfiles_before_nofile(t_settings *set, t_cmd *cmd);
void				ft_create_pipe(t_settings *set);
void				ft_open_outfiles(t_settings *set, t_cmd *cmd);

void				ft_regular_simplecmd(t_settings *set, t_cmd *cmd);
void				ft_open_infiles(t_settings *set, t_cmd *cmd);
int					ft_set_fdin_for_regular_simplecmd(t_settings *set, \
										t_cmd *cmd, int flag_random_heredoc);

void				ft_last_simplecmd(t_settings *set, t_cmd *cmd);
void				ft_open_outfiles_in_last_but_not_first_simplecmd(\
										t_settings *set, t_cmd *cmd, int k);
int					ft_set_fdin_in_last_but_not_first_simplecmd(\
						t_settings *set, t_cmd *cmd, int flag_random_heredoc);
void				ft_set_fdin_error_msg(t_settings *set, t_cmd *cmd);

void				ft_heredoc_interaction(t_cmd *cmd, size_t i, int mode);
int					ft_open_heredoc_hidden_file(int mode, int fd);
void				ft_get_eof(t_cmd *cmd, size_t i, char *line_heredoc, \
																	int fd);
void				ft_recursiv(t_cmd *cmd, size_t i, int fd);
void				ft_fill_heredocument(int fd, char *line_heredoc);
int					ft_last_heredoc(t_cmd *cmd, char *line_heredoc, size_t i);
void				ft_add_history_and_free_rl(char *line_heredoc);
void				ft_heredoc_input_is_null(t_cmd *cmd, size_t i);
int					ft_exit_status(pid_t last_pid, t_settings *set);

int					ft_setting_redirections_and_pipes(t_param_sr para);

void				ft_child_process(t_settings *set, t_param_sr para);

void				ft_set_fdin_for_first_simplecmd(t_settings *set, \
																t_cmd *cmd);
void				ft_redirect_input(t_settings *set, t_cmd *cmd);
void				ft_redirect_output(t_settings *set);
void				ft_save_in_and_out(t_settings *set);
void				ft_restore_original_in_and_out(t_settings *set);
void				ft_open_outfiles_in_last_but_not_first_simplecmd(\
										t_settings *set, t_cmd *cmd, int k);
void				ft_open_outfiles(t_settings *set, t_cmd *cmd);
void				ft_free_in_child(t_cmd *cmd, t_data *data, char *line);

t_expand			*ft_struct_init_expand(t_expand **exp);
int					ft_get_token_quoting_rule2(char *str, size_t i, \
								int *quoting_rule, int *quoting_rule_adequate);
void				ft_tag_ambigeous_for_redir(t_list *lst_token);
int					ft_is_expand_here(t_list *lst_token, char *str, \
												char *buffer, char *envp[]);
char				*ft_get_scope_expand(size_t end_expand_pos, \
					size_t start_expand_pos, char *str, int *flag_expand_here);
int					ft_is_expand_to_substitute_redir(t_list *lst_token);
int					ft_tag_ambiguous_redir(t_list *lst_token);
int					ft_expand_exists(t_list *lst_token);
char				*ft_dequote(char *str);
void				ft_modify_lst_token(t_list *lst_token);
void				ft_expand_and_retokenize(t_list *lst_token, char *envp[]);
int					ft_is_expand_to_substitute(t_list *lst_token, char *envp[]);
int					ft_is_expand_unvalidated(char **invalidators, char c);
int					ft_strcmp_char(char c1, char c2);
void				ft_free_struct_t_expand(t_expand **exp);
char				*ft_substitute(char *expand, char *envp[], t_expand *exp);
char				*ft_get_var(char **envp, char *expand, t_expand *exp);
int					ft_get_token_quoting_rule3(t_param_qr2 para, \
								char *buffer_dequote);
t_list				*retok_dequot_1(t_cmd *cmd, t_list *start_lst_token, \
													t_simpleCmd *simpleCmd);
t_data				*ft_retokenize_and_dequote_token_2(t_cmd *cmd, \
						t_list *start_lst_token, t_simpleCmd *simpleCmd, \
																t_data *data2);
void				ft_lstdelone_beta(t_list **lst);
void				ft_aff_list_ptr_sur_char_content2(t_list *alst);
t_list				*ft_lstnew_for_lst_retokenized2(t_list *token);
t_list				*ft_lstnew_for_lst_retokenized(\
										t_list *start_lst_token_retokenized);
t_list				*ft_lstfind(t_list *alst, int position);
int					parse_cmdarg_scmd_frm_retoknzd_lst(\
						t_list *lst_token_retokenized, t_simpleCmd *simpleCmd);
void				ft_count_final_nb_of_tokens_in_simplecmd_retokenized(\
							t_list *start_lst_token, t_simpleCmd *simpleCmd);
void				ft_aff_list_ptr_sur_char_content3(t_list *alst);
t_list				*ft_lstnew_for_lst2(t_data *data);
t_list				*ft_lstnew_for_lst_token_copy(t_list *tmp);
size_t				ft_get_end_expand(char *str, t_expand *exp, \
												char **expand, size_t i);
int					ft_strcmp_char(char c1, char c2);
int					ft_is_expand_unvalidated(char **invalidators, char c);
int					ft_isdigit(char c);
int					ft_isalpha(int c);
int					ft_is_alphanum(char c);
size_t				ft_isunderscore(char *str, size_t i);
char				*ft_epur_buffer_ws(char *buffer);
char				*ft_strtrim(char const *s1, char const *set);

void				ft_get_start_expand(char *str, t_expand *exp, size_t i, \
															char *buffer);
char				*ft_substitute(char *expand, char *envp[], t_expand *exp);
void				ft_check_expand_for_tag_ambigeous(char *expand, \
											t_expand *exp, t_list *lst_token);
void				ft_check_dollar_to_remove_before_sq(char *str, \
													t_expand *exp, size_t i);
t_list				*ft_lstfind(t_list *alst, int position);
char				*ft_substr(char const *s, unsigned int start, size_t len);

t_data				*ft_copy_token(t_list *tmp, t_data *data2);
t_data				*ft_retokenize(t_list *tmp, t_data *data2);
t_data				*ft_trim_and_clear2(char *line, t_data *data2);
size_t				ft_tokenize2(char *str, char *line, size_t i, \
															t_data *data);
int					ft_get_token_quoting_rule4(char *str, size_t i, \
								int *quoting_rule, int *quoting_rule_adequate);
int					ft_get_token_content2(t_data *data, int start_token_pos, \
												int end_token_pos, char *line);
t_data				*ft_retokenize_and_dequote_token_2(t_cmd *cmd, \
						t_list *start_lst_token, t_simpleCmd *simpleCmd, \
																t_data *data2);
t_list				*retok_dequot_1(t_cmd *cmd, t_list *start_lst_token, \
													t_simpleCmd *simpleCmd);

void				ft_aff_listenv_ptr_sur_char_content(t_listenvp *alst);
void				ft_lstadd_back_envp(t_listenvp **alst, t_listenvp *new);
t_listenvp			*ft_lstlast_envp(t_listenvp *lst);
t_listenvp			*ft_lstnew_for_lst_envp(char *str);
void				ft_add_var_to_lst_envp(t_list *tmp, t_listenvp *lst_envp);
void				ft_add_var_to_lst_envp_d(t_list *tmp, \
														t_listenvp *lst_envp_d);
int					ft_is_var_already_in_lst_envp_d(\
											t_listenvp *lst_envp_d, char *str);
int					ft_is_var_already_in_lst_envp(\
											t_listenvp *lst_envp, char *str);
int					ft_key_value_is_valid(t_list *tmp, t_simpleCmd *simpleCmd);
int					ft_check_format_is_key_value(t_list *tmp);

void				ft_free_struct_t_list_lst_envp(t_listenvp **lst_envp);
t_listenvp			*ft_get_lst_envp2(char **envp);
void				ft_aff_tab_envp(char **tab);
char				**ft_lst_to_tab(t_listenvp *lst_envp);
size_t				ft_count_keys_in_lst_envp(t_listenvp *lst_envp);
t_data_env			*ft_struct_init_data_env(t_data_env **data_env);
void				ft_free_struct_t_data_env(t_data_env **data_env);
int					ft_export_x(char **envp);
int					ft_isalpha(int c);
int					ft_isalnum(int c);
int					ft_isdigit(char c);
void				ft_check_export(t_cmd *cmd, \
						t_list *start_lst_token_retokenized, \
							t_simpleCmd *simpleCmd, t_data_env *data_env);
void				ft_aff_list_envp_sur_char_content(t_listenvp *alst);
void				ft_aff_list_envp_d_sur_char_content(t_listenvp *alst);
t_listenvp			*ft_add_double_quote_to_envp_d(t_listenvp *lst_envp_d);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
int					ft_is_var_already_in_lst_envpd2(\
										t_listenvp *lst_envp, char *str);
void				ft_check_unset(t_cmd *cmd, \
							t_list *start_lst_token_retokenized, \
								t_simpleCmd *simpleCmd, t_data_env *data_env);
int					ft_unset_value_is_valid(t_list *tmp);
void				ft_remove_var_from_lst_envp(t_list *tmp, \
													t_data_env *data_env);
void				ft_remove_var_from_lst_envp_d(t_list *tmp, \
													t_data_env *data_env);
int					ft_is_var_to_unset_already_in_lst_envp(\
											t_data_env *data_env, char *str);
int					ft_is_var_to_unset_already_in_lst_envp_d(\
											t_data_env *data_env, char *str);
t_listenvp			*delnod(t_listenvp *alst, char *str);
int					ft_count_n_for_key(char *str);
void				ft_check_env(t_cmd *cmd, \
						t_list *start_lst_token_retokenized, \
								t_simpleCmd *simpleCmd, t_data_env *data_env);
void				ft_check_echo(t_cmd *cmd, \
						t_list *start_lst_token_retokenized, \
								t_simpleCmd *simpleCmd, t_data_env *data_env);
void				ft_error_msg4(char *str);
void				ft_error_msg5(char *infile);
void				ft_check_cd(t_cmd *cmd, \
						t_list *start_lst_token_retokenized, \
								t_simpleCmd *simpleCmd, t_data_env *data_env);
void				ft_cd_no_option(t_data_env *data_env, char *str, \
													t_simpleCmd *simpleCmd);
char				*get_var_in_lst_envp_for_cd(\
											t_data_env *data_env, char *str);
char				*ft_strchr(const char *s, int c);
int					ft_check_is_space(char *str);
int					ft_is_space(int c);
void				ft_update_oldpwd_and_pwd_in_lst_envp(\
											t_data_env *data_env, char *cwd);
void				ft_add_var_oldpwd_to_lst_envp_d_for_cd_dash(\
											char *str, t_listenvp *lst_envp_d);
t_listenvp			*ft_lstfind_content(t_listenvp *alst, char *keyval);
int					ft_is_var_to_unset_already_in_lst_envp(\
											t_data_env *data_env, char *str);
int					ft_is_var_to_unset_already_in_lst_envp_d(\
											t_data_env *data_env, char *str);
int					ft_is_var_already_in_lst_envp_d( \
										t_listenvp *lst_envp_d, char *str);
void				ft_update_oldpwd_and_pwd_in_lst_envp_for_cd_dash_dash(\
										t_data_env *data_env, char *cwd);
int					ft_change_directory_for_cd_dash_dash(\
						t_data_env *data_env, char *new_path, \
													t_simpleCmd *simpleCmd);
t_listenvp			*ft_lstfind_content(t_listenvp *alst, char *keyequal);

void				ft_cd_option_slash(t_data_env *data_env, char *str, \
													t_simpleCmd *simpleCmd);
void				ft_check_pwd(t_cmd *cmd, \
						t_list *start_lst_token_retokenized, \
								t_simpleCmd *simpleCmd, t_data_env *data_env);
int					ft_get_pwd(t_data_env *data_env, t_simpleCmd *simpleCmd);

void				ft_check_exit(t_cmd *cmd, \
						t_list *start_lst_token_retokenized, \
								t_simpleCmd *simpleCmd, t_data_env *data_env);
int					ft_exit_is_arg_valid(char *str, t_simpleCmd *simpleCmd);
unsigned long long	ft_atoi_modulo(char *s, t_simpleCmd *simpleCmd);
int					ft_numeric_arg_required_msg(\
											t_simpleCmd *simpleCmd, char *str);
void				ft_too_many_arg_msg(t_simpleCmd *simpleCmd);

void				ft_error_msg6(char *infile);

t_listenvp			*ft_get_exit_status(t_listenvp **alst, char *key, \
														int exit_status);
void				ft_check_tab_envp_removed(char **tab);

void				handler_sigint(int num);
void				ft_set_exit_code_in_lst_envp(void *lst_envp, \
														int flag, int num);
int					ft_get_token_quoting_rule2b(char *str, size_t i, \
								int *quoting_rule, int *quoting_rule_adequate);
int					ft_atoi(char *str);
char				*ft_get_var_dollar_quest(char *key_value);
int					ft_get_exit_status_atoi(t_listenvp *lst_envp);

void				ft_error_msg2b(char *str);
void				ft_error_msg2c(char *str);
int					ft_check_bash_syntax_error_caracteres_volee(\
														t_list *lst_token);
int					ft_chevron(t_list *tmp);
int					ft_pipe(t_list *tmp, int *flag_pipe);
int					ft_check_double_points_token(t_list *tmp);
int					ft_strcmp(char *s1, char *s2);
int					ft_is_check_for_token(t_list *lst_token, char *str);
int					ft_exclamation(t_list *tmp);
int					ft_ampersand(t_list *tmp);

void				ft_check_cd(t_cmd *cmd, \
						t_list *start_lst_token_retokenized, \
								t_simpleCmd *simpleCmd, t_data_env *data_env);
void				ft_cd_no_option(t_data_env *data_env, char *str, \
													t_simpleCmd *simpleCmd);
void				ft_cd_option_dash(t_data_env *data_env, char *str, \
													t_simpleCmd *simpleCmd);
void				ft_cd_option_dash_dash(t_data_env *data_env, char *str, \
													t_simpleCmd *simpleCmd);
void				ft_cd_option_slash(t_data_env *data_env, char *str, \
													t_simpleCmd *simpleCmd);
int					ft_check_path(t_data_env *data_env, char *new_path, \
													t_simpleCmd *simpleCmd);
int					ft_change_directory_for_cd_dash_dash(\
						t_data_env *data_env, char *new_path, \
													t_simpleCmd *simpleCmd);
int					ft_change_directory(t_data_env *data_env, char *new_path, \
													t_simpleCmd *simpleCmd);
int					ft_change_directory_for_cd_dash(t_data_env *data_env, \
									char *new_path, t_simpleCmd *simpleCmd);
int					ft_change_directory_for_cd_slash(t_data_env *data_env, \
									char *new_path, t_simpleCmd *simpleCmd);
void				ft_update_oldpwd_and_pwd_in_lst_envp(\
											t_data_env *data_env, char *cwd);
void				ft_update_oldpwd_and_pwd_in_lst_envp_for_cd_dash(\
					t_data_env *data_env, char *cwd, t_simpleCmd *simpleCmd);
void				ft_add_var_oldpwd_to_lst_envp_d_for_cd_dash(char *str, \
													t_listenvp *lst_envp_d);

char				*get_var_in_lst_envp_for_cd(\
											t_data_env *data_env, char *str);
char				*ft_strchr(const char *s, int c);
int					ft_check_is_space(char *str);
int					ft_is_space(int c);
t_listenvp			*ft_lstfind_content(t_listenvp *alst, char *keyequal);
int					ft_is_var_already_in_lst_envp_for_cd(\
											t_listenvp *lst_envp, char *str);

t_list				*ft_lstnew_for_lst_token_copy(t_list *tmp);

void				ft_set_exit_code_in_lst_envp(\
										void *lst_envp, int flag, int num);
int					ft_get_exit_status_atoi(t_listenvp *lst_envp);
int					ft_atoi(char *str);
char				*ft_get_var_dollar_quest(char *key_value);
t_listenvp			*ft_get_lst_envp(char **envp);

void				ft_signal_setup(char **envp);
int					ft_no_line(t_data_env *data_env);
void				ft_reinit_setup(t_data_env *data_env, \
						int *exit_syntax, char **envp_tab, t_list *lst_token);
void				ft_init_setup(t_data_env *data_env, char **envp, \
										char **envp_tab, t_list *lst_token);

void				ft_execute_command(t_param_m *para, char **envp);
void				ft_free_and_exit_child(t_settings *set, t_param_sr *para);
void				ft_cwd_error(t_simpleCmd *simpleCmd);
void				ft_lst_add_back_var(t_data_env *data_env, char *oldpwd, \
											char *newpwd, t_listenvp *new);
void				ft_add_var_oldpwd_to_lst_envp_d_for_cd_dash(\
										char *str, t_listenvp *lst_envp_d);
int					ft_skip_quote6(t_param_qr2 *para);
int					ft_skip_quote7(t_param_qr2 *para);
int					ft_skip_quote(t_param_qr2 *para, char c);
int					ft_skip_quote2(t_param_qr2 *para, char c);
int					ft_skip_quote3(t_param_qr2 *para, char c);
int					ft_skip_quote4(t_param_qr2 *para, char c);
int					ft_skip_quote5(t_param_qr2 *para, char c);
void				ft_retokenize_last(t_list *tmp, \
						t_simpleCmd *simpleCmd, int flag_retokenize, \
															t_data **data2);
void				ft_retokenize_middle(t_simpleCmd *simpleCmd,
						t_data **data2, t_list **tmp, int *flag_retokenize);
void				ft_retokenize_first(char *check_dequote, \
					t_simpleCmd *simpleCmd, int *flag_retokenize, t_list *tmp);
void				ft_free_keys(char **key_input, char **key);
int					ft_quoting_rule_4(t_list *lst_token, char c, char *str, \
																size_t i);
int					ft_quoting_rule_3(t_list *lst_token, char c, char *str, \
																size_t i);
int					ft_quoting_rule_2(t_list *lst_token, char c, char *str, \
																size_t i);
int					ft_quoting_rule_1(t_list *lst_token, char c, char *str, \
																size_t i);
int					ft_quoting_rule_0(t_list *lst_token);
int					ft_get_end_expand_rule_4(char *str, char **expand, \
													t_expand *exp, size_t i);
int					ft_get_end_expand_rule_3(char *str, char **expand, \
													t_expand *exp, size_t i);
int					ft_get_end_expand_rule_2(char *str, char **expand, \
													t_expand *exp, size_t i);
int					ft_get_end_expand_rule_1(char *str, char **expand, \
													t_expand *exp, size_t i);
void				ft_get_end_expand_rule_0(char *str, char **expand, \
													t_expand *exp, size_t i);
int					ft_get_end_expand_rule_5(char *str, char **expand, \
													t_expand *exp, size_t i);
int					ft_get_end_expand_rule_6(char *str, char **expand, \
													t_expand *exp, size_t i);
int					ft_get_end_expand_rule_7(char *str, char **expand, \
													t_expand *exp, size_t i);
int					ft_quoting_rule_7(t_list *lst_token, char c, char *str, \
																	size_t i);
int					ft_quoting_rule_6(t_list *lst_token, char c, char *str, \
																	size_t i);
int					ft_quoting_rule_5(t_list *lst_token, char c, char *str, \
																	size_t i);

int					ft_strncmp(const char *s1, const char *s2, size_t n);
void				handler_sigquit(int num);
int					ft_is_flag_n(char *str);
void				ft_echo(t_simpleCmd *simpleCmd);
void				ft_free_and_exit_child(t_settings *set, \
															t_param_sr *para);
void				ft_aff_export(t_listenvp *tmp, t_param_sr para, \
															t_settings *set);
void				ft_builtin_export_w_option(t_settings *set, \
															t_param_sr para);
void				ft_builtin_cd(t_settings *set, t_param_sr para);
void				ft_builtin_env(t_settings *set, t_param_sr para);
void				ft_builtin_export_declare_x(t_settings *set, \
															t_param_sr para);
void				ft_cmd_not_found(int exec_return, t_settings *set, \
															t_param_sr para);
void				ft_builtin_pwd(t_settings *set, t_param_sr para);
void				ft_builtin_echo(t_settings *set, t_param_sr para);
void				ft_builtin_exit(t_settings *set, t_param_sr para);
void				ft_builtin_unset(t_settings *set, t_param_sr para);
void				ft_flag_empty_head_list(t_cmd *cmd, t_settings_del *del);
t_settings_del		*ft_del_empty_in_middle(t_list *curr, t_cmd *cmd, \
									t_list *lst_token, t_settings_del *del);
t_list				*ft_middle_empty_token(t_list *curr, t_cmd *cmd, \
														t_list *lst_token, \
													t_settings_del *del);
t_settings_del		*ft_del_empty_in_head(t_list *curr, t_cmd *cmd, \
									t_list *lst_token, t_settings_del *del);
t_list				*ft_head_empty_token(t_list *curr, t_cmd *cmd, \
														t_list *lst_token, \
														t_settings_del *del);
void				ft_struct_init_simplecmd_1(t_simpleCmd **simpleCmd);
void				ft_struct_init_simplecmd_2(t_simpleCmd **simpleCmd);
t_simpleCmd			*ft_struct_init_simplecmd(t_simpleCmd **simpleCmd);
void				ft_reset_positions2(t_data *data, size_t i);
void				ft_reset_positions(t_data *data);
t_listenvp			*ft_delete_node_first(t_listenvp *alst, \
													t_listenvp *to_remove);
void				ft_delete_key(char *key);
void				ft_free_1(t_simpleCmd **simpleCmd);
void				ft_free_2(t_simpleCmd **simpleCmd);
void				ft_init(t_param_m *para, char **envp);
void				ft_reinit(t_param_m *para);
void				ft_get_para_exit_status(t_param_m *para);
void				ft_init_t_param(t_param_m *para);
void				ft_execute_scmds(t_param_m *para);
#endif
