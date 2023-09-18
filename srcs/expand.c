/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 11:53:45 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/09/04 11:53:59 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int ft_get_token_quoting_rule2(char *str, size_t i, int *quoting_rule, int *quoting_rule_adequate)
{	
	char c;

	c = str[i];
/*	if((*quoting_rule == 1 || *quoting_rule == 2) && *quoting_rule_adequate == 1)
	{
		*quoting_rule = 0;
		*quoting_rule_adequate = 0;
	}
	*/
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
	else if (*quoting_rule == 0 && str[i + 1] == '\0') // c est le cas de $> l[s]    ->[s] est checke dans la ft_get_token_quoting rule on verifie si la quoting rule  == 0 et que lindex suivant est un \0 alors cela signifie qu on a la fin d un token
	{
		if (c == '\"')
			*quoting_rule = 2;
		if (c == '\'')
			*quoting_rule = 1;
	}
return(*quoting_rule_adequate);
}

/*
void	ft_dequote(t_list *lst_token)
{
	(void)lst_token;
}
int	ft_expand_exists(t_list *lst_token)
{
	(void)lst_token;
	return (0);
}

void ft_get_scope_expand(t_list *lst_token)
{

(void)lst_token;

}


int ft_tag_ambiguous_redir(t_list *lst_token)
{
	//compter le nb d expand ici
	printf("expand here\n");
	if(ft_expand_exists(lst_token) == 1)
		lst_token->expand_exists = 1;
	if(lst_token->expand_exists == 0)
	{

		lst_token->tag_ambigeous = 1;
		lst_token->prev->title = redir_in;
		return (0);
	}
	return(1);
}

int ft_is_expand_to_substitute_redir(t_list *lst_token)
{
	char *str;
	int i;
	int	quoting_rule;
	int	quoting_rule_adequate;

	quoting_rule = 0;
	quoting_rule_adequate = 0;
	str = lst_token->content;
	i = 0;
	if(str == NULL)
		return (0);
	if(ft_strcmp(lst_token->content,"$") == 0)
		return(0);//garder $ comme un token pour creer le file
	while(str[i]) //ft_expand_authorized
	{
		quoting_rule_adequate = ft_get_token_quoting_rule2(str, i, &quoting_rule, &quoting_rule_adequate);
			
		if(str[i] == '$' && quoting_rule != 1) //&& que $ n est pas suivi de '\0' ->suivi de \0 signifie que ce n est pas un expand , mais simplement un caractere $
			{
				if(str[i + 1] == '\0') //|| str[i + 1] ==  '\"')//cas du ls >VAR$
					return(0);
				
			//	ft_get_scope_expand(lst_token);
			//	if(ft_expand_exists(lst_token) == 1)
			//	{
					
			//	}
			//	else
			//		return(0);
			}
		i++;
		if(quoting_rule_adequate == 1)
			quoting_rule_adequate = 0;

	}
	return(0);
}


void ft_tag_ambigeous_for_redir(t_list *lst_token)
{
	t_list *tmp;
	int nb_of_expand;

	nb_of_expand = 0;
	tmp = lst_token;
	if(tmp == NULL)
		return;
	while(lst_token)
	{
		if(lst_token->title == redir_out || lst_token->title == redir_append || lst_token->title == redir_in)
		{
			if(ft_is_expand_to_substitute_redir(lst_token->next) == 1)
			{
				nb_of_expand++;
				//printf(" nb of expand : %d ", nb_of_expand)

				ft_tag_ambiguous_redir(lst_token);
			}
			
			
		}
		else //0 " pas de substitute on garde le token tel quel"
		{
			ft_dequote(lst_token);
		}
		lst_token = lst_token->next;
	}
lst_token = tmp;
return;
}

void	ft_modify_lst_token(t_list *lst_token)
{
	ft_tag_ambigeous_for_redir(lst_token);
}
*/