# Syscall table:  https://blog.rchapman.org/posts/Linux_System_Call_Table_for_x86_64/
# instructions cheatsheet: https://cs.brown.edu/courses/cs033/docs/guides/x64_cheatsheet.pdf

ASM         = nasm
AR          = ar
RANLIB      = ranlib
ASM_FLAGS   = -g -f elf64
NAME        = libasm.a
NAME_BONUS  = libasm_bonus.a

# Main sources
ASM_SRC     = src/ft_strlen.s \
              src/ft_strcpy.s \
              src/ft_strcmp.s \
              src/ft_write.s \
              src/ft_read.s \
              src/ft_strdup.s \
              src/ft_atoi_base.s
ASM_OBJ     = $(ASM_SRC:.s=.o)

# Bonus sources
ASM_SRC_BONUS = src/ft_list_size_bonus.s \
                src/ft_list_push_front_bonus.s \
                src/ft_list_sort_bonus.s \
                src/ft_list_remove_if_bonus.s
ASM_OBJ_BONUS = $(ASM_SRC_BONUS:.s=.o)

# Unit test sources
CC          = gcc
C_FLAGS     = -Wall -Wextra -Werror -g3
TEST_INCLUDE = -I src/
TEST_SRC    = src/main.c
TEST_OBJ    = $(TEST_SRC:.c=.o)
TEST_NAME   = unit_tests

# Bonus test
TEST_SRC_BONUS = src/main_bonus.c
TEST_OBJ_BONUS = $(TEST_SRC_BONUS:.c=.o)
TEST_NAME_BONUS = unit_tests_bonus

# Default rule
all: $(NAME) $(TEST_NAME)

# Main library
$(NAME): $(ASM_OBJ)
	$(AR) rcs $@ $^
	$(RANLIB) $@

# Bonus target
bonus: $(NAME_BONUS) $(TEST_NAME_BONUS)

$(NAME_BONUS): $(ASM_OBJ_BONUS)
	$(AR) rcs $@ $^
	$(RANLIB) $@

# Test binary for main
$(TEST_NAME): $(TEST_OBJ) $(NAME)
	$(CC) $(C_FLAGS) $(TEST_INCLUDE) -o $@ $^ $(NAME)

$(TEST_NAME_BONUS): $(TEST_OBJ_BONUS) $(NAME_BONUS)
	$(CC) $(C_FLAGS) $(TEST_INCLUDE) -o $@ $^ $(NAME_BONUS)

# Assembly compilation
%.o: %.s
	$(ASM) $(ASM_FLAGS) -o $@ $<

# C compilation
%.o: %.c
	$(CC) $(C_FLAGS) -c $(TEST_INCLUDE) -o $@ $<

# Clean rule
clean:
	rm -f $(ASM_OBJ) $(ASM_OBJ_BONUS) $(TEST_OBJ) $(TEST_OBJ_BONUS)

fclean: clean
	rm -f $(NAME) $(TEST_NAME) $(TEST_NAME_BONUS)

re: fclean all
