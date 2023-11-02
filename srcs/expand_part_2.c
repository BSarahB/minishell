/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_part_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 17:06:05 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/09/26 17:06:18 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_data	*ft_retokenize_and_dequote_token_2(t_cmd *cmd, \
t_list *start_lst_token, t_simpleCmd *simpleCmd, t_data *data2)
{
	size_t	k;
	t_list	*tmp;
	int		flag_retokenize;
	char	*check_dequote;

	(void)k;
	(void)cmd;
	flag_retokenize = -1;
	tmp = start_lst_token;
	check_dequote = NULL;
	ft_retokenize_first(check_dequote, simpleCmd, &flag_retokenize, tmp);
	ft_retokenize_middle(simpleCmd, &data2, &tmp, &flag_retokenize);
	ft_retokenize_last(tmp, simpleCmd, flag_retokenize, &data2);
	return (data2);
}

t_list	*ft_retokenize_and_dequote_token_1_empty( \
t_simpleCmd *simpleCmd, t_data	**data2, int i)
{
	simpleCmd->flag_empty_simplecmd = 1;
	simpleCmd->nb_of_tokens_in_simplecmd = i;
	ft_free_struct_t_data(data2);
	return (NULL);
}

void	ft_add_retokenized_token(t_list *new, \
t_list **start_lst_token_retokenized, size_t *i, t_cmd *cmd)
{
	new = ft_lstnew_for_lst_retokenized(*start_lst_token_retokenized);
	ft_lstadd_back(&(cmd->lst_token_retokenized), new);
	*start_lst_token_retokenized = (*start_lst_token_retokenized)->next;
	(*i)++;
}

void	ft_add_last_retokenized_token(t_simpleCmd *simpleCmd, t_list *new, \
t_cmd *cmd, t_list *start_lst_token)
{
	new = ft_lstnew_for_lst_retokenized2(ft_lstfind(start_lst_token, \
											simpleCmd->end_simplecmd_pos));
	ft_lstadd_back(&(cmd->lst_token_retokenized), new);
}

t_list	*retok_dequot_1(t_cmd *cmd, \
t_list *st_lst_tk, t_simpleCmd *scmd)
{
	t_data	*data2;
	t_list	*start_lst_token_retokenized;
	t_list	*head;
	t_list	*new;
	size_t	i;

	data2 = NULL;
	start_lst_token_retokenized = NULL;
	head = NULL;
	new = NULL;
	i = 0;
	data2 = ft_struct_init_data(&data2);
	data2 = ft_retokenize_and_dequote_token_2(cmd, st_lst_tk, \
scmd, data2);
	start_lst_token_retokenized = data2->lst_token;
	head = start_lst_token_retokenized;
	if (start_lst_token_retokenized == NULL)
		return (ft_retokenize_and_dequote_token_1_empty(scmd, &data2, i));
	while (start_lst_token_retokenized)
		ft_add_retokenized_token(new, &start_lst_token_retokenized, &i, cmd);
	if (!(ft_lstfind(st_lst_tk, scmd->end_simplecmd_pos)->next == NULL))
		ft_add_last_retokenized_token(scmd, new, cmd, st_lst_tk);
	scmd->nb_of_tokens_in_simplecmd = i;
	ft_free_struct_t_data(&data2);
	return (head);
}
