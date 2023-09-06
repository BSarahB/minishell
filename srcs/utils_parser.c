/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 18:03:08 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/08/14 18:03:31 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void *ft_memset2(void *b, int n, size_t len)
{
	size_t i;

	i = 0;
	while (i < len)
		((int *)b)[i++] = n;
	return (b);
}

int *ft_init_ctab(int **int_tab, size_t len, int init_value)
{
	*int_tab = malloc(sizeof(int) * (len + 1));
	if (!(*int_tab))
		return (NULL);
	if (len > 0)
		ft_memset2(*int_tab, init_value, len);
	(*int_tab)[len] = '\0';
	return (*int_tab);
}

void	ft_get_end_simpleCmd_pos(t_cmd *cmd, t_simpleCmd *simpleCmd, t_list **start_lst_token)
{
	(void)cmd;
	(void)simpleCmd;
	t_list *tmp;

	tmp = *start_lst_token;
	while(tmp)
	{
		if(tmp->title == operator)
		{
			simpleCmd->end_simpleCmd_pos = tmp->position;
			break;
		}
		if(tmp->next == NULL)
		{
			simpleCmd->end_simpleCmd_pos = tmp->position;
			break;
		}
		tmp = tmp->next;
	}
}
//fcts utiles au deboggage
void	ft_aff_abs_cmd_and_args(t_cmd	*cmd)
{
	size_t i;
	size_t j;

	i = 0;
	while (i < cmd->nb_of_simpleCmds)
	{
		j = 0;
		while(cmd->simpleCmds[i]->nb_of_tokens_in_simpleCmd == 0 &&  cmd->simpleCmds[i+1] != NULL)
			i++;//protections pour >a | ls |grep c
		if(cmd->simpleCmds[i] != NULL && cmd->simpleCmds[i]->nb_of_tokens_in_simpleCmd != 0) 
		{
			while (cmd->simpleCmds[i]->cmd_and_args[j] != NULL)
		{
			//printf("cmd_and_args de la simpleCmd[%zu] : <%s>\n", i,cmd->simpleCmds[i]->cmd_and_args[j]);
			j++;
		}
		//printf("\n\n\n");		
		j= 0;
		while (cmd->simpleCmds[i]->abs_cmd_and_args[j] != NULL)
		{
			//printf("abs_cmd_and_args[%zu]<%s>\n",i, cmd->simpleCmds[i]->abs_cmd_and_args[j]);
			j++;
		}
//	printf("\n\n\n");
		i++;
		}
		else
			break;
	}
}

