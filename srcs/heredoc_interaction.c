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
						ft_add_history_and_free_rl(line_heredoc);
						break;
					}
			}
			ft_fill_heredocument(fd, line_heredoc);
		}
		if (!line_heredoc) //CTRLD
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