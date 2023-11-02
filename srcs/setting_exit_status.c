/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setting_exit_status.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 02:04:42 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/30 02:04:59 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

extern int	g_signal;

char	*ft_get_var_dollar_quest(char *key_value)
{
	char	**var_content;
	char	*var;

	var = NULL;
	var_content = NULL;
	var_content = ft_split(key_value, '=');
	if (var_content == NULL || var_content[0] == NULL)
	{
		ft_free_tab(&var_content);
		return (NULL);
	}
	var = ft_strdup(var_content[1]);
	ft_free_tab(&var_content);
	return (var);
}

int	ft_atoi(char *str)
{
	int	result;
	int	sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || *str == '\t' || *str == '\n' \
		|| *str == '\v' || *str == '\f')
		str++;
	if (*str == '-')
		sign = -1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str <= '9' && *str >= '0')
	{
		result = result * 10 + *str - '0';
		str++;
	}
	result = sign * result;
	return (result);
}

int	ft_get_exit_status_atoi(t_listenvp *lst_envp)
{
	int			exit_status_int;
	t_listenvp	*tmp;
	char		*val;

	val = NULL;
	exit_status_int = 0;
	tmp = lst_envp;
	while (tmp)
	{
		if (ft_strncmp((tmp)->key_value, "?=", 2) == 0)
		{
			val = ft_get_var_dollar_quest(tmp->key_value);
			exit_status_int = ft_atoi(val);
			ft_free_struct_str(&val);
			return (exit_status_int);
		}
		tmp = tmp->next;
	}
	return (exit_status_int);
}

void	ft_set_exit_code_in_lst_envp(void *lst_envp, int flag, int num)
{
	static t_listenvp	*ptr;

	(void)num;
	if (flag == 1)
		ptr = lst_envp;
	if (g_signal == HD_STOP)
		ptr = ft_get_exit_status(&ptr, "?=", 130);
	else if (num != -13)
		ptr = ft_get_exit_status(&ptr, "?=", num);
}

t_listenvp	*ft_get_lst_envp(char **envp)
{
	static t_listenvp	*lst_envp;
	t_listenvp			*new;
	int					i;

	lst_envp = NULL;
	i = 0;
	new = NULL;
	if (envp)
	{
		while (envp[i])
		{
			new = ft_lstnew_for_lst_envp(envp[i]);
			ft_lstadd_back_envp(&lst_envp, new);
			i++;
		}
		new = ft_lstnew_for_lst_envp("?=0");
		ft_lstadd_back_envp(&lst_envp, new);
	}
	return (lst_envp);
}
