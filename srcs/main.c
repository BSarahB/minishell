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

	line = NULL;
	while (1)
	{
		signal(SIGQUIT, SIG_IGN);
   		line = readline(" ~ ");
		if (!line)
		{
			ft_putstr_fd("no line quit \n", 2);
			break;
		}
		add_history(line);
		ft_check_prerequesite_of_line_input(line);
		ft_check_input_cases_for_return_empty_prompt(line);
		ft_check_bash_syntax_error_caracteres_volee(line);
		lst_token = ft_tokenize_line_b(line);
		cmd = ft_struct_init_cmd(&cmd, 0, lst_token);
		cmd->path_tab = ft_get_path(envp);	
		ft_parse_tokens_in_s_cmd(cmd, line, envp, lst_token);
		ft_setting_redirections_and_pipes(cmd, envp);
		ft_free_struct_str(&line);
		ft_free_struct_t_cmd(&cmd);
	}
	ft_free_struct_t_cmd(&cmd);
	return (0);
}