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

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while (i < n - 1 && s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

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

char	*ft_get_var(char **envp, char *expand, t_expand *exp)
{
	char	**var_content;
	char 	*var;
	int		i;
	size_t 	n;

	i = 0;
	var = NULL;
	var_content = NULL;
	n = ft_strlen(expand);
	char	equal[2];

	equal[0] = '=';
	equal[1] = 0;
	expand = ft_update_string(&expand, ft_strjoin(expand, equal));
	if(((ft_strcmp(expand,"$=") == 0) && exp->quoting_rule_adequate == 1) || ( (ft_strcmp(expand,"$=") == 0) && exp->quoting_rule == 2 )) //|| ( (ft_strcmp(expand,"$ =") == 0) && exp->quoting_rule == 2 &&exp->quoting_rule_adequate == 0 ))//&& exp->quoting_rule_adequate == 0))//il faut que QR == 2
	{
		var = ft_strdup("$");
		ft_update_string(&expand, var);
		return(var);
	}
	while (envp[i])
	{
		if (ft_strncmp(envp[i], (&expand[1]), n ) == 0)
		{
			var_content = ft_split(&envp[i][n], '=');
			if(var_content == NULL || var_content[0] == NULL)
				ft_free_tab(&var_content);
			break;
		}
		i++;
	}
	if(var_content == NULL || var_content[0] == NULL) // "$" -> expand est $= 
		{

			ft_free_struct_str(&expand);
			return(NULL);

		}
	var = ft_strdup(var_content[0]);
	ft_update_string(&expand, var);
	ft_free_tab(&var_content);
	return (var);
}