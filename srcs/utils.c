/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raamorim <raamorim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 20:00:33 by decortejohn       #+#    #+#             */
/*   Updated: 2024/10/02 16:27:09 by raamorim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	handle_errors(int error)
{
	if (error == 1)
		ft_printf("./pipex infile cmd cmd outfile\n", 2);
	exit(1);
}

void	ft_free(char **cmd)
{
	size_t	i;

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
	int		len;
	char	*value; // tudo o que vem depois do nome da variavel ex : PATH=;
	if (!variable_name)
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

char	*find_path( char *cmd, char **env)
{
	char	**cmd_flags;// separa o comando e as flags caso tenha 
	char	**path;//armazena os diretorios extraidos por path;
	char	*correct_path;//caminho do executavel(cmd);
	char	*directory;//armazena parte dos diretorios enquanto procura o correto;
	int		i;
	
	i = 0;
	path = ft_split(get_env("PATH", env), ':');
	cmd_flags = ft_split(cmd, ' ');
	while (path[i++])
	{
		directory = ft_strjoin(path[i], "/"); //Combina o diretório atual com (/) para formar a base do caminho do executável.
		correct_path = ft_strjoin(directory, *cmd_flags); //adiciona o comando ao diretorio 
		free(directory);
		if (access(correct_path, F_OK | X_OK) == 0) //verifica se existe o caminho e se e executavel
		{
			ft_free(cmd_flags);
			return (correct_path);
		}
		free(correct_path);
	}
	ft_free(path); //liberta a memoria alocada 
	ft_free(cmd_flags); //liberta a memoria alocada
	return (correct_path); // se nenhum caminho for encontrado retorna o comando passado;
}
