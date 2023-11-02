/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_4.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 18:21:29 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/31 18:21:45 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_add_var_oldpwd_to_lst_envp_d_for_cd_dash(char *str, \
t_listenvp *lst_envp_d)
{
	t_listenvp	*new;

	new = NULL;
	if (ft_is_var_already_in_lst_envp_d(lst_envp_d, str) == 0)
	{
		new = ft_lstnew_for_lst_envp(str);
		ft_lstadd_back_envp(&lst_envp_d, new);
	}
}

void	ft_lst_add_back_var(t_data_env *data_env, char *oldpwd, char *newpwd, \
t_listenvp *new)
{
	if (ft_is_var_already_in_lst_envp_for_cd(data_env->lst_envp, oldpwd) == 0)
	{
		new = ft_lstnew_for_lst_envp(oldpwd);
		ft_lstadd_back_envp(&data_env->lst_envp, new);
	}
	if (ft_is_var_already_in_lst_envp_for_cd(data_env->lst_envp, newpwd) == 0)
	{
		new = ft_lstnew_for_lst_envp(newpwd);
		ft_lstadd_back_envp(&data_env->lst_envp, new);
	}
}

void	ft_cwd_error(t_simpleCmd *simpleCmd)
{
	ft_putstr_fd("cd: error retrieving current directory, \
getcwd: cannot access parent directory", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	simpleCmd->exit_code = 1;
}
