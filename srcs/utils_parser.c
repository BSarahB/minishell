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

int	*ft_init_ctab(int **int_tab, size_t len, int init_value)
{
	*int_tab = malloc(sizeof (int) * (len + 1));
	if (!(*int_tab))
		return (NULL);
	if (len > 0)
		ft_memset2(*int_tab, init_value, len);
	(*int_tab)[len] = '\0';
	return (*int_tab);
}

void	ft_get_end_simplecmd_pos(t_cmd *cmd, t_simpleCmd *simpleCmd, \
													t_list **start_lst_token)
{
	t_list	*tmp;

	(void)cmd;
	(void)simpleCmd;
	tmp = *start_lst_token;
	while (tmp)
	{
		if (tmp->title == operator)
		{
			simpleCmd->end_simplecmd_pos = tmp->position;
			break ;
		}
		if (tmp->next == NULL)
		{
			simpleCmd->end_simplecmd_pos = tmp->position;
			break ;
		}
		tmp = tmp->next;
	}
}

void	ft_aff_abs_cmd_and_args(t_cmd *cmd)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < cmd->nb_of_simplecmds)
	{
		j = 0;
		while (cmd->simplecmds[i]->nb_of_tokens_in_simplecmd == 0 \
			&& cmd->simplecmds[i + 1] != NULL)
			i++;
		if (cmd->simplecmds[i] != NULL \
			&& cmd->simplecmds[i]->nb_of_tokens_in_simplecmd != 0)
		{
			while (cmd->simplecmds[i]->cmd_and_args[j] != NULL)
				j++;
			j = 0;
			while (cmd->simplecmds[i]->abs_cmd_and_args[j] != NULL)
				j++;
			i++;
		}
		else
			break ;
	}
}

void	ft_check_tab_envp_removed(char **tab)
{
	if (*tab == NULL)
	{
		ft_putstr_fd("Please set back the environnement to run minishell \
															properly\n", 2);
		exit(1);
	}
}

void	ft_aff_tab_envp(char **tab)
{
	size_t	k;

	k = 0;
	printf("tab envp \n");
	while (tab[k])
	{
		printf("tab[%zu]= <%s>\n", k, tab[k]);
		k++;
	}
}
