SRCS_DIR = srcs
SRCS = $(SRCS_DIR)/pipex.c $(SRCS_DIR)/utils.c $(SRCS_DIR)/utils_2.c
SRCS_BONUS = $(SRCS_DIR)/bonus.c $(SRCS_DIR)/utils.c $(SRCS_DIR)/utils_2.c

NAME = pipex
BONUS_NAME = pipex_bonus

COMPRESS = ar rcs
RM = rm -f
CFLAGS = -Wall -Wextra -Werror -g
CC = cc

OBJ_DIR = obj

OBJS = $(SRCS:$(SRCS_DIR)/%.c=$(OBJ_DIR)/%.o)
BONUS_OBJS = $(SRCS_BONUS:$(SRCS_DIR)/%.c=$(OBJ_DIR)/%.o)

LIBFT_DIR = ./includes/libft
LIBFT = $(LIBFT_DIR)/libft.a

FT_PRINTFDIR = ./includes/libft/ft_printf
FT_PRINT = $(FT_PRINTFDIR)/libftprintf.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT) $(FT_PRINT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(FT_PRINT)
	@echo "$(NAME) ready!"

bonus: $(BONUS_NAME)

$(BONUS_NAME): $(BONUS_OBJS) $(LIBFT) $(FT_PRINT)
	$(CC) $(CFLAGS) -o $(BONUS_NAME) $(BONUS_OBJS) $(LIBFT) $(FT_PRINT)
	@echo "$(BONUS_NAME) ready!"

$(OBJ_DIR)/%.o: $(SRCS_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)


$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR) --no-print-directory

$(FT_PRINT):
	@$(MAKE) -C $(FT_PRINTFDIR) --no-print-directory


clean:
	@$(RM) -r $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean --no-print-directory
	@$(MAKE) -C $(FT_PRINTFDIR) clean --no-print-directory

fclean: clean
	@$(RM) $(NAME) $(BONUS_NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean --no-print-directory
	@$(MAKE) -C $(FT_PRINTFDIR) fclean --no-print-directory


re: fclean all

.PHONY: all clean fclean re bonus
