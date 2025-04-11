/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_rays.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isainz-r <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:01:26 by isainz-r          #+#    #+#             */
/*   Updated: 2025/04/11 10:01:28 by isainz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cube3d.h"

int	is_a_block(t_game *game, float ray_x, float ray_y)
{
	if (ray_x > game->map_width || ray_y > game->map_height
		|| ray_x < 0 || ray_y < 0)
		return (1);
	return (game->map[(int)ray_y][(int)ray_x] == '1');
}

float	pitagoras(float horizontal_step, float vertical_step)
{
	float	hipotenuse;

	hipotenuse = sqrt(horizontal_step * horizontal_step +
		vertical_step * vertical_step);
	return (hipotenuse);
}

void	put_pixel(int x, int y, int color, t_game *game)
{
	int	index;

	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return ;
	index = (y * game->mlx->size_line) + (x * (game->mlx->bpp / 8));
	if (index < 0 || index >= (HEIGHT * game->mlx->size_line))
		return ;
	game->mlx->data[index] = color & 0xFF;
	game->mlx->data[index + 1] = (color >> 8) & 0xFF;
	game->mlx->data[index + 2] = (color >> 16) & 0xFF;
}

unsigned int	get_pixel_from_texture(int wall_plane, int texture_x, int texture_y, t_game *game)
{
	t_info			texture;
	//unsigned int	color;
	char			*image;

	if (wall_plane == NORTH)
		texture = game->info[0];
	if (wall_plane == SOUTH)
		texture = game->info[1];
	if (wall_plane == EAST)
		texture = game->info[2];
	if (wall_plane == WEST)
		texture = game->info[3];
	image = texture.data;
	image += (texture_x * (texture.bpp / 8)) + (texture_y * texture.size_line);
	//color = texture.data[(texture_x * (texture.bpp / 8)) + (texture_y * texture.size_line)];
	return (*(unsigned int *)image);
}

void	draw_texture(int wall_plane, float dist, float plane_x,
	int ray_iteration, t_game *game)
{
	int	wall_height;
	int	wall_y;
	int	i;

	if (dist < 1)
		dist = 1;
	wall_height = HEIGHT / dist;
	wall_y = (HEIGHT - wall_height) / 2;

	i = 0;
	while (i < wall_height)
	{
		put_pixel(ray_iteration, wall_y,
			get_pixel_from_texture(wall_plane, plane_x, 64 * i / wall_height, game), game);
		wall_y++;
		i++;
	}
}

void	draw_a_ray(t_game *game, float ray_angle, int ray_iteration)
{
	float	delta_x;
	float	delta_y;
	float	prev_x;
	float	prev_y;
	float	ray_x;
	float	ray_y;
	//float	dist;

	delta_x = cos(deg_to_rad(ray_angle));
	delta_y = sin(deg_to_rad(ray_angle));
	prev_x = game->player_x;
	prev_y = game->player_y;
	ray_x = game->player_x + (delta_x * STEP);
	ray_y = game->player_y + (delta_y * STEP);
	while (!is_a_block(game, ray_x, ray_y))
	{
		prev_x = ray_x;
		prev_y = ray_y;
		ray_x += delta_x * STEP;
		ray_y += delta_y * STEP;
	}
	if ((int)prev_x > (int)ray_x)
	{
		draw_texture(EAST,
			pitagoras(ray_x - game->player_x, ray_y - game->player_y),
			64 - ((ray_y - (int)ray_y) * 64), ray_iteration, game);
	}
	else if ((int)prev_y > (int)ray_y)
	{
		draw_texture(SOUTH,
			pitagoras(ray_x - game->player_x, ray_y - game->player_y),
			(ray_x - (int)ray_x) * 64, ray_iteration, game);
	}
	else if ((int)prev_x < (int)ray_x)
	{
		draw_texture(WEST,
			pitagoras(ray_x - game->player_x, ray_y - game->player_y),
			(ray_y - (int)ray_y) * 64, ray_iteration, game);
	}
	else
	{
		draw_texture(NORTH,
			pitagoras(ray_x - game->player_x, ray_y - game->player_y),
			64 - ((ray_x - (int)ray_x) * 64), ray_iteration, game);
	}
}

void	draw_rays(t_game *game)
{
	float	ray_angle;
	int		ray_iteration;

	ray_angle = -FOV / 2;
	ray_iteration = 0;
	while (ray_iteration < WIDTH)
	{
		draw_a_ray(game, ray_angle + game->player->angle, ray_iteration);
		ray_iteration++;
		ray_angle += (float)FOV / (float)WIDTH;
	}
}
