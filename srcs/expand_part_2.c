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
//ici on va s occuper de: si cmd principale  == echo -> DEQUOTE(PAS DE RETOKENIZE) 
// si cmd principale  == none ->RETOKENIZE + dequote

int ft_get_token_content2(t_data *data, int start_token_pos, int end_token_pos, char *line)
{
	
	t_list *new;

	ft_get_token_content_lengh_for_malloc(data->token, start_token_pos, end_token_pos);
	data->token->content = ft_memcpy(data->token->content, &line[start_token_pos], (size_t)(end_token_pos - start_token_pos + 1));
	new = ft_lstnew_for_lst2(data);
	ft_lstadd_back(&(data->lst_token), new);
	data->token->quoting_rule = 0;
	data->token->quoting_rule_adequate = 0;
	data->token->title = -1;
	start_token_pos = -1;
	return(start_token_pos);
}	

int ft_get_token_quoting_rule4(char *str, size_t i, int *quoting_rule, int *quoting_rule_adequate)
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
		{
			*quoting_rule = 2;
		}
	else if (c == '\"' && *quoting_rule == 2)
	{
		*quoting_rule = 2;
		*quoting_rule_adequate = 1;
	}
	else if (*quoting_rule == 0 && c == '\'' && str[i + 1]!= '\0')
		*quoting_rule = 1;
	else if (c == '\'' && *quoting_rule == 1)
	{
		*quoting_rule = 0;
		*quoting_rule_adequate = 1;
	
	}
	else if (*quoting_rule == 0 &&str[i + 1] == '\0') // c est le cas de $> l[s]    ->[s] est checke dans la ft_get_token_quoting rule on verifie si la quoting rule  == 0 et que lindex suivant est un \0 alors cela signifie qu on a la fin d un token
	{
		if (c == '\"')
			*quoting_rule = 2;
		if (c == '\'')
			*quoting_rule = 1;
	}
return(*quoting_rule_adequate);
}

size_t	ft_tokenize2(char *str, char *line, size_t i, t_data *data)
{
	int quoting_rule;
	int quoting_rule_adequate;
	int start_pos;

	quoting_rule = 0;
	quoting_rule_adequate = 0;
	
	
	while(str[i] == ' ')
			i++;
	start_pos = i;
	while (str[i])
	{
		
		//if(str[i] == '\0')
		if(start_pos == -1)
			start_pos = i;

		quoting_rule_adequate = ft_get_token_quoting_rule4(str, i, &quoting_rule, &quoting_rule_adequate);
		if(quoting_rule_adequate == 1 && (quoting_rule == 1 || quoting_rule == 2))
			quoting_rule = 0;
		if (str[i] == ' ' && quoting_rule == 0) //&& que $ n est pas suivi de '\0' ->suivi de \0 signifie que ce n est pas un expand , mais simplement un caractere $
			start_pos = ft_get_token_content2(data, start_pos, (int)i -1, line);
		if(str[i] == ' ')
		{
			while(str[i] == ' ')
			i++;
		}
		else
			i++;
		if(str[i] == '\0')
		{
			ft_get_token_content2(data, start_pos, (int)i -1, line);
		}

	}
	return (i);
}

t_data	*ft_trim_and_clear2(char *line, t_data *data2)
{
	char *str;
	size_t i;

	str = line;
	i = 0;
	i = ft_tokenize2(str, line, i, data2);

	return (data2);
}



t_data *ft_retokenize(t_list *tmp, t_data *data2)
{
	return(ft_trim_and_clear2(tmp->content, data2));
}

t_data *ft_copy_token(t_list *tmp, t_data *data2)
{
		t_list *new;

		new = ft_lstnew_for_lst_token_copy(tmp);
		ft_lstadd_back(&(data2->lst_token), new);
		return(data2);
}


t_data	*ft_retokenize_and_dequote_token_2(t_cmd *cmd, t_list *start_lst_token, t_simpleCmd *simpleCmd, t_data *data2)
{
	size_t	k;
	t_list	*tmp;
    int flag_retokenize;
	(void)simpleCmd;
	(void)k;
	(void)cmd;
	char *check_dequote;
  
    flag_retokenize = -1;
	tmp = start_lst_token;
	if(tmp != NULL)
	{
		if(flag_retokenize == -1)
        {
			check_dequote = ft_dequote(ft_strdup(tmp->content));
            if(flag_retokenize == -1 && ft_strcmp(check_dequote, "echo") == 0)
                {
                    flag_retokenize = 0;
					simpleCmd->builtin = echo;
					simpleCmd->is_builtin = 1;//TODO modifier quand echo builtin sera a faire
                }
            else
                flag_retokenize = 1;
			//printf("check_DQ <%s> check_DQ\n", check_dequote);
			free(check_dequote);
        }
       
	}
	while (tmp !=NULL && tmp->position < simpleCmd->end_simpleCmd_pos)
	{ 
    	if(flag_retokenize == 1)
        {
            data2 = ft_retokenize(tmp, data2);
        }
		if(flag_retokenize == 0)
		{
			data2 = ft_copy_token(tmp, data2);
		}
		tmp = tmp->next;
	}
	if(tmp != NULL && tmp->position == simpleCmd->end_simpleCmd_pos && tmp->title != operator)
	{
		if(flag_retokenize == 1)
        {
            data2= ft_retokenize(tmp, data2);
        }
		if(flag_retokenize == 0)
		{
			data2 = ft_copy_token(tmp, data2);

		}
	}
	return (data2);
}


t_list *ft_lstfind(t_list *alst, int position)
{
	t_list *tmp;
	tmp = alst;
	while(tmp != NULL)
	{
		if(tmp->position == (size_t)position)
			return(tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

t_list *ft_retokenize_and_dequote_token_1(t_cmd *cmd, t_list *start_lst_token, t_simpleCmd *simpleCmd)
{
	t_data	*data2;
	t_list *start_lst_token_retokenized;
	t_list *head;
	t_list *new;
	size_t i;


  	data2 = NULL;
	start_lst_token_retokenized = NULL;
	head = NULL;
	new = NULL;
	i = 0;
	
	data2 = ft_struct_init_data(&data2); //TODO proteger tous les mallocs
	data2 = ft_retokenize_and_dequote_token_2(cmd, start_lst_token, simpleCmd, data2);
	start_lst_token_retokenized = data2->lst_token;
	head  = start_lst_token_retokenized;
	//remplissage de lst_retokenized
	if(start_lst_token_retokenized == NULL)
	{
		simpleCmd->flag_empty_simpleCmd = 1;
		simpleCmd->nb_of_tokens_in_simpleCmd = i;
		
		ft_free_struct_t_data(&data2);
		return (NULL);
	}
	while(start_lst_token_retokenized)
	{
		new = ft_lstnew_for_lst_retokenized(start_lst_token_retokenized);

		ft_lstadd_back(&(cmd->lst_token_retokenized), new);
		start_lst_token_retokenized = start_lst_token_retokenized->next;
		i++;
	}

	if(!(ft_lstfind(start_lst_token, simpleCmd->end_simpleCmd_pos)->next == NULL))
		{
			new = ft_lstnew_for_lst_retokenized2(ft_lstfind(start_lst_token, simpleCmd->end_simpleCmd_pos));
			ft_lstadd_back(&(cmd->lst_token_retokenized), new);
		}

	//ft_aff_list_ptr_sur_char_content2(cmd->lst_token_retokenized);
	simpleCmd->nb_of_tokens_in_simpleCmd = i;

	
	ft_free_struct_t_data(&data2);
	
	return(head);

}

