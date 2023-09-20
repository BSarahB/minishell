/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_and_retokenize.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 13:52:34 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/09/04 13:52:55 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
0/Delimiter le debut et a fin de l expand (cf fiche pour les regles QR 0 et QR 2)
0/A le debut de l expand : SI $ EST En QUOTING RULE == 0 : $ doit etre obligatoirement suivi:
-1) d un caractere alpha
-2) d un caractere num compris entre 0 et 9
-3) d un " apostrophe double rempli ou non
-4) d un ' apostrophe simple rempli ou non
-5) d un signe ? ou !
-6) d un signe parmi : ($ suivi immediatement de)
OUI expand: suivi d un des signes: {$(1 ou plusieurs),!, # -> seront substitues par a valeur ou par RIEN (on aura le return \n a l echo )
NON expand: suivi d un des signes: {+, ,(virgule) }, ], = , ~ }  -> ne donnent pas de valeurs a l expand. $ suivi d un de ces signes n aura pas de valeur expand. il aura son sens litteral de dollar
delimitateurs

[idem pour QR == 2 , (pas d expand en QR == 1)]

0/B la fin de l expand :  SI $ EST DANS LA QUOTING RULE == 0
-2) ds le cas d un caractere numerique : l expansion s arrete a ce moment la
-3) & 4) l expansion s arretera au "" ou ' (on n inclut dans la substitution que le $, et n le fera disparatra car ce sera un $ solo, il ne substituera a RIEN)
-5) l expansion s arrete au $ ! #

fin de l expand dans le cas de $ dans la QUTING RULE == 2
- des qu on rencontre un espace
-2) ds le cas d un caractere numerique : l expansion s arrete a ce moment la ex :  echo "$1232" L expansion s arretera des le 2
-5) l expansionn s arrete au ? ou !
-6) des que je rencontre un " ou '
-des qu on rencontre un autre $ ou les signes +#-?@= etc...
OUI expand: suivi d un des signes: {$(1 ou plusieurs),!, # -, }  -> seront substitues par a valeur ou par RIEN (on aura le return \n a l echo )
les signes suivant annulent la valeur de l expand
NON expand: suivi d un des signes: {+, ,(virgule) }, ], = , ~ }  -> ne donnent pas de valeurs a l expand. $ suivi d un de ces signes n aura pas de valeur expand. il aura son sens litteral de dollar
delimitateurs


***EXPAND ***

1/substitute
2/if found : copy in buffer if (not found || $ trouve seul): no copy
3/epur multiple spaces sauf dans les "bloc" ou 'bloc' -> on aura qu un seul espace la ou il y en avait  plusieurs et dans les "blocs" les espaces seront preserves.
4/Trim les spaces aux extremites X   X de la string du buffer
5/RETOKENIZE / si echo : PAS de retokenize
6/DEQUOTE(quote removal sauf dans les "bloc" ou 'bloc') (epurer la string des signes "" ou ' sauf quand ils sont dans  des "bloc" ou 'bloc')
*/

/*
void	ft_is_trim_and_clear_and_retokenize_token_allowed(t_list *lst_token)
{


}


char *ft_substitute(t_list *lst_token)
{
	char *expanded_content;
	expanded_content == NULL;

	//on va effectuer la substitution /expansion de notre VAR
	return(expanded_content);

}


void	ft_get_token_expansion(void)
{
}

*/

// void ft_get_scope_expand(void)
//{

//}


char 	*ft_get_expand_lengh_for_malloc(size_t start_expand_pos, size_t end_expand_pos)
{
	size_t size_content;
	char *expand;

	size_content = end_expand_pos - start_expand_pos;
	expand = ft_init_cstring(&expand, size_content + 1, '\0');
	return(expand);
}

char 	*ft_get_expand_content(size_t start_expand_pos, size_t end_expand_pos, char *content)
{
	char *expand;
	
	expand = ft_get_expand_lengh_for_malloc(start_expand_pos, end_expand_pos);
	expand = ft_memcpy(expand, &content[start_expand_pos], end_expand_pos - start_expand_pos + 1);
	return(expand);
}


int ft_strcmp_char(char c1, char c2)
{
	return (c1 - c2);
}

int ft_is_expand_unvalidated(char **invalidators, char c)
{
	int i;

	i = 0;
	while (invalidators[i] != NULL)
	{
		if (ft_strcmp_char(invalidators[i][0], c) == 0)
			return (1);
		i++;
	}

	return (0);
}

int ft_isdigit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int ft_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}

int ft_is_alphanum(char c)
{

	if (ft_isalpha(c) || ft_isdigit(c))
		return (1);
	return (0);
}


char *ft_get_scope_expand(size_t end_expand_pos, size_t start_expand_pos, char *str, int *flag_expand_here)
{
	char *expand;
	expand = NULL;
	*flag_expand_here = 0;
	expand = ft_get_expand_content(start_expand_pos, end_expand_pos, str);
	return(expand);
}


