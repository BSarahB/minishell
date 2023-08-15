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

int main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	char *line;
	t_cmd *cmd;
	t_list *lst_token;
	t_data *data;
	int	exit_status;

	exit_status = 0;
	line = NULL;
	cmd = NULL;
	while (1)
	{
		signal(SIGQUIT, SIG_IGN);
   		line = readline(" ~ ");
		if (!line)
		{
			// on passe ici avec CTRL D
			break;
		}
		add_history(line);
		ft_check_prerequesite_of_line_input(line);
		ft_check_input_cases_for_return_empty_prompt(line);
		//ft_check_bash_syntax_error_caracteres_volee(line);
		data = ft_tokenize_line(line);
		lst_token = data->lst_token;
		if(ft_check_bash_syntax_error_caracteres_volee(lst_token) == 0)
		{	
			cmd = ft_struct_init_cmd(&cmd, 0, lst_token);
			cmd->path_tab = ft_get_path(envp);	
			ft_parse_tokens_in_s_cmd(cmd, line, envp, lst_token);
			exit_status = ft_setting_redirections_and_pipes(cmd, envp, data);
			//printf("exit_status = %d\n", exit_status);
		}
			ft_free_struct_str(&line);
			if (cmd != NULL)
				ft_free_struct_t_cmd(&cmd);
			else
				{
					if(lst_token)
						ft_free_struct_t_list_lst_token(&lst_token);
				}

			ft_free_struct_t_data(&data);

	}
	//ft_free_struct_t_cmd(&cmd);
	return (exit_status);
}
//idee: creer une liste chainee ou je mets les pointeurs qui viennent d etre malloc, et a la fin je free depuis la fin tous les pointeurs en remontant jusquau debut de la liste
