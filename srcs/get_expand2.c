/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_expand2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 23:12:59 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/29 23:13:18 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

char	**ft_allocate_invalidators(void)
{
	char	**tab;

	tab = malloc(sizeof (*tab) * 7);
	tab[0] = ft_strdup("+");
	tab[1] = ft_strdup(",");
	tab[2] = ft_strdup("}");
	tab[3] = ft_strdup("]");
	tab[4] = ft_strdup("~");
	tab[5] = ft_strdup("=");
	tab[6] = NULL;
	return (tab);
}

void	ft_get_start_expand(char *str, t_expand *exp, size_t i, char *buffer)
{
	char	**invalidators;

	invalidators = ft_allocate_invalidators();
	exp->flag_expand_here = 1;
	if (str[i + 1] == '\0' \
		|| ft_is_expand_unvalidated(invalidators, str[i + 1]) == 1)
	{
		exp->flag_expand_here = 0;
		buffer[exp->j] = str[i];
		exp->j = exp->j + 1;
	}
	if (((str[i + 1] == '\"' || str[i + 1] == '\'') \
		&& exp->quoting_rule == 0) \
			|| (exp->quoting_rule == 2 && exp->quoting_rule_adequate == 1))
	{
		exp->flag_dollar_to_remove = 1;
	}
	if (exp->flag_expand_here == 1)
	{
		exp->start_expand_pos = i;
		exp->flag_expand_in_token = 1;
	}
	ft_free_tab(&invalidators);
}

char	*ft_substitute(char *expand, char *envp[], t_expand *exp)
{
	expand = ft_get_var(envp, expand, exp);
	return (expand);
}

void	ft_check_expand_for_tag_ambigeous(char *expand, \
									t_expand *exp, t_list *lst_token)
{
	int	i;

	i = 0;
	if ((exp->quoting_rule == 0) && (lst_token->prev != NULL))
	{
		if (lst_token->prev->title == redir_in \
			|| lst_token->prev->title == redir_out \
			|| lst_token->prev->title == redir_append)
		{
			if (expand[i])
			{
				while (expand[i])
				{
					if (expand[i] == ' ')
					{
						lst_token->tag_ambigeous = 1;
						return ;
					}
					i++;
				}
			}
		}
	}
}

void	ft_check_dollar_to_remove_before_sq(char *str, t_expand *exp, size_t i)
{
	if (((str[i + 1] == '\"' || str[i + 1] == '\'') \
		&& exp->quoting_rule == 0) \
		|| (exp->quoting_rule == 1 && exp->quoting_rule_adequate == 1))
	{
		exp->flag_dollar_to_remove = 1;
	}
	exp->start_expand_pos = i;
	exp->flag_expand_in_token = 1;
}
