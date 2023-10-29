/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_and_retokenize.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 13:52:34 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/09/04 13:52:55 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int		ft_strcmp_char(char c1, char c2)
{
	return (c1 - c2);
}

int	ft_is_expand_unvalidated(char **invalidators, char c)
{
	int i;

	i = 0;
	while (invalidators[i] != NULL)
	{
		if (ft_strcmp_char(invalidators[i][0], c) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	ft_is_expand_here(char *str)
{
	int quoting_rule_adequate;
	int	quoting_rule;
	int i;
	char *invalidators[] = {"+", ",", "}", "]", "~", "=", NULL};
	int flag_expand_here;

	i = 0;
	flag_expand_here = 0;
	quoting_rule_adequate = 0;
	quoting_rule = 0;
	while(str[i])
	{
		quoting_rule_adequate = ft_get_token_quoting_rule2(str, i, &quoting_rule, &quoting_rule_adequate);
		if(str[i] == '$' && quoting_rule != 1 ) //&& que $ n est pas suivi de '\0' ->suivi de \0 signifie que ce n est pas un expand , mais simplement un caractere $
			{
				flag_expand_here = 1;
				if(str[i + 1] == '\0' || ft_is_expand_unvalidated(invalidators, str[i + 1]) == 1) //|| str[i + 1] ==  '\"')//cas du ls >VAR$
					flag_expand_here = 0;
		}
		i++;
		if(quoting_rule_adequate == 1)
			quoting_rule_adequate = 0;
	}
	if(flag_expand_here == 0)
		return(0);
	else
		return(1);
}

int ft_is_expand_to_substitute(t_list *lst_token)
{
	char *str;
	int i;
	int i_save;
	char *buffer;

	buffer = ft_init_string(1096);
	str = lst_token->content;
	i = 0;
	i_save = i;
	if(str == NULL)
		return (0);
	if(ft_strcmp(lst_token->content,"$") == 0)
		return(0);
	ft_is_expand_here(str);
	free(buffer);
	return(0);
}

void	ft_expand_and_retokenize(t_list *lst_token)
{
	t_list *tmp;

	tmp = lst_token;
	if(tmp == NULL)
		return;
	while(lst_token)
	{
		ft_is_expand_to_substitute(lst_token);		
		lst_token = lst_token->next;
	}
	lst_token = tmp;
	return;
}