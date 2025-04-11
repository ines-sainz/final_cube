/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isainz-r <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 12:39:56 by isainz-r          #+#    #+#             */
/*   Updated: 2025/04/10 12:39:58 by isainz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cube3d.h"

float	deg_to_rad(float angle)
{
	return (angle * PI / 180.0);
}

float	fix_ang(float angle)
{
	if (angle > 359)
		angle -= 360;
	if (angle < 0)
		angle += 360;
	return (angle);
}

void	clear_image(t_game *game)
{
	int	y;
	int	x;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			put_pixel(x, y, 0, game);
			x++;
		}
		y++;
	}
}

void	move_player(t_game *game)
{
	float	speed;
	int		rotation_speed;

	speed = 0.2;
	rotation_speed = 4;
	if (game->player->right_rotate)
		game->player->angle += rotation_speed;
	if (game->player->left_rotate)
		game->player->angle -= rotation_speed;
	game->player->angle = fix_ang(game->player->angle);
	game->player->delta_x = cos(deg_to_rad(game->player->angle));
	game->player->delta_y = sin(deg_to_rad(game->player->angle));
	if (game->player->key_up)
	{
		// if (game->player_y <= 0)
		// {
		// 	printf("player cant up\n");
		// 	return ;
		// }
		game->player_x += game->player->delta_x * speed;
		game->player_y += game->player->delta_y * speed;
	}
	if (game->player->key_down)
	{
		// if (game->player_y >= game->map_height)
		// {
		// 	printf("player cant down\n");
		// 	return ;
		// }
		game->player_x -= game->player->delta_x * speed;
		game->player_y -= game->player->delta_y * speed;
	}
	if (game->player->key_left)
	{
		// if (game->player_x <= 0)
		// {
		// 	printf("player cant left\n");
		// 	return ;
		// }
		game->player_x += game->player->delta_y * speed;
		game->player_y -= game->player->delta_x * speed;
	}
	if (game->player->key_right)
	{
		// if (game->player_y >= game->map_width)
		// {
		// 	printf("player cant right\n");
		// 	return ;
		// }
		game->player_x -= game->player->delta_y * speed;
		game->player_y += game->player->delta_x * speed;
	}
	clear_image(game);
	draw_rays(game);
	mlx_put_image_to_window(game->mlx->mlx, game->mlx->mlx_win,
		game->mlx->img, 0, 0);
}

int	draw_loop(t_game *game)
{
	move_player(game);
	//clear_image(game);
	return (0);
}
