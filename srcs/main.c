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

void	ft_free(t_cmd *cmd, t_list *lst_token, t_data *data, char *line)
{
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


//TODO:
/*~ ls << A| wc <<B
 > A
 > B
/wc: 'standard input': Bad file descriptor
/wc: -: Bad file descriptor
/wc: write error
*/
size_t	ft_strlen(const char *s)
{
	size_t i;

	i = 0;
	if(s == NULL)
		return (0);
	while (s[i])
		i++;
	return (i);
}

void	ft_heredoc_interaction(t_cmd *cmd, size_t i, int mode)
{
	char	*line_heredoc;
	int		fd;

	//fd = -1;
	line_heredoc = NULL;
	if(mode == 1)
	{
		//if(fd != -1)
		//	close(fd)
		fd = open(".heredoc", O_CREAT | O_RDWR | O_APPEND, 0644);
	}
	if(mode == 2)
	{
		//if(fd != -1)
		//	close(fd)
		fd = open(".heredoc", O_CREAT | O_RDWR | O_TRUNC, 0644);
	}

	if(fd == -1)
	{
		perror("minishell");//ERROR ouverture .heredoc
		exit(1);
	}
	while (1)
	{
		signal(SIGQUIT, SIG_IGN);
   		line_heredoc = readline(" > ");
		cmd->line_count++;//TODO line_count doit s accumuler jusqu a quand on quitte le programme
		if(line_heredoc != NULL)
		{
		
			if(ft_strcmp(line_heredoc, cmd->heredocs[i]) == 0)
			{
				if(i  == cmd->k -1)
				{
				//	close(fd);
					break;
				}
				else
					{
						if(cmd->heredocs[++i])
						{
							close(fd);
							ft_heredoc_interaction(cmd, i, 2);
						}
						break;
					}
			}
			write(fd, line_heredoc, ft_strlen(line_heredoc));
			write(fd, "\n", 1);
		}
		if (!line_heredoc)
		{
			//if (cmd->line_count == 1) pour quand le CTLR
			cmd->line_count = cmd->line_count -1;
						// on passe ici avec CTRL D
			//IL FAUT CONSIDERER LE FICHIER COMME UN INFILE MALGRE LE CTLD et executer la commande
			//SI HEREDOCS[i] est le dernier on execute la cmd line on break
			ft_error_heredoc(cmd->heredocs[i], cmd->line_count);
			if(cmd->heredocs[++i])
				ft_heredoc_interaction(cmd, i, 2);
			break;
		}
		add_history(line_heredoc); //on est 
	}

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
			if(cmd->nb_of_heredocs != 0)
				ft_heredoc_interaction(cmd, 0, 1);

			exit_status = ft_setting_redirections_and_pipes(cmd, envp, data);
			//printf("exit_status = %d\n", exit_status);
		}
		ft_free(cmd, lst_token, data, line);

	}
	//ft_free_struct_t_cmd(&cmd);
	return (exit_status);
}
//idee: creer une liste chainee ou je mets les pointeurs qui viennent d etre malloc, et a la fin je free depuis la fin tous les pointeurs en remontant jusquau debut de la liste
