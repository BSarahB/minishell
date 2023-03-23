/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 15:10:59 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/02/07 15:11:02 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#define mode_push 0

int main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	char *line;
	t_cmd *cmd;
	char **blocks;
	char **token_tab;

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


		
		//LE LEXING DEVRAIT ETRE FAIT DE MANIERE SEQUENTIELLE car :
		//ON NE VA PAS POUVOIR SPLIT SUR whitespaces car si on a un operateur comme echo b |cat-e on veut que le pipe soit un token a part
		//ANSI le lexing doit se faire de MANIERE SEQUENTIELLE  meme un "    " ca ne fonctionnera pas
		if(mode_push == 0){
			(void)envp;
			ft_check_validity_of_line_input(line);
			ft_check_input_cases_for_return_empty_prompt(line);
			ft_check_bash_syntax_error_caracteres_volee(line);
			token_tab = ft_tokenize_line(line);
			
			
			//ft_tab_to_lst(token_tab);
		}
		else {
			(void) token_tab;
		blocks = ft_split(line, '|');
		cmd = ft_struct_init(&cmd, 0, blocks);
		cmd->path_tab = ft_get_path(envp);
		ft_split_line_in_s_cmd(cmd, line, envp);
		ft_setting_redirections_and_pipes(cmd, envp);
		ft_free_struct_str(&line);
		ft_free_struct_t_cmd(&cmd);
		}
	}
	ft_free_struct_t_cmd(&cmd);
	return (0);
}