size_t		ft_isunderscore(char *str, size_t i)
{
	size_t	i_tmp;

	i_tmp = i;
	while(str[i] && str[i] == '_')
		i++;
	if(i_tmp == i)
		return(0);
	else
		return(i -1);
}


size_t ft_get_end_expand(char *str, t_expand *exp, char **expand, size_t i)
{	
	if(exp->quoting_rule == 1)//ici c est pour le cas :  $VAR'$USER' ->il faut delimiter la fin de l expand $VAR
	{
		if (str[i] == '\"' || str[i] == '\'') 
			*expand = ft_get_scope_expand(i - 1, exp->start_expand_pos, str, &(exp->flag_expand_here));
		else if(str[i - 1] != '$' && (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13)))//- des qu on rencontre un espace "$VAR l"
			*expand = ft_get_scope_expand(i - 1, exp->start_expand_pos, str, &(exp->flag_expand_here));
	}
	if (exp->quoting_rule == 0 || exp->quoting_rule == 2)
	{	
		if(exp->quoting_rule == 2 && str[i - 1] != '$' && (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13)))//- des qu on rencontre un espace "$VAR l"
			*expand = ft_get_scope_expand(i - 1, exp->start_expand_pos, str, &(exp->flag_expand_here));
		else if (str[i -1] == '$' && ft_isdigit(str[i]) == 1) //"$2000"
			*expand = ft_get_scope_expand(i, exp->start_expand_pos, str, &(exp->flag_expand_here));
		else if (str[i -1] == '$' && ft_isunderscore(str, i)> 0) //"$2000"
		{
			i = ft_isunderscore(str, i);
			if(str[i + 1] == '\0')
			{
				*expand = ft_get_scope_expand(i, exp->start_expand_pos, str, &(exp->flag_expand_here));
				exp->flag_expand_here = 1;
			}
		}
		else if (str[i] == '\"' || str[i] == '\'') //$"VAR"
			*expand = ft_get_scope_expand(i - 1, exp->start_expand_pos, str, &(exp->flag_expand_here));
		else if (str[i] == '$' && str[i - 1] != '$') //$VAR$
			*expand = ft_get_scope_expand(i - 1, exp->start_expand_pos, str, &(exp->flag_expand_here));
		else if(str[i] == '$' && str[i -1] == '$' && str[i + 1] == '\0') //$VAR$$\0
			*expand = ft_get_scope_expand(i, exp->start_expand_pos, str, &(exp->flag_expand_here));	
		else if (ft_is_alphanum(str[i]) == 0) //$VAR+
		{
			if(ft_isunderscore(str, i) > 0) //$VAR_
			{
				i = ft_isunderscore(str, i);
				if(str[i + 1] == '\0')
				{
					*expand = ft_get_scope_expand(i, exp->start_expand_pos, str, &(exp->flag_expand_here));
					exp->flag_expand_here = 1;
				}
			}
			else if(!(str[i] == '$' && str[i -1] == '$')) // cs de $VAR$$$
				*expand = ft_get_scope_expand(i - 1, exp->start_expand_pos, str, &(exp->flag_expand_here));
		}
		else if (str[i + 1] == '\0') //TODO
		{
			*expand = ft_get_scope_expand(i, exp->start_expand_pos, str, &(exp->flag_expand_here));
			exp->flag_expand_here = 1;
		}
	}
	return(i);	
}

void ft_get_start_expand(char *str, t_expand *exp, size_t i, char *buffer)
{	
	char 	*invalidators[] = {"+", ",", "}", "]", "~", "=", NULL};

	exp->flag_expand_here = 1;
	if (str[i + 1] == '\0' || ft_is_expand_unvalidated(invalidators, str[i + 1]) == 1) //|| str[i + 1] ==  '\"')//cas du ls >VAR$
		{
			exp->flag_expand_here = 0;
			buffer[exp->j] = str[i];
			exp->j = exp->j + 1;
		}
	if (exp->flag_expand_here == 1)
		{
			exp->start_expand_pos = i;
			exp->flag_expand_in_token = 1;
		}
}


char *ft_substitute(char *expand, char *envp[])
{
	expand = ft_get_var(envp, expand);

	return(expand);
}

