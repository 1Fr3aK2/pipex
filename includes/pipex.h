#ifndef PIPEX_H
#define PIPEX_H

# include <unistd.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include<sys/types.h>
# include<sys/stat.h>
# include <fcntl.h>  
# include <stdlib.h>
#include "../includes/libft/libft.h"
#include "../includes/libft/ft_printf/ft_printf.h"

char	*get_env(char *name, char **env);
char	*get_path(char *cmd, char **env);
void	exec(char *argv, char **env);
void	ft_free_tab(char **tab);
void	handle_errors(int errors);
int	ft_strcmp(char *s1, char *s2);


#endif