/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_part4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 18:40:05 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/31 18:40:23 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_get_token_quoting_rule4(char *str, size_t i, int *quoting_rule, \
int *quoting_rule_adequate)
{
	char	c;

	c = str[i];
	if (*quoting_rule == 0 && c == '\"' && str[i + 1] != '\0')
		*quoting_rule = 2;
	else if (c == '\"' && *quoting_rule == 2)
	{
		*quoting_rule = 2;
		*quoting_rule_adequate = 1;
	}
	else if (*quoting_rule == 0 && c == '\'' && str[i + 1] != '\0')
		*quoting_rule = 1;
	else if (c == '\'' && *quoting_rule == 1)
	{
		*quoting_rule = 0;
		*quoting_rule_adequate = 1;
	}
	else if (*quoting_rule == 0 && str[i + 1] == '\0')
	{
		if (c == '\"')
			*quoting_rule = 2;
		if (c == '\'')
			*quoting_rule = 1;
	}
	return (*quoting_rule_adequate);
}

void	ft_retokenize_first(char *check_dequote, t_simpleCmd *simpleCmd, \
int *flag_retokenize, t_list *tmp)
{
	if (tmp != NULL)
	{
		if (*flag_retokenize == -1)
		{
			check_dequote = ft_dequote(ft_strdup(tmp->content));
			if (*flag_retokenize == -1 && ft_strcmp(check_dequote, "echo") == 0)
			{
				*flag_retokenize = -2;
				simpleCmd->builtin = echo;
				simpleCmd->is_builtin = 1;
			}
			else if (*flag_retokenize == -1 && tmp->retokenize_allowed == 1)
				*flag_retokenize = 1;
			else
				*flag_retokenize = 0;
			ft_free_struct_str(&check_dequote);
		}
	}
}

void	ft_retokenize_middle(t_simpleCmd *simpleCmd, t_data **data2, \
t_list **tmp, int *flag_retokenize)
{
	while (*tmp != NULL && (*tmp)->position < simpleCmd->end_simplecmd_pos)
	{
		if (*flag_retokenize == -2)
			*data2 = ft_copy_token(*tmp, *data2);
		else if (*flag_retokenize == 0)
			*data2 = ft_copy_token(*tmp, *data2);
		else if (*flag_retokenize == 1)
			*data2 = ft_retokenize(*tmp, *data2);
		*tmp = (*tmp)->next;
		if (tmp != NULL)
		{
			if (*flag_retokenize == 0 || *flag_retokenize == 1)
			{
				if ((*tmp)->retokenize_allowed == 1)
					*flag_retokenize = 1;
				else
					*flag_retokenize = 0;
			}
		}
	}
}

void	ft_retokenize_last(t_list *tmp, t_simpleCmd *simpleCmd, \
int flag_retokenize, t_data **data2)
{
	if (tmp != NULL && tmp->position == simpleCmd->end_simplecmd_pos \
	&& tmp->title != operator)
	{
		if (flag_retokenize == -2)
			*data2 = ft_copy_token(tmp, *data2);
		else if (flag_retokenize == 0)
			*data2 = ft_copy_token(tmp, *data2);
		else if (flag_retokenize == 1)
			*data2 = ft_retokenize(tmp, *data2);
	}
}
