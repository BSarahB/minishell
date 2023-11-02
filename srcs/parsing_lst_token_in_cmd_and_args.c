/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_lst_token_in_cmd_and_args.c                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 18:51:33 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/08/14 18:51:49 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_allocate_simplecmd_nbtokens(char ***c_and_a, char ***abs_c_and_a, \
														t_simpleCmd *simpleCmd)
{
	*c_and_a = malloc(sizeof (**c_and_a) * \
					(simpleCmd->nb_of_tokens_in_simplecmd + 1));
	*abs_c_and_a = malloc(sizeof (**abs_c_and_a) * \
					(simpleCmd->nb_of_tokens_in_simplecmd + 1));
	return (*c_and_a && *abs_c_and_a);
}

void	ft_struct_init_t_param_parse(t_param_parse *para)
{
	para->k = 0;
	para->c_and_a = NULL;
	para->abs_c_and_a = NULL;
}

int	parse_cmdarg_scmd_frm_retoknzd_lst( \
					t_list *lst_token_retokenized, t_simpleCmd *simpleCmd)
{
	t_param_parse	para;
	t_list			*start_lst_token_retokenized;

	ft_struct_init_t_param_parse(&para);
	start_lst_token_retokenized = lst_token_retokenized;
	if (simpleCmd->nb_of_tokens_in_simplecmd == 0)
		return (0);
	if (!ft_allocate_simplecmd_nbtokens(&(para.c_and_a), \
										&(para.abs_c_and_a), simpleCmd))
		return (0);
	while (para.k < simpleCmd->nb_of_tokens_in_simplecmd)
	{
		para.c_and_a[para.k] = ft_strdup(lst_token_retokenized->content);
		para.abs_c_and_a[para.k] = ft_strdup(lst_token_retokenized->content);
		lst_token_retokenized = lst_token_retokenized->next;
		para.k++;
	}
	para.c_and_a[para.k] = NULL;
	simpleCmd->cmd_and_args = para.c_and_a;
	para.abs_c_and_a[para.k] = NULL;
	para.abs_c_and_a = ft_get_abs_argumentsb(para.abs_c_and_a);
	simpleCmd->abs_cmd_and_args = para.abs_c_and_a;
	if (start_lst_token_retokenized)
		ft_free_struct_t_list_lst_token(&start_lst_token_retokenized);
	return (1);
}

void	st_t_param_parse_init(t_param_parse *para)
{
	para->k = 0;
	para->c_and_a = NULL;
	para->abs_c_and_a = NULL;
}

int	ft_malloc_and_parse_cmd_and_args_tab_of_simplecmd(t_list *lst_token, \
												t_simpleCmd *simpleCmd)
{
	t_param_parse	para ;

	st_t_param_parse_init(&para);
	if (simpleCmd->nb_of_tokens_in_simplecmd == 0)
		return (0);
	if (!ft_allocate_simplecmd_nbtokens(&(para.c_and_a), &(para.abs_c_and_a), \
																	simpleCmd))
		return (0);
	while (para.k < simpleCmd->nb_of_tokens_in_simplecmd)
	{
		para.c_and_a[para.k] = ft_strdup(lst_token->content);
		para.abs_c_and_a[para.k] = ft_strdup(lst_token->content);
		lst_token = lst_token->next;
		para.k++;
	}
	para.c_and_a[para.k] = NULL;
	simpleCmd->cmd_and_args = para.c_and_a;
	para.abs_c_and_a[para.k] = NULL;
	para.abs_c_and_a = ft_get_abs_argumentsb(para.abs_c_and_a);
	simpleCmd->abs_cmd_and_args = para.abs_c_and_a;
	return (1);
}
