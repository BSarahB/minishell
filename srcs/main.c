/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 15:10:59 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/03/07 15:11:02 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal;



char	*ft_get_var_dollar_quest(char *key_value)
{
	char	**var_content;
	char 	*var;

	var = NULL;
	var_content = NULL;
	
	var_content = ft_split(key_value, '=');
	if(var_content == NULL || var_content[0] == NULL)
		{
			ft_free_tab(&var_content);
			return(NULL);
		}
	var = ft_strdup(var_content[1]);
	ft_free_tab(&var_content);
	return (var);
}



int	ft_atoi(char *str)
{
	int	result;
	int	sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || *str == '\t' || *str == '\n' \
		 || *str == '\v' || *str == '\f')
		str++;
	if (*str == '-')
		sign = -1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str <= '9' && *str >= '0')
	{
		result = result * 10 + *str - '0';
		str++;
	}
	result = sign * result;
	return (result);
}



int 	ft_get_exit_status_atoi(t_listenvp *lst_envp)
{
    int    		exit_status_int;
    t_listenvp 	*tmp;
	char 		*val;

	val = NULL;
	exit_status_int = 0;
    tmp = lst_envp;
    while (tmp)
    {
        if (ft_strncmp((tmp)->key_value, "?=", 2) == 0)
        {
			val = ft_get_var_dollar_quest(tmp->key_value);
            exit_status_int = ft_atoi(val);
			ft_free_struct_str(&val);
            return(exit_status_int);
        }        
        tmp = tmp->next;
    }
    return(exit_status_int);
}

void ft_set_exit_code_in_lst_envp(void *lst_envp, int flag, int num)
{
	static t_listenvp *ptr;
	(void)num;
	//on recupere l adresse de lst_envp
	if(flag == 1)
		ptr = lst_envp;
	if(g_signal == HD_STOP)
		ptr = ft_get_exit_status(&ptr,"?=", 130);
	else if(num != -13)
		ptr = ft_get_exit_status(&ptr,"?=", num);
	

}

void handler_sigint(int num)
{

	(void)num;
	int fd_heredoc;

	fd_heredoc = 0;
	if(g_signal == IN_PROMPT)
	{
		ft_putstr_fd("\n", 0);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		ft_set_exit_code_in_lst_envp(NULL, 0, 130);
	}

	if(g_signal == IN_HD)
	{
		g_signal = HD_STOP;
	}
	if(g_signal == HD_STOP)
	{
		ft_putstr_fd("\n", 0);
		ft_set_exit_code_in_lst_envp(NULL, 0, 130);
		dup2(fd_heredoc , STDIN_FILENO);
		close(fd_heredoc);
		return;
	}
	if(g_signal == IN_S_CMD)
	{
		ft_putstr_fd("\n", 2);
	}


}


t_listenvp *ft_get_lst_envp(char **envp)
{
	static t_listenvp *lst_envp;
    t_listenvp *new;
    int         i;

    lst_envp = NULL;
    i = 0;
    new = NULL;
    if(envp)
    {
        while(envp[i])
        {
            new = ft_lstnew_for_lst_envp(envp[i]);
            ft_lstadd_back_envp(&lst_envp, new);
            i++;
        }
		//add ?=0 key=value pour le exit code
		new = ft_lstnew_for_lst_envp("?=0");
        ft_lstadd_back_envp(&lst_envp, new);
    }
    return(lst_envp);
}

int main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	char *line;
	t_cmd *cmd;
	t_list *lst_token;
	t_data *data;
	int	exit_status;
	int exit_syntax_error;
	int exit_syntax;
	int flag_save_envp;
	char **envp_t;
	char **envp_tab;
	t_data_env *data_env;

	exit_status = 0;
	exit_syntax_error = 0;
	exit_syntax = -13;
	line = NULL;
	cmd = NULL;
	data_env = NULL;
	envp_t = NULL;
	envp_tab = NULL;
	flag_save_envp = 1;
	data = NULL;
	lst_token = NULL;
	
	while (1)
	{
		g_signal = IN_PROMPT;
		signal(SIGQUIT, SIG_IGN);//on doit le mettre pour eviter les signaux envoyes via le terminal du style kill etc...
		signal(SIGINT, handler_sigint);
		ft_check_tab_envp_removed(envp);
		line = readline(" ~ ");
		if (!line) //CTRL D
			{
				ft_putstr_fd("exit\n",1);
				if(data_env)
				{
					if(data_env->lst_envp)
					{
						exit_status = ft_get_exit_status_atoi(data_env->lst_envp);
						printf("exit_status : %d", exit_status);
					}
				}
			//	else
			//		exit_status = 0;
				break;
			}
		add_history(line);
		ft_check_prerequesite_of_line_input(line);
		ft_check_input_cases_for_return_empty_prompt(line);
		data = ft_tokenize_line(line);
		lst_token = data->lst_token;
		exit_syntax_error = ft_check_bash_syntax_error_caracteres_volee(lst_token);
		if(exit_syntax_error == 0)
		{	
			if(flag_save_envp == 0)
			{
				//ft_aff_list_envp_sur_char_content(data_env->lst_envp);
				ft_set_exit_code_in_lst_envp(data_env->lst_envp, 1, exit_syntax);
				exit_syntax =-13;
				envp_tab = ft_lst_to_tab(data_env->lst_envp);
				ft_expand_and_retokenize(lst_token, envp_tab);
				if(envp_tab != NULL)
					ft_free_tab(&envp_tab);
			}
			if(flag_save_envp == 1)
				{
					ft_struct_init_data_env(&data_env);//_env(&data_env);
					data_env->lst_envp = ft_get_lst_envp(envp);
					data_env->lst_envp_d = ft_get_lst_envp(envp);
					ft_set_exit_code_in_lst_envp(data_env->lst_envp, 1, exit_syntax);
					exit_syntax = -13;
					flag_save_envp = 0;
					envp_tab = ft_lst_to_tab(data_env->lst_envp);
					ft_expand_and_retokenize(lst_token, envp_tab);
					if(envp_tab != NULL)
						ft_free_tab(&envp_tab);
				}
			cmd = ft_struct_init_cmd(&cmd, lst_token);
			cmd->path_tab = ft_get_path(envp);	//il faudra modifier cette fonction et recuperer path tab si jamais env -i ou unset PATH
			ft_parse_tokens_in_s_cmd(cmd, lst_token, data_env);
			if(cmd->nb_of_heredocs != 0)
				ft_heredoc_interaction(cmd, 0, 1);
			printf("g signal = %d\n", g_signal);
			if(g_signal != HD_STOP)
			{
				envp_t = ft_lst_to_tab(data_env->lst_envp);
				exit_status = ft_setting_redirections_and_pipes(cmd, envp_t, data, line, data_env);
			}
			
		}
		else
		{
				if(exit_syntax_error > 0)
					exit_syntax = exit_syntax_error;
				if(exit_syntax_error == -12)
					exit_syntax = 0;

		}
		
		ft_free(cmd, lst_token, data, line);
		ft_free_struct_t_cmd_only(&cmd);
		if(envp_t != NULL)
			ft_free_tab(&envp_t);
		//ft_aff_list_envp_sur_char_content(data_env->lst_envp);
		g_signal = IN_PROMPT;
	}
	if(data_env != NULL)
	{
		ft_free_struct_t_data_env(&data_env);
	}
	return (exit_status);
}
