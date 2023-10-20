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


void ft_set_exit_code_in_lst_envp(void *lst_envp, int flag)
{
	(void)flag;
	(void)*lst_envp;
	//on recupere l adresse de lst_envp
	//if(flag == 1)
	//	lst_envp = 

}

void handler_sigint(int num)
{
	(void)num;
	ft_putstr_fd("\n", 0);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	ft_set_exit_code_in_lst_envp(NULL, 0);
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
	int flag_save_envp;
	char **envp_t;
	char **envp_tab;
	t_data_env *data_env;

	exit_status = 0;
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
		signal(SIGQUIT, SIG_IGN);//on doit le mettre pour eviter les signaux envoyes via le terminal du style kill etc...
		signal(SIGINT, handler_sigint);
		ft_check_tab_envp_removed(envp);
		line = readline(" ~ ");
		if (!line) //CTRL D
			{
				ft_putstr_fd("exit\n",1);
				break;
			}
		add_history(line);
		ft_check_prerequesite_of_line_input(line);
		ft_check_input_cases_for_return_empty_prompt(line);
		data = ft_tokenize_line(line);
		lst_token = data->lst_token;
		if(ft_check_bash_syntax_error_caracteres_volee(lst_token) == 0)
		{	
			if(flag_save_envp == 0)
			{
				//ft_aff_list_envp_sur_char_content(data_env->lst_envp);
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
			envp_t = ft_lst_to_tab(data_env->lst_envp);
			exit_status = ft_setting_redirections_and_pipes(cmd, envp_t, data, line, data_env);
		}
		ft_free(cmd, lst_token, data, line);
		ft_free_struct_t_cmd_only(&cmd);
		if(envp_t != NULL)
			ft_free_tab(&envp_t);
		//ft_aff_list_envp_sur_char_content(data_env->lst_envp);
	
	}
	if(data_env != NULL)
	{
		ft_free_struct_t_data_env(&data_env);
	}
	return (exit_status);
}
