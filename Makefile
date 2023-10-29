NAME = minishell
CC = clang

INCLUDE_DIR	=	./includes/
SRC_DIR	=	srcs
SRCS	=	$(SRC_DIR)/main.c \
			$(SRC_DIR)/shell_lexer.c \
			$(SRC_DIR)/shell_parser.c \
			$(SRC_DIR)/parsing_lst_token_in_cmd_and_args.c \
			$(SRC_DIR)/utils_parser.c \
			$(SRC_DIR)/utils_parser2_malloc_redir_files.c \
			$(SRC_DIR)/utils.c \
			$(SRC_DIR)/utils_parser3_count.c \
			$(SRC_DIR)/struct_init_lexer.c \
			$(SRC_DIR)/struct_init_parser.c \
			$(SRC_DIR)/executor.c \
			$(SRC_DIR)/get_path.c \
			$(SRC_DIR)/setting_redirections.c \
			$(SRC_DIR)/update_free_memory.c \
			$(SRC_DIR)/update_free_memory_2.c \
			$(SRC_DIR)/check_validity_of_line_input.c \
			$(SRC_DIR)/display_error.c \
			$(SRC_DIR)/tokenize.c \
			$(SRC_DIR)/get_data_token.c \
			$(SRC_DIR)/lst_token.c \
			$(SRC_DIR)/struct_init_execution.c \
			$(SRC_DIR)/first_simpleCmd_w_infile_setting_redirections.c \
			$(SRC_DIR)/last_simpleCmd_setting_redirections.c \
			$(SRC_DIR)/regular_simpleCmd_setting_redirections.c \
			$(SRC_DIR)/utils_settings_redirections.c \
			$(SRC_DIR)/heredoc_interaction.c \
			$(SRC_DIR)/update_free_memory_3.c \
			$(SRC_DIR)/expand.c \
			$(SRC_DIR)/parse_heredoc.c \
			$(SRC_DIR)/parse_heredoc_2.c \
			$(SRC_DIR)/parse_redir_head_token.c \
			$(SRC_DIR)/parse_redir_middle_token.c \
			$(SRC_DIR)/del_and_parse.c \
			$(SRC_DIR)/lst_token_2.c \
			$(SRC_DIR)/expand_part_1.c \
			$(SRC_DIR)/del_and_parse_2.c \
			$(SRC_DIR)/display_error_2.c \
			$(SRC_DIR)/get_token_type.c \
			$(SRC_DIR)/heredoc_interaction_2.c \
			$(SRC_DIR)/exit_status.c \
			$(SRC_DIR)/setting_redirections_2.c \
			$(SRC_DIR)/utils_2.c \
			$(SRC_DIR)/utils_3.c \
			$(SRC_DIR)/utils_lexer.c \
			$(SRC_DIR)/struct_init_expand.c \
			$(SRC_DIR)/expand_part_2.c \
			$(SRC_DIR)/export.c \
			$(SRC_DIR)/get_lst_envp.c \
			$(SRC_DIR)/lst_envp.c \
            $(SRC_DIR)/unset.c \
            $(SRC_DIR)/env.c \
            $(SRC_DIR)/echo.c \
            $(SRC_DIR)/cd.c \
			$(SRC_DIR)/pwd.c \
			$(SRC_DIR)/exit.c \
			$(SRC_DIR)/exit_code.c \
			$(SRC_DIR)/cd_1.c \
			$(SRC_DIR)/cd_2.c \
			$(SRC_DIR)/cd_3.c \
			$(SRC_DIR)/cd_utils.c \
			$(SRC_DIR)/cd_utils2.c \
			$(SRC_DIR)/check_validity_of_line_input2.c \
			$(SRC_DIR)/dequote.c \
			$(SRC_DIR)/display_error2.c\
			$(SRC_DIR)/exit2.c

OBJS = $(SRCS:.c=.o)

RM = rm -f

CFLAGS = -Wall -Wextra -Werror -I $(INCLUDE_DIR) #-fsanitize=address -fsanitize=leak
all:		$(NAME)


.c.o:
		$(CC) $(CFLAGS) -c $< -o $(<:.c=.o)

$(NAME):	$(OBJS)
	$(CC) -o $@ $^ -lreadline #-lasan -lreadline

clean:
			$(RM) $(OBJS) 

fclean:		clean
			$(RM) $(NAME)

re:		fclean all

.PHONY:		clean fclean re all 
