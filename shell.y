goal: command_list;
arg_list:
		arg_list WORD
		| /*empty*/
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
		|/*empty*/
		;

background_optional:
		AMPERSAND
		|/*empty*/
		;

command_line:
		pipe_list io_modifier_list background_optional NEWLINE
		| NEWLINE /*accept empty command line*/
		| error NEWLINE{yyerrok}
		;
		/*error recovery*/
command_list:
		command_list command_line
		; /*command loop*/
		