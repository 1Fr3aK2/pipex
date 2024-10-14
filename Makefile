# Definindo variáveis
SRCS_DIR = srcs
SRCS = $(SRCS_DIR)/pipex.c $(SRCS_DIR)/utils.c
SRCS_BONUS = $(SRCS_DIR)/bonus.c $(SRCS_DIR)/utils.c

# Nome do programa
NAME = pipex
BONUS_NAME = pipex_bonus

COMPRESS = ar rcs
RM = rm -f
CFLAGS = -Wall -Wextra -Werror -g
CC = cc

# Pasta para os arquivos objetos
OBJ_DIR = obj

# Lista de arquivos objetos
OBJS = $(SRCS:$(SRCS_DIR)/%.c=$(OBJ_DIR)/%.o)
BONUS_OBJS = $(SRCS_BONUS:$(SRCS_DIR)/%.c=$(OBJ_DIR)/%.o)

LIBFT_DIR = ./includes/libft
LIBFT = $(LIBFT_DIR)/libft.a

FT_PRINTFDIR = ./includes/libft/ft_printf
FT_PRINT = $(FT_PRINTFDIR)/libftprintf.a

# Regras
all: $(NAME)

$(NAME): $(OBJS) $(LIBFT) $(FT_PRINT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(FT_PRINT)
	@echo "$(NAME) ready!"

# Regra para compilar o bônus
bonus: $(BONUS_NAME)

$(BONUS_NAME): $(BONUS_OBJS) $(LIBFT) $(FT_PRINT)
	$(CC) $(CFLAGS) -o $(BONUS_NAME) $(BONUS_OBJS) $(LIBFT) $(FT_PRINT)
	@echo "$(BONUS_NAME) ready!"

# Regra para compilar arquivos .c para .o na pasta obj
$(OBJ_DIR)/%.o: $(SRCS_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Compilar a libft e ft_printf
$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR) --no-print-directory

$(FT_PRINT):
	@$(MAKE) -C $(FT_PRINTFDIR) --no-print-directory

# Limpar os arquivos de objeto e recompilar a libft e ft_printf
clean:
	@$(RM) -r $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean --no-print-directory
	@$(MAKE) -C $(FT_PRINTFDIR) clean --no-print-directory

# Limpar tudo, inclusive os binários
fclean: clean
	@$(RM) $(NAME) $(BONUS_NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean --no-print-directory
	@$(MAKE) -C $(FT_PRINTFDIR) fclean --no-print-directory

# Recompilar tudo
re: fclean all

.PHONY: all clean fclean re bonus
