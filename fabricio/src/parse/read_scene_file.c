/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_scene_file.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albartol <albartol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 14:25:58 by albartol          #+#    #+#             */
/*   Updated: 2024/07/09 16:35:38 by albartol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cube3d.h>

static void	remove_newline(char *str)
{
	size_t	i;

	i = 0;
	while (str[i] && str[i] != '\n')
		i++;
	str[i] = 0;
}

static t_list	*read_file_error(t_list *file_content)
{
	if (file_content)
		ft_lstclear(&file_content, free);
	perror(NULL);
	return (NULL);
}

static t_list	*read_file(int fd)
{
	size_t	i;
	char	*line_read;
	t_list	*file_content;
	t_list	*temp;

	i = 0;
	file_content = NULL;
	while (i < MAX_LINES)
	{
		line_read = get_next_line(fd);
		if (!line_read && errno == ENOMEM)
			return (read_file_error(file_content));
		if (!line_read)
			break ;
		remove_newline(line_read);
		temp = ft_lstnew(line_read);
		if (!temp)
		{
			free(line_read);
			return (read_file_error(file_content));
		}
		ft_lstadd_back(&file_content, temp);
		i++;
	}
	return (file_content);
}

int	read_scene_file(t_scene_file *scene_info, const char *file_path)
{
	int		fd;

	if (check_file_type(file_path, ".cub"))
	{
		print_error("Invalid file type.\n");
		return (EXIT_FAILURE);
	}
	fd = open(file_path, O_RDONLY | O_DIRECTORY);
	if (fd > 0)
	{
		close(fd);
		return (exit_msg("Invalid file type.\n", EXIT_FAILURE));
	}
	fd = open(file_path, O_RDONLY);
	if (fd == -1)
	{
		perror(NULL);
		return (EXIT_FAILURE);
	}
	scene_info->file_content = read_file(fd);
	close(fd);
	if (!scene_info->file_content)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
