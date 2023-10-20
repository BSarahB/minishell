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


int ft_export_x(char **envp)
{
	int i;

	i = 0;
	if(envp == NULL)
		return (0); //TODO CLARIFIER LE ENV VIDE

	while(envp[i])
	{

		printf("declare -x %s\n", envp[i]);

		/*
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(envp[i], STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		*/
		i++;
	}

	return(0); //tt s est bien passe, declar x a ete affiche
}


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
		path_cmd_joined = NULL;
		if (exec_return != -1)
			{
			//	free(path_cmd_joined);
				break ;
			}
	}
	
	return (exec_return);
}

// int execve(const char *pathname, char *const argv[], char *const envp[]);
int ft_execute_cmd(t_cmd *cmd, int i, char *envp[], t_settings *set)
{
	int exec_return;

	exec_return = 0;
	if(cmd->simpleCmds[i] == NULL || cmd->simpleCmds[i]->cmd_and_args == NULL)
	{
		close(set->savein);
		close(set->saveout);
		return(exec_return);
	}
	if (execve(cmd->simpleCmds[i]->cmd_and_args[0], cmd->simpleCmds[i]->cmd_and_args, envp) == -1)
			exec_return = ft_execve_join(cmd, envp, cmd->simpleCmds[i]->abs_cmd_and_args);
				
	if (exec_return == -1 && (errno == 2 || errno == 13))
		{
			cmd->simpleCmds[i]->errnum = 127;
			ft_putstr_fd(cmd->simpleCmds[i]->cmd_and_args[0], 2);
			ft_putstr_fd(": command not found", 2);
			ft_putstr_fd("\n", 2);
			return(exec_return);
		}
	ft_free_tab(&(cmd->simpleCmds[i]->cmd_and_args));
	ft_free_tab(&(cmd->simpleCmds[i]->abs_cmd_and_args));

	return (exec_return);
}
