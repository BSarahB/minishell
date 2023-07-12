/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 14:10:40 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/03/13 14:10:43 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_execve_join(t_cmd *cmd, char **envp, char **abs_cmd_and_args)
{
	char	*path_cmd_joined;
	int		i;
	int		exec_return;

	exec_return = 0;
	i = -1;
	while ((*cmd).path_tab[++i])
	{
		path_cmd_joined = ft_strjoin((*cmd).path_tab[i], abs_cmd_and_args[0]);
		exec_return = execve(path_cmd_joined, abs_cmd_and_args, envp);
		free(path_cmd_joined);
		if (exec_return != -1)
			break ;
	}
	return (exec_return);
}

int	ft_execute_cmd(t_cmd *cmd, int i,char *envp[])
{
	// int execve(const char *pathname, char *const argv[], char *const envp[]);
	int exec_return;

	exec_return = 0;
	(void)cmd;

	if (execve(cmd->simpleCmds[i]->cmd_and_args[0], cmd->simpleCmds[i]->cmd_and_args, envp) == -1)
		exec_return = ft_execve_join(cmd, envp, cmd->simpleCmds[i]->abs_cmd_and_args);
	if (exec_return == -1 && (errno == 2 || errno == 13))
		{
			cmd->simpleCmds[i]->errnum = 127;
			ft_putstr_fd(cmd->simpleCmds[i]->cmd_and_args[0], 2);
			ft_putstr_fd(": command not found: ", 2);
			ft_putstr_fd("\n", 2);
			ft_free_struct_t_cmd(&cmd);
			return(exec_return);

		}
	//printf("%d\n",((cmd->simpleCmds[0])->errnum));
		//est ce judicieux de free ici? ou ne vaut il mieux pas free tout a la fin?
	printf("freee\n");
	ft_free_tab(&(cmd->simpleCmds[i]->cmd_and_args));
	ft_free_tab(&(cmd->simpleCmds[i]->abs_cmd_and_args));
	return (exec_return);

}