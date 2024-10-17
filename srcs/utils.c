/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raamorim <raamorim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 13:21:02 by raamorim          #+#    #+#             */
/*   Updated: 2024/10/14 14:01:23 by raamorim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	handle_errors(int error, char *argv[])
{
	if (error == 1)
	{
		ft_putstr_fd("./pipex infile cmd cmd outfile\n", 2);
		exit(1);
	}
	if (error == 2 && argv)
	{
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		exit(2);
	}
	if (error == 3)
	{
		ft_putstr_fd("Error creating the pipe\n", 2);
		exit(3);
	}
	if (error == 4)
	{
		ft_putstr_fd("Error with fork\n", 2);
		exit(4);
	}
}
void handle_errors_plus(int error)
{
	if (error == 5)
	{
		ft_putstr_fd("Error duplicating file descriptor\n", 2);
		exit(5);
	}
	if (error == 6)
	{
		ft_putstr_fd("Error opening/creating the file\n", 2);
		exit(6);
	}
	if (error == 7)
	{
		ft_putstr_fd("Waitpid failed\n", 2);
		exit(7);
	}
	if (error == 8)
	{
		ft_putstr_fd("./pipex <file1> <cmd1> <cmd2> <...> <file2>\n", 2);
		exit(8);
	}

}

void	ft_free(char **cmd)
{
	size_t	i;

	if (!(*cmd) || !cmd)
		return ;
	i = 0;
	while (cmd[i])
	{
		free(cmd[i]);
		i++;
	}
	free(cmd);
}

/* variable name = PATH=
value = return = /home/raamorim/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin */
char	*get_env(char *variable_name, char **env)
{
	int		i;
	int			len;
	char	*value; // tudo o que vem depois do nome da variavel ex : PATH=;
	if (!variable_name || !env || !(*env))
		return (NULL);
	len = ft_strlen(variable_name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], variable_name, len) == 0 && env[i][len] == '=')
		{
			value = ft_strchr(env[i], '=');
			if (!value)
				return (NULL);
			else
				return (value + 1);
		}
		i++;
	}
	return (NULL);
}

char	*find_path(char *cmd, char **env)
{
	int		i;
	char	**cmd_flags;
	char	**paths;
	char	*correct_path;
	char	*directory;
	
	if (!cmd || !(*cmd) || !env || !(*env))
		return (NULL);
	paths = ft_split(get_env("PATH", env), ':');
	if (!paths)
		return (NULL);
	cmd_flags = ft_split(cmd, ' ');
	if (!cmd_flags)
	{
		ft_free(paths);
		return (NULL);
	}
	i = 0;
	while (paths[i])
	{
		directory = ft_strjoin(paths[i], "/");
		correct_path = ft_strjoin(directory, cmd_flags[0]);
		free(directory);
		if (access(correct_path, F_OK | X_OK) == 0)
		{
			ft_free(paths);
			ft_free(cmd_flags);
			return (correct_path);
		}
		free(correct_path);
		i++;
	}
	ft_free(paths);
	ft_free(cmd_flags);
	return (NULL);
}
