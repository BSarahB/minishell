cmake_minimum_required(VERSION 2.8)

project(my_minishell)
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-Wall -Wextra -Werror")
#target_include_directories(ftprintf PUBLIC .)

#target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR/includes})
include_directories(includes)
add_executable(${PROJECT_NAME} includes/minishell.h srcs/main.c srcs/shell_lexer.c srcs/shell_parser.c srcs/utils.c srcs/ft_struct_init.c srcs/executor.c srcs/get_path.c srcs/setting_redirections.c srcs/update_free_memory.c
)
target_link_libraries(${PROJECT_NAME} readline) #  -lreadline-> lreadline.a
add_link_options(-fsanitize=address -fsanitize=leak)
