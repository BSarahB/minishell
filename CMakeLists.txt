cmake_minimum_required(VERSION 3.0.0)

project(my_minishell)
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-Wall -Wextra -Werror")
#target_include_directories(ftprintf PUBLIC .)

#target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR/includes})
include_directories(includes)
add_executable(${PROJECT_NAME} includes/minishell.h srcs/main.c srcs/shell_lexer.c srcs/shell_parser.c srcs/parsing_lst_token_in_cmd_and_args.c srcs/utils.c srcs/utils_parser.c srcs/utils_parser2_malloc_redir_files.c srcs/utils_parser3_count.c srcs/struct_init_lexer.c srcs/struct_init_parser.c srcs/executor.c srcs/get_path.c srcs/setting_redirections.c srcs/update_free_memory.c srcs/update_free_memory_2.c srcs/check_validity_of_line_input.c srcs/display_error.c srcs/tokenize.c srcs/get_data_token.c srcs/lst_token.c srcs/struct_init_execution.c srcs/first_simpleCmd_w_infile_setting_redirections.c srcs/last_simpleCmd_setting_redirections.c srcs/regular_simpleCmd_setting_redirections.c srcs/utils_settings_redirections.c
)
target_link_libraries(${PROJECT_NAME} readline) #  -lreadline-> lreadline.a
add_link_options(-fsanitize=address -fsanitize=leak)

