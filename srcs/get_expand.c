/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 11:28:59 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/31 11:29:01 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_go_to_substitute(t_param_sub *para, t_expand *exp, \
												char **expand, char **envp)
{
	para->lst_token->retokenize_allowed = 1;
	*expand = ft_substitute(*expand, envp, exp);
	if (*expand != NULL)
	{
		ft_memcpy(&(para->buffer[exp->j]), *expand, ft_strlen(*expand));
		exp->j = exp->j + ft_strlen(*expand);
		ft_check_expand_for_tag_ambigeous(*expand, exp, para->lst_token);
		if (exp->quoting_rule_adequate == 1 \
					&& (exp->quoting_rule == 2 || exp->quoting_rule == 1))
		{
			exp->quoting_rule_adequate = 0;
			exp->quoting_rule = 0;
		}
	}
	free(*expand);
	*expand = NULL;
	if (exp->flag_dollar_quest == 1)
	{
		exp->flag_expand_here = 0;
		exp->flag_dollar_quest = 0;
		para->i++;
	}
}

void	ft_printf_in_buffer(t_param_sub *para, t_expand *exp)
{
	if (exp->flag_expand_here != 1)
	{
		para->buffer[exp->j] = para->str[para->i];
		exp->j++;
	}
	if ((para->str[para->i] == '\'' || para->str[para->i] == '\"') && \
			exp->flag_dollar_quest == 1)
	{
		exp->quoting_rule_adequate = \
			ft_get_token_quoting_rule2(para->str, para->i, \
				&(exp->quoting_rule), &(exp->quoting_rule_adequate));
		exp->flag_dollar_quest = 0;
	}
	else if ((para->str[para->i] == '\'' || para->str[para->i] == '\"') && \
			(exp->quoting_rule == 2 || exp->quoting_rule == 1) && \
					exp->quoting_rule_adequate == 1)
		exp->quoting_rule_adequate = ft_get_token_quoting_rule2b(para->str, \
				para->i, &(exp->quoting_rule), &(exp->quoting_rule_adequate));
}

void	ft_search_for_expand(t_param_sub *para, t_expand *exp, \
											char *expand, char **envp)
{
	while (para->str[para->i])
	{
		exp->quoting_rule_adequate = ft_get_token_quoting_rule2(para->str, \
				para->i, &(exp->quoting_rule), &(exp->quoting_rule_adequate));
		if (exp->flag_expand_here == 1)
			para->i = ft_get_end_expand(para->str, exp, &expand, para->i);
		if (expand != NULL)
			ft_go_to_substitute(para, exp, &expand, envp);
		if (para->str[para->i] == '$' && exp->quoting_rule == 1 && \
				exp->flag_expand_here != 1)
			ft_check_dollar_to_remove_before_sq(para->str, exp, para->i);
		if (para->str[para->i] == '$' && exp->quoting_rule != 1 && \
				exp->flag_expand_here != 1)
			ft_get_start_expand(para->str, exp, para->i, para->buffer);
		else
			ft_printf_in_buffer(para, exp);
		para->i++;
	}
}

void	ft_init_t_param_sub(t_param_sub *para, t_list *lst_token, \
													char *str, char *buffer)
{
	para->lst_token = lst_token;
	para->str = str;
	para->buffer = buffer;
	para->i = 0;
}

int	ft_is_expand_here(t_list *lst_token, char *str, char *buffer, char *envp[])
{
	t_expand	*exp;
	char		*expand;
	t_param_sub	para;

	expand = NULL;
	exp = NULL;
	exp = ft_struct_init_expand(&exp);
	ft_init_t_param_sub(&para, lst_token, str, buffer);
	ft_search_for_expand(&para, exp, expand, envp);
	if (exp->flag_expand_in_token == 0)
	{
		ft_free_struct_t_expand(&exp);
		return (0);
	}
	else
	{
		ft_free_struct_t_expand(&exp);
		return (1);
	}
}
