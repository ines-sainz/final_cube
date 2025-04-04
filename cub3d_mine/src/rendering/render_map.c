/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isainz-r <isainz-r@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 09:40:37 by isainz-r          #+#    #+#             */
/*   Updated: 2025/03/26 09:40:39 by isainz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cube3d.h"

int	load_images(t_game *game)
{
	int	i;

	game->mlx->img = mlx_new_image(game->mlx->mlx, WIDTH, HEIGHT);
	game->mlx->data = mlx_get_data_addr(game->mlx->img, &game->mlx->bpp,
		&game->mlx->size_line, &game->mlx->endian);
	i = 0;
	while (i < 4)
	{
		game->textures[i]
	}
	
}

int	render_map(t_game *game)
{
	game->mlx->mlx = mlx_init();
	if (!game->mlx->mlx)
		return (errors("Problem with the MLX\n"));
	game->mlx->mlx_win = mlx_new_window(game->mlx->mlx, WIDTH, HEIGHT, "CUB3D");
	if (!game->mlx->mlx_win)
		return (errors("problem with the MLX\n"));
	if (load_images(game) == 1)
		return (errors("Problem loading the images\n"));
	
}
