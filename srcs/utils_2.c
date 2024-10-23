/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raamorim <raamorim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 14:01:21 by raamorim          #+#    #+#             */
/*   Updated: 2024/10/23 14:01:22 by raamorim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

static char	*join_path(char *path, char *cmd)
{
	char	*directory;
	char	*full_path;

	if (!path || !cmd)
		return (NULL);
	directory = ft_strjoin(path, "/");
	if (!directory)
		return (NULL);
	full_path = ft_strjoin(directory, cmd);
	free(directory);
	if (!full_path)
		return (NULL);
	return (full_path);
}

static char	*check_paths(char **paths, char *cmd)
{
	int		i;
	char	*correct_path;

	if (!cmd || !(paths) || !(*paths))
		return (NULL);
	i = 0;
	while (paths[i])
	{
		correct_path = join_path(paths[i], cmd);
		if (!correct_path)
			return (NULL);
		if (access(correct_path, F_OK | X_OK) == 0)
		{
			ft_free(paths);
			return (correct_path);
		}
		free(correct_path);
		i++;
	}
	ft_free(paths);
	return (NULL);
}

char	*find_path(char *cmd, char **env)
{
	char	**cmd_flags;
	char	**paths;
	char	*path;

	if (!cmd || !env)
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
	path = check_paths(paths, cmd_flags[0]);
	ft_free(cmd_flags);
	return (path);
}
