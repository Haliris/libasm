# Syscall table:  https://blog.rchapman.org/posts/Linux_System_Call_Table_for_x86_64/
# instructions cheatsheet: https://cs.brown.edu/courses/cs033/docs/guides/x64_cheatsheet.pdf


ASM = nasm
AR = ar
RANLIB = ranlib
ASM_FLAGS = -g -f elf64 
NAME = libasm.a

ASM_SRC = 	src/ft_strlen.s \
			src/ft_strcpy.s \
			src/ft_strcmp.s \
			src/ft_write.s \
			src/ft_read.s \
			src/ft_strdup.s \
			src/ft_atoi_base.s

ASM_OBJ = $(ASM_SRC:.s=.o)

ASM_SRC_BONUS =	src/ft_list_size_bonus.s \
				src/ft_list_push_front_bonus.s \
				src/ft_list_sort_bonus.s \
				src/ft_list_remove_if_bonus.s


ASM_OBJ_BONUS = $(ASM_SRC_BONUS:.s=.o)

CC = gcc
C_FLAGS = -Wall -Wextra -Werror -g3

TEST_SRC = src/main.c
TEST_INCLUDE = -I src/
TEST_NAME = unit_tests

TEST_SRC_BONUS = src/main_bonus.c
TEST_NAME_BONUS = unit_tests_bonus

all: $(NAME) unit_tests

$(NAME): $(ASM_OBJ)
	$(AR) rcs $@ $^
	$(RANLIB) $@

%.o: %.s
	$(ASM) $(ASM_FLAGS) -o $@ $<

unit_tests:
	$(CC) $(C_FLAGS) $(TEST_SRC) $(NAME) $(TEST_INCLUDE) -o $(TEST_NAME)

bonus: $(ASM_OBJ_BONUS)
	ar rc $(NAME) $(ASM_OBJ_BONUS)

unit_tests_bonus:
	$(CC) $(C_FLAGS) $(TEST_SRC_BONUS) $(NAME) $(TEST_INCLUDE) -o $(TEST_NAME_BONUS)

clean:
	rm -rf $(ASM_OBJ)

fclean: clean
	rm -rf $(NAME)
	rm -rf $(TEST_NAME)

re: fclean all

.PHONY: all clean fclean re
