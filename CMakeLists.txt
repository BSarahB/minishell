cmake_minimum_required(VERSION 3.0.0)

project(my_minishell)
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-Wall -Wextra -Werror")
#target_include_directories(ftprintf PUBLIC .)

#target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR/includes})
include_directories(includes)
add_executable(${PROJECT_NAME} includes/minishell.h srcs/main.c srcs/shell_lexer.c srcs/shell_parser.c srcs/parsing_lst_token_in_cmd_and_args.c srcs/utils.c srcs/utils_parser.c srcs/utils_parser2_malloc_redir_files.c srcs/utils_parser3_count.c srcs/struct_init_lexer.c srcs/struct_init_parser.c srcs/executor.c srcs/get_path.c srcs/setting_redirections.c srcs/update_free_memory.c srcs/update_free_memory_2.c srcs/check_validity_of_line_input.c srcs/display_error.c srcs/tokenize.c srcs/get_data_token.c srcs/lst_token.c srcs/struct_init_execution.c srcs/first_simpleCmd_w_infile_setting_redirections.c srcs/last_simpleCmd_setting_redirections.c srcs/regular_simpleCmd_setting_redirections.c srcs/utils_settings_redirections.c srcs/heredoc_interaction.c srcs/update_free_memory_3.c srcs/expand.c srcs/parse_heredoc.c srcs/parse_heredoc_2.c srcs/parse_redir_head_token.c srcs/parse_redir_middle_token.c srcs/del_and_parse.c srcs/lst_token_2.c srcs/del_and_parse_2.c srcs/display_error_2.c srcs/get_token_type.c srcs/heredoc_interaction_2.c srcs/exit_status.c srcs/setting_redirections_2.c srcs/utils_2.c srcs/utils_3.c srcs/utils_lexer.c srcs/struct_init_expand.c srcs/expand_part_1.c srcs/expand_part_2.c srcs/export.c srcs/get_lst_envp.c srcs/lst_envp.c srcs/unset.c srcs/env.c srcs/echo.c srcs/cd.c srcs/pwd.c srcs/exit.c srcs/exit_code.c srcs/cd_1.c srcs/cd_2.c srcs/cd_3.c srcs/cd_utils.c srcs/cd_utils2.c srcs/check_validity_of_line_input2.c srcs/dequote.c srcs/display_error2.c srcs/exit2.c srcs/get_end_expand.c srcs/expand_utils.c srcs/expand_utils2.c srcs/get_expand.c srcs/get_expand2.c srcs/get_expand3.c srcs/expand_part3.c srcs/expand_epur_and_trim_buffer.c srcs/export_check_var_in.c srcs/export_check_key.c srcs/get_data_token_quoting_rule.c srcs/lst_envp2.c srcs/lst_token_3.c srcs/setting_exit_status.c
srcs/child_process.c srcs/cd_4.c srcs/expand1.c srcs/expand2.c srcs/expand_part4.c srcs/export_check_var_in2.c srcs/get_data_token_quoting_rule2.c srcs/get_data_token_quoting_rule3.c srcs/get_end_expand3.c srcs/get_end_expand2.c
srcs/get_path1.c
srcs/last_simpleCmd_setting_redirections2.c
srcs/lst_token_4.c
srcs/parsing_lst_token_in_cmd_and_args2.c
srcs/regular_simpleCmd_setting_redirections2.c
srcs/setting_redirections_3.c
srcs/setting_redirections_4.c
srcs/setting_redirections_5.c
srcs/shell_lexer2.c
srcs/shell_parser2.c
srcs/shell_parser3.c
srcs/struct_init_parser2.c
srcs/tokenize2.c
srcs/unset2.c
srcs/unset3.c
srcs/update_free_memory_4.c
srcs/utils_parser3_count2.c
srcs/utils_parser2.c
srcs/main_helper.c
)
target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address -fsanitize=leak)
target_link_libraries(${PROJECT_NAME} readline) #  -lreadline-> lreadline.a
target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address -fsanitize=leak)
