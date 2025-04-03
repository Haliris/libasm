# Syscall table:  https://blog.rchapman.org/posts/Linux_System_Call_Table_for_x86_64/
# instructions cheatsheet: https://cs.brown.edu/courses/cs033/docs/guides/x64_cheatsheet.pdf


ASM = nasm
AR = ar
RANLIB = ranlib
ASM_FLAGS = -f elf64
NAME = libasm.a

SRC = 	src/ft_strlen.s \
		src/ft_strcpy.s \
		src/ft_strcmp.s \
		src/ft_write.s \
		src/ft_read.s \
		src/ft_strdup.s

OBJ = $(SRC:.s=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(AR) rcs $@ $^
	$(RANLIB) $@

%.o: %.s
	$(ASM) $(ASM_FLAGS) -o $@ $<

clean:
	rm -rf $(OBJ) $(NAME)

fclean: clean
	rm -rf $(NAME)

.PHONY: all clean fclean
