
ASM = nasm
AR = ar
RANLIB = ranlib
CFLAGS = -felf64
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
	$(ASM) $(CFLAGS) -o $@ $<

clean:
	rm -rf $(OBJ) $(NAME)

fclean: clean
	rm -rf $(NAME)

.PHONY: all clean fclean

