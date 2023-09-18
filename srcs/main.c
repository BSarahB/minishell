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
			break;
		add_history(line);
		ft_check_prerequesite_of_line_input(line);
		ft_check_input_cases_for_return_empty_prompt(line);
		data = ft_tokenize_line(line);
		lst_token = data->lst_token;
		ft_aff_list_ptr_sur_char_content(lst_token);
		if(ft_check_bash_syntax_error_caracteres_volee(lst_token) == 0)
		{	
			//ft_modify_lst_token(lst_token);
			ft_expand_and_retokenize(lst_token);
			cmd = ft_struct_init_cmd(&cmd, lst_token);
			cmd->path_tab = ft_get_path(envp);	
			ft_parse_tokens_in_s_cmd(cmd, lst_token);
			if(cmd->nb_of_heredocs != 0)
				ft_heredoc_interaction(cmd, 0, 1);
			exit_status = ft_setting_redirections_and_pipes(cmd, envp, data, line);
		}
		ft_free(cmd, lst_token, data, line);
		ft_free_struct_t_cmd_only(&cmd);
	}
	return (exit_status);
}