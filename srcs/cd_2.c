/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 17:08:03 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/29 17:08:21 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int ft_change_directory_for_cd_slash(t_data_env *data_env, char *new_path, t_simpleCmd *simpleCmd)
{
	char	buf[1096];
	char	*cwd;

	cwd = NULL;
	if (chdir(new_path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);//mettre en place TODO l erreur ERRNO le msg approprie errno
		if(errno == 14 || errno == 116)
			errno = ENOENT; // The directory specified in path does not exist.
		ft_putstr_fd(new_path, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);//mettre en place TODO l erreur ERRNO le msg approprie errno
		simpleCmd->exit_code = 1;
		return (errno);
	}
	cwd = getcwd(buf, 1096); //si error du return : errno is set
	if (!cwd)
	{
		ft_putstr_fd("cd: error retrieving current directory, getcwd: cannot access parent directory", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		simpleCmd->exit_code = 1;
		return(errno);
	}
	ft_update_oldpwd_and_pwd_in_lst_envp_for_cd_dash_dash(data_env, cwd, simpleCmd);//faire une copie de PWD avant de la modifier en cwd
	return (0);
}

int ft_change_directory_for_cd_dash(t_data_env *data_env, char *new_path, t_simpleCmd *simpleCmd)
{
	char	buf[1096];
	char	*cwd;
	(void)data_env;
	(void)simpleCmd;

	cwd = NULL;
	if (chdir(new_path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);//mettre en place TODO l erreur ERRNO le msg approprie errno
		if(errno == 14 || errno == 116)
			errno = ENOENT; // The directory specified in path does not exist.
		ft_putstr_fd(new_path, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);//mettre en place TODO l erreur ERRNO le msg approprie errno
		simpleCmd->exit_code = 1;
		return (errno);
	}
	cwd = getcwd(buf, 1096); //si error du return : errno is set
	if (!cwd)
	{
		ft_putstr_fd("cd: error retrieving current directory, getcwd: cannot access parent directory", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		simpleCmd->exit_code = 1;
		return(errno);
	}
	ft_update_oldpwd_and_pwd_in_lst_envp_for_cd_dash(data_env, cwd, simpleCmd);//faire une copie de PWD avant de la modifier en cwd
	return (0);
}

int ft_change_directory(t_data_env *data_env, char *new_path, t_simpleCmd *simpleCmd)
{
	char	buf[1096];
	char	*cwd;

	cwd = NULL;
	if (chdir(new_path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);//mettre en place TODO l erreur ERRNO le msg approprie errno
		if(errno == 14 || errno == 116)
			errno = ENOENT; // The directory specified in path does not exist.
		ft_putstr_fd(new_path, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);//mettre en place TODO l erreur ERRNO le msg approprie errno
		simpleCmd->exit_code = 1;
		return (errno);
	}
	cwd = getcwd(buf, 1096); //si error du return : errno is set
	if (!cwd)
	{
		ft_putstr_fd("cd: error retrieving current directory, getcwd: cannot access parent directory", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		simpleCmd->exit_code = 1;
		return(errno);
	}
	ft_update_oldpwd_and_pwd_in_lst_envp(data_env, cwd);
	return (1);
}

int ft_change_directory_for_cd_dash_dash(t_data_env *data_env, char *new_path, t_simpleCmd *simpleCmd)
{
	char	buf[1096];
	char	*cwd;
	(void)data_env;
	(void)simpleCmd;
	cwd = NULL;
	if (chdir(new_path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);//mettre en place TODO l erreur ERRNO le msg approprie errno
		if(errno == 14 || errno == 116)
			errno = ENOENT; // The directory specified in path does not exist.
		ft_putstr_fd(new_path, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);//mettre en place TODO l erreur ERRNO le msg approprie errno
		simpleCmd->exit_code = 1;
		return (errno);
	}
	cwd = getcwd(buf, 1096); //si error du return : errno is set
	if (!cwd)
	{
		ft_putstr_fd("cd: error retrieving current directory, getcwd: cannot access parent directory", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		simpleCmd->exit_code = 1;
		return(errno);
	}
	ft_update_oldpwd_and_pwd_in_lst_envp_for_cd_dash_dash(data_env, cwd, simpleCmd);//faire une copie de PWD avant de la modifier en cwd
	return (0);
}

int ft_check_path(t_data_env *data_env, char *new_path, t_simpleCmd *simpleCmd)
{
	char	buf[1096];
	char	*cwd;

	cwd = NULL;
	if (chdir(new_path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);//mettre en place TODO l erreur ERRNO le msg approprie errno
		if(errno == 14 || errno == 116)
			errno = ENOENT; // The directory specified in path does not exist.
		ft_putstr_fd(new_path, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);//mettre en place TODO l erreur ERRNO le msg approprie errno
		simpleCmd->exit_code = 1;
		return (errno);

	}
	cwd = getcwd(buf, 1096); //si error du return : errno is set
	if (!cwd)
	{
		ft_putstr_fd("cd: error retrieving current directory, getcwd: cannot access parent directory", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		simpleCmd->exit_code = 1;
		return(errno);
	}
	ft_update_oldpwd_and_pwd_in_lst_envp_for_cd_dash_dash(data_env, cwd, simpleCmd);//faire une copie de PWD avant de la modifier en cwd
	return (0);
}