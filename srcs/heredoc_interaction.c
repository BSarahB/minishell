/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_interaction.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 12:09:08 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/09/04 12:09:29 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_fill_heredocument(int fd, char *line_heredoc)
{
	write(fd, line_heredoc, ft_strlen(line_heredoc));
	write(fd, "\n", 1);
}

void	ft_recursiv(t_cmd *cmd, size_t i, int fd)
{
	if(cmd->heredocs[++i])
	{
		close(fd);//proteger le close
		ft_heredoc_interaction(cmd, i, 2);
	}
}

void ft_get_EOF(t_cmd *cmd, size_t i, char *line_heredoc, int fd)
{
	while (1)
	{
		signal(SIGQUIT, SIG_IGN);
   		line_heredoc = readline(" > ");
		cmd->line_count++;
		if(line_heredoc != NULL)
		{
			if(ft_strcmp(line_heredoc, cmd->heredocs[i]) == 0)
			{
				if(ft_last_heredoc(cmd, line_heredoc, i) ==1)
					break;
				else
					{
						ft_recursiv(cmd, i, fd);
						break;
					}
			}
			ft_fill_heredocument(fd, line_heredoc);
		}
		if (!line_heredoc)
		{
			ft_heredoc_input_is_null(cmd, i);
			break;
		}
	ft_add_history_and_free_rl(line_heredoc);
	}	
}

int ft_open_heredoc_hidden_file(int mode, int fd)
{
	if(mode == 1)
		fd = open(".heredoc", O_CREAT | O_RDWR | O_APPEND, 0644);
	if(mode == 2)
		fd = open(".heredoc", O_CREAT | O_RDWR | O_TRUNC, 0644);
	if(fd == -1)
	{
		perror("minishell");
		exit(1);
	}
	return(fd);
}

void	ft_heredoc_interaction(t_cmd *cmd, size_t i, int mode)
{
	char	*line_heredoc;
	int		fd;

	fd = -1;
	line_heredoc = NULL;
	fd = ft_open_heredoc_hidden_file(mode, fd);

	ft_get_EOF(cmd, i, line_heredoc, fd);
	if(fd)
		close(fd);
}


/* //Original avant factorisation
void	ft_heredoc_interaction(t_cmd *cmd, size_t i, int mode)
{
	char	*line_heredoc;
	int		fd;

	//fd = -1;
	//initialiser fd TODO
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
	if(fd)
		close(fd);
}


*/
