/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 17:48:27 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/29 17:48:39 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_listenvp *ft_lstfind_content(t_listenvp *alst, char *keyequal)
{
	t_listenvp *tmp;
	tmp = alst;
	while(tmp != NULL)
	{
		if(ft_strncmp(keyequal, tmp->key_value, ft_strlen(keyequal)) == 0)
			return(tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

int	ft_is_space(int c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\r'
		|| c == '\v' || c == '\f')
		return (c);
	return (0);
}

int ft_check_is_space(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if(ft_is_space(str[i]) != 0)
			return (1);
		i++;
	}
	return(0);
}

char *ft_strchr(const char *s, int c)
{
	char *str;
	str = (char *)s;

	while(*str && *str != (unsigned char)c)
		str++;
	if(*str == (unsigned char)c || c == '\0')
		return(str);
	return (NULL);
}

char	*get_var_in_lst_envp_for_cd(t_data_env *data_env, char *str)
{
	char	*key;
	t_listenvp *tmp;

	key= ft_strjoin(str, "=");
	tmp = data_env->lst_envp;
	if (!key)
		return (NULL);
	while (tmp)
	{
		if (ft_strncmp(key, tmp->key_value, ft_strlen(key)) == 0) //a la difference des autres fonctions , ici je choisis moi meme la vairable qui est HOME dans les autres cas de figure pour les codes prcedents la variable etait imposee par l entree user et stockee sous forme de KEY=VALUE sur un seul token
		{
			free(key);
			key = NULL;
			return (ft_strchr(tmp->key_value, '=') + 1);
		}
		tmp = tmp->next;
	}
	free(key);
	key = NULL;
	return (NULL);
}