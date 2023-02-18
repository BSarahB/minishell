#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>//pour open les 3 directives de preprocesseur
#include <sys/stat.h>
#include <fcntl.h>
# include <errno.h>
#include <sys/wait.h> //pour le waitpid



// description dune simple commande et ses arguments 

typedef struct s_simpleCmd{

	int number_of_arguments;
	int errnum;
	char **cmd_and_args;
	char **abs_cmd_and_args;


}				t_simpleCmd;

//description d une commande complete avec les multiples pipes eventuels et les IO redirections eventuels 

typedef struct s_cmd{
	
	int number_of_simpleCmds;
	t_simpleCmd **simpleCmds;
	t_simpleCmd *simpleCmd;

	char	**path_tab;


	char *outfile;
	char *inputfile;
	char *errfile;
	//a voir si on met tous les char * dans un double tab io_redirections(**)
	int background;

}				t_cmd;


//t_cmd *currentCmd;

int			ft_split_line_in_s_cmd(t_cmd *cmd, char *line, char **envp);
char		**ft_get_abs_arguments(int i, char **blocks);

size_t		ft_words_nbr(const char *s, char c);

char		**ft_create_tab(char const *s, char c, size_t words_nbr);
char		**ft_split(char const *s, char c);
char		*ft_strcpy(char *dst, char const *src);
char		*ft_init_string(size_t len);
char		*ft_strcat(char *dest, char const *src);
char		*ft_strjoin(char *s1, char const *s2);
char		*ft_strndup(char *src, int n);
char		*ft_update_string(char **str, char *new);
void		ft_free_struct_str(char **p);
void		ft_free_tab(char ***tab);


t_cmd		*ft_struct_init(t_cmd **cmd, char init_value, size_t simpleCmds_nbr);
t_simpleCmd	*ft_struct2_init(t_simpleCmd **ptr, char init_value);
t_simpleCmd	**ft_struct_array_init(t_simpleCmd **ptr, char init_value, size_t simpleCmds_nbr);
char		**ft_get_path(char **envp);

int			ft_execute_cmd(t_cmd *cmd, int i,char *envp[]);
void		ft_setting_redirections_and_pipes(t_cmd *cmd, char *envp[]);




char		*ft_init_cstring(char **str, size_t len, char init_value);
char		*ft_init_cstring(char **str, size_t len, char init_value);
void		*ft_memset(void *b, char c, size_t len);


void		ft_insert_argument(char *argument);
void		fr_insert_simpleCmd(t_simpleCmd *simpleCmd);

#endif
