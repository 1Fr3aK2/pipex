/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raamorim <raamorim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 12:39:38 by raamorim          #+#    #+#             */
/*   Updated: 2024/10/23 14:03:03 by raamorim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>  
# include <stdlib.h>
# include <errno.h>
# include "../includes/libft/libft.h"
# include "../includes/libft/ft_printf/ft_printf.h"

char	*get_env(char *variable_name, char **env);
char	*find_path(char *cmd, char **env);
void	exec(char *argv, char **env);
void	ft_free(char **cmd);
void	handle_errors(int error, char *argv[]);
void	close_pipe(int fd[2]);
void	handle_errors_plus(int error);

//bonus
void	childo_process(char *argv, char **env, int *infile);
void	last_child(char *argv, char **env, int read_fd, char *outfile_path);
void	exec(char *argv, char **env);
void	close_fds(int i);
#endif