int ft_is_expand_here(char *str, char *buffer, char *envp[])
{
	t_expand 	*exp;
	char 		*expand;
	size_t 		i;
	
	expand = NULL;
	exp = NULL;
	i = 0;
	exp = ft_struct_init_expand(&exp);
	while (str[i])
	{
		exp->quoting_rule_adequate = ft_get_token_quoting_rule2(str, i, &(exp->quoting_rule), &(exp->quoting_rule_adequate));
		if (exp->flag_expand_here == 1)
			i = ft_get_end_expand(str, exp, &expand, i);
		if (expand != NULL)
				{
					printf("expand = %s \n", expand);
					expand = ft_substitute(expand, envp);
					if(expand != NULL)
						{
							ft_memcpy(&buffer[exp->j], expand, ft_strlen(expand));
							exp->j = exp->j + ft_strlen(expand);
						}
					free(expand);
					expand = NULL;
				}
		if (str[i] == '$' && exp->quoting_rule != 1 && exp->flag_expand_here != 1) //&& que $ n est pas suivi de '\0' ->suivi de \0 signifie que ce n est pas un expand , mais simplement un caractere $
			ft_get_start_expand(str, exp, i, buffer);
		else
			{
				if(exp->flag_expand_here != 1)
				{
					buffer[exp->j] = str[i];
					exp->j++;
				}
			}
		i++;
	}
	//printf("start_expand_pos = %zu, end_expand_pos = %zu \n", start_expand_pos, end_expand_pos);
	printf("<%s>\n", buffer);
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

char *ft_dequote(char *str)
{
	return(str);
}

char *ft_epur_buffer_ws(char *buffer)
{//TODO rajouter la condition pas dans les blocs de quoting
	int i;
	int j;
	int flag;
	char *new_buffer;
	int quoting_rule;
	int quoting_rule_adequate;

	new_buffer = ft_init_string(1096);
	i = 0;
	j = 0;
	flag = 0;
	if(buffer[i] == ' ' || buffer[i] == '\t')
		i++;
	quoting_rule_adequate = 0;
	quoting_rule = 0;
	while(buffer[i])
	{
		quoting_rule_adequate = ft_get_token_quoting_rule2(buffer, i, &quoting_rule, &quoting_rule_adequate);
		 if(buffer[i] == ' ' || buffer[i] == '\t')//if(exp->quoting_rule == 0 && (buffer[i] == ' ' || buffer[i] == '\t'))
			{
				flag = 1;
				if(quoting_rule == 2 || quoting_rule == 1)
					{
						new_buffer[j] = ' ';
						j++;
						flag = 0;
					}
			}
		if(!(buffer[i] == ' ' || buffer[i] == '\t'))
		{
			if(flag)
				{
					new_buffer[j] = ' ';
					j++;
				}
			flag = 0;
			new_buffer[j] = buffer[i];
			j++;
		}
		if(flag == 1 && buffer[i + 1] == '\0')
			new_buffer[j] = ' ';
		i++;
	}
	ft_update_string(&buffer, new_buffer);
	return(new_buffer);
} 

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*tab;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!s)
		return (NULL);
	if (!(tab = (char*)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	while (s[j])
	{
		if (start <= j && i < len)
		{
			tab[i] = s[j];
			i++;
		}
		j++;
	}
	tab[i] = '\0';
	return (tab);
}

static int	ft_isset(char *set, char c)
{
	while (*set)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

char		*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	len;
	char	*set2;
	char	*s;

	if (!s1 || !set)
		return (NULL);
	set2 = (char *)set;
	s = (char *)s1;
	start = 0;
	while (s[start] && (ft_isset(set2, s[start]) == 1))
		start++;
	len = ft_strlen((char *)&s[start]);
	if (len != 0)
		while (s[start + len - 1]
				&& (ft_isset(set2, s[start + len - 1]) == 1))
			len--;
	return (ft_substr(s1, start, len));
}

int ft_is_expand_to_substitute(t_list *lst_token, char *envp[])
{
	char *str;
	int i;
	int i_save;
	char *buffer;
	char *trimmed_buffer;
	// char *backup_content;

	buffer = ft_init_string(1096);
	str = lst_token->content;
	trimmed_buffer = NULL;
	// backup_content = ft_strdup(lst_token->content);

	i = 0;
	i_save = i;
	if (str == NULL)
		return (0);
	if (ft_strcmp(lst_token->content, "$") == 0)
		{
			free(buffer);
			return (0);
		}
	if(lst_token->prev)
	{
		if (lst_token->prev->title == redir_heredoc)
		{
			printf("heredoc to not expand\n");
			free(buffer);
			return(0);
		}
	}
	if (ft_is_expand_here(str, buffer, envp) == 1)
	{
		printf("expand is here\n");
		printf("buffer apres substitution :<%s>\n", buffer);

		if(*buffer == '\0')
			printf("buffer vide alors que expand a ete substitute\n");//il faudra delete le token de la liste chainee
		else
			{
				buffer = ft_epur_buffer_ws(buffer);
				printf("buffer apres epur_buffer :epur <%s> epur\n", buffer);
				trimmed_buffer = ft_strtrim(buffer, " ");
				printf("buffer apres trim_buffer :trim <%s> trim\n", trimmed_buffer);
				//ON VA ABORDER LA SUITE de LEXPANSION DURANT LE PARSING (phase de retokenization et de DEQUOTE car nous avons besoin de savoir si la cmd principale qui gouverne notre expand est echo )

			}
	}
	ft_dequote(str);
	free(buffer);
	free(trimmed_buffer);
	return (0);
}

void ft_expand_and_retokenize(t_list *lst_token, char *envp[])
{
	t_list *tmp;

	tmp = lst_token;
	if (tmp == NULL)
		return;
	while (lst_token)
	{

		ft_is_expand_to_substitute(lst_token, envp);

		// ft_dequote(lst_token);

		lst_token = lst_token->next;
	}
	lst_token = tmp;
	return;
}