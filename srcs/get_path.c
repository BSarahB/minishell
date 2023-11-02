/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 14:11:35 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/03/13 14:11:36 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

char	**ft_get_path(char **envp)
{
	char	**path_addr;
	int		i;

	i = 0;
	path_addr = NULL;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_addr = ft_split(&envp[i][5], ':');
			break ;
		}
		i++;
	}
	i = 0;
	return (path_addr);
}

void	ft_reset_flag(t_expand *exp)
{
	if (exp->flag_dollar_to_remove == 1)
		exp->flag_dollar_to_remove = 0;
}

void	ft_find_var(char *expand, t_param_var *p, char **envp, size_t n)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], (&expand[1]), n) == 0)
		{
			p->var_content = ft_split(&envp[i][n], '=');
			if (p->var_content == NULL || p->var_content[0] == NULL)
				ft_free_tab(&(p->var_content));
			break ;
		}
		i++;
	}
}

void	st_t_param_var_init(t_param_var *para)
{
	para->var_content = NULL;
	para->var = NULL;
	para->equal[0] = '=';
	para->equal[1] = 0;
}

char	*ft_get_var(char **envp, char *expand, t_expand *exp)
{
	t_param_var	para;
	size_t		n;

	st_t_param_var_init(&para);
	n = ft_strlen(expand);
	expand = ft_update_string(&expand, ft_strjoin(expand, para.equal));
	if (((ft_strcmp(expand, "$=") == 0) && exp->quoting_rule_adequate == 1 \
		&& exp->flag_dollar_to_remove == 0) || ((ft_strcmp(expand, "$=") == 0) \
			&& exp->quoting_rule == 2 && exp->flag_dollar_to_remove == 0))
	{
		para.var = ft_strdup("$");
		ft_update_string(&expand, para.var);
		return (para.var);
	}
	ft_reset_flag(exp);
	ft_find_var(expand, &para, envp, n);
	if (para.var_content == NULL || para.var_content[0] == NULL)
	{
		ft_free_struct_str(&expand);
		return (NULL);
	}
	para.var = ft_strdup(para.var_content[0]);
	ft_update_string(&expand, para.var);
	ft_free_tab(&(para.var_content));
	return (para.var);
}
