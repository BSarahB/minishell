#include "minishell.h"

void	ft_aff_list_ptr_sur_char_content(t_list *alst) // pour void		*content; de type char *
{
		//	printf("list diplayed: \n");
	while(alst)
	{
	//	printf("%s , %zu\n",(char *)(alst)->content, alst->position); //ou printf("%d - ", (*(int *)(*alst)->content));
		alst = (alst)->next;
	}
}

void	ft_simplify_list(t_list *lst)
{
	t_list	*tmp;
	size_t	position;

	tmp = lst;
	position = 1;

	while (tmp)
	{
		tmp->position = position;
		tmp = tmp->next;
		position++;
	}
}

t_list *ft_lstlast(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void ft_lstadd_back(t_list **alst, t_list *new)
{
	if (new)
		new->next = NULL;
	if (!(*alst))
		*alst = new;
	else
		ft_lstlast(*alst)->next = new;
}

t_list *ft_lstnew_for_lst(t_data *data)
{
	t_list *node;

	node = malloc(sizeof(t_list));
	if (!node)
		return (NULL);
	node->content = data->token->content;
//	node->position = 0;
//	node->end_token_pos = 0;
//	node->start_token_pos = 0;
//	node->start_token_pos_exists = 0;
	node->type = data->token->type;
	node->title = data->token->title;
	node->quoting_rule = data->token->quoting_rule;
//	node->quoting_rule_adequate = 0;
//	node->retokenize_allowed = 0;
	node->next = NULL;
	node->previous = NULL;
	return (node);
}

t_list *ft_lstnew_data_token(char *content)
{
	t_list *list;

	list = malloc(sizeof(t_list));
	if (!list)
		return (NULL);
	list->content = content;
	list->position = 0;
	list->end_token_pos = 0;
	list->start_token_pos = 0;
	list->start_token_pos_exists = 0;
	list->type = 0;
	list->title = -1;
	list->quoting_rule = 0; 
	list->quoting_rule_adequate = 0;
	list->retokenize_allowed = 0;
	list->next = NULL;
	list->previous = NULL;
	return (list);
}

t_list *ft_create_list(void)
{
	t_list *alst;

	alst = NULL;
	return (alst);
}