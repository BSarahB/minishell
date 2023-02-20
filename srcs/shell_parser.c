/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 15:55:07 by mbenmesb          #+#    #+#             */
/*   Updated: 2023/02/07 15:55:30 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*3)  PARSER/yacc traite shell.y) == shell_parser.c on va essayer de mettre des regles de grammaire et labels qui vont generer la table de commande
labels: cmd_and_args, arg_list ou pipe_list ou io_modifier_list ou background optional etc..
Grammaire shell dans la forme Backus-Naur et
regles de grammaire et labels du Parser (3)  :

cmd[arg]* [| cmd[arg]*]*		[[> filename] [< filename] [>& filename] [>> filename] [>>& filename]]*			 [&]
[cmd_and_args]+[arg_list]			[io_m]	+	[io_m]	+	[io_modifier]	+	io_m   	+	io_m
  == [pipe_list]					==						[io_modifier_list]								 == background_opitonal

les elements decrits ci dessus sont dits "lablels"
ci dessous a quoi ressemble un fichier shell.y
goal: command_list;
arg_list:
		arg_list WORD
		| *empty
			;
cmd_and_args:
		WORD arg_list
			;

pipe_list:
		pipe_list PIPE cmd_and_args
		|cmd_and_args
		;

io_modifier:
		GREATGREAT WORD
		| GREAT WORD
		| GREATGREAGAMPERSAND WORD
		| GREATAMPERSAND WORD
		| LESS WORD
		;

io_modifier_list:
		io_modifier_list io_modifier
		|*empty
		;

background_optional:
		AMPERSAND
		|*empty
		;

command_line:
		pipe_list io_modifier_list background_optional NEWLINE
		| NEWLINE *accept empty command line
		| error NEWLINE{yyerrok}
		;
		*error recovery
command_list:
		command_list command_line
		; *command loop
		*/

#include "minishell.h"

char	**ft_get_abs_arguments(int i, char **blocks)
{
	char	**abs_arguments;
	char	slash[2];

	slash[0] = '/';
	slash[1] = 0;
	abs_arguments = ft_split(blocks[i], ' ');
	ft_update_string(&abs_arguments[0], ft_strjoin(slash, abs_arguments[0]));
	return (abs_arguments);
}

int	ft_split_line_in_s_cmd(t_cmd *cmd, char *line, char **envp)
{
	char	**arguments;
	char	**abs_arguments;
	int		exec_return;
	size_t		i;
	int		j;

	(void)envp;
	(void)line;
	exec_return = 0;
	i = 0;
	j = 0;

	while (i < cmd->nb_of_simpleCmds)
	{
		arguments = ft_split(cmd->blocks[i], ' ');
		printf("%s\n", arguments[0]);
		abs_arguments = ft_get_abs_arguments(i, cmd->blocks);
		cmd->simpleCmds[i]->cmd_and_args = arguments;
		cmd->simpleCmds[i]->abs_cmd_and_args = abs_arguments;
		i++;
	}
	i = 0;
	while (i < cmd->nb_of_simpleCmds)
	{
		j = 0;
		printf("blocks[%zu]: %s\n",i, cmd->blocks[i]);
		printf("pr cmd_and_args de simple_Cmd %zu on a : \n", i);
		while (cmd->simpleCmds[i]->cmd_and_args[j] != NULL)
		{
			printf("%s*", cmd->simpleCmds[i]->cmd_and_args[j]);
			j++;

		}
		printf("\n");
		j= 0;
		while (cmd->simpleCmds[i]->abs_cmd_and_args[j] != NULL)
		{
			printf("%s*", cmd->simpleCmds[i]->abs_cmd_and_args[j]);
			j++;
		}
		printf("\n");
		i++;
	}
	return (exec_return);
}
