/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_validity_of_line_input.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 18:43:25 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/10/29 18:43:30 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int ft_check_double_points_token(t_list *tmp)
{
	if(tmp->next)
	{
		if (ft_is_check_for_token(tmp->next, "|") == 1)
			return(0);//TO DO on va exec la commande en attente
		else
			return (-12);	//pour retourner ensuite 0 apres transformation
	}
	if(tmp->prev)
	{
		if(ft_strcmp(tmp->prev->content, "!@") == 0)
		{
			ft_error("minishell: !@: event not found\n");
			return(-12);
		}
		if(ft_strcmp(tmp->prev->content, "|") != 0 && ft_strcmp(tmp->prev->content, "!") != 0) //&& (ft_strcmp(tmp->prev->content, "!") != 0))
			return(0);
	}
	return(-12);//pour return 0 je mets -12 
}

int ft_pipe(t_list *tmp, int *flag_pipe)
{
	if(ft_strcmp(tmp->content, "|") == 0)
	{
		if(*flag_pipe == 1)
		{
			ft_error_msg2b(tmp->content);
			return(2);
		}
		if(tmp->next == NULL)
		{
			ft_error_msg2b(tmp->content);
			return(2);
		}
		if((ft_strcmp(tmp->next->content, "|") == 0))
		{
			ft_error_msg2c(tmp->content);
			return(2);
		}
	}
	*flag_pipe = 0;
	return(0);
}

int ft_chevron(t_list *tmp)
{
	if((ft_strcmp(tmp->content, "<") == 0) || (ft_strcmp(tmp->content, ">")) == 0 || (ft_strcmp(tmp->content, "<<") == 0) || (ft_strcmp(tmp->content, ">>") == 0))
	{
		if(tmp->next == NULL)
		{
			ft_error_msg2("`newline'");
			return(2);
		}
		if (ft_strcmp(tmp->next->content, "|") == 0)
		{
			ft_error_msg2b(tmp->next->content);
			return(2);
		}
		if((ft_strcmp(tmp->next->content, "<") == 0) || (ft_strcmp(tmp->next->content, ">")) == 0 || (ft_strcmp(tmp->next->content, "<<") == 0) || (ft_strcmp(tmp->next->content, ">>") == 0))
		{
			ft_error_msg2b(tmp->next->content);
			return(2);
		}
	}
	return(0);
}

int		ft_check_bash_syntax_error_caracteres_volee(t_list *lst_token)
{
	t_list *tmp;
	int		flag_pipe;

	flag_pipe = 1;
	if(lst_token == NULL)
		return(-13);
	if(*lst_token->content == '#')
		return(-12);
	tmp = lst_token;
	while(tmp)
	{
		if(ft_exclamation(tmp))
			return(1);
		if(ft_chevron(tmp))
			return(2);
		if(ft_pipe(tmp, &flag_pipe))
			return(2);
		if(ft_ampersand(tmp))
			return(2);	
		if((ft_strcmp(tmp->content, ":") == 0))
			return(ft_check_double_points_token(tmp));
		if((ft_strcmp(tmp->content, "#") == 0))
			return(2);//TODO ft_modify_lst_token()
		tmp = tmp->next;
	}
	return(0);
}