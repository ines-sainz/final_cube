/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sergio <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 05:26:24 by sergio            #+#    #+#             */
/*   Updated: 2025/02/13 05:26:27 by sergio           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/game.h"

float deg_to_rad(int a)
{
    return (a * PI / 180.0);
}

float fix_ang(float a)
{
     if(a > 359)
        a -= 360;
     if(a < 0)
        a += 360;
    return (a);
}

float distance(float x, float y)
{
    return sqrt(x * x + y * y);
}

float fix_dist(float ra, float dist, t_game *game)
{
    float   fixed_dist;
    int     ca;

    ca = fix_ang(game->player.angle - ra);
    fixed_dist = dist * cos(deg_to_rad(ca));
    return (fixed_dist);
}

void put_pixel(int x, int y, int color, t_game *game)
{
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        return;
    int index = (y * game->size_line) + (x * (game->bpp / 8));

    if (index < 0 || index >= (HEIGHT * game->size_line))
        return;
    game->data[index] = color & 0xFF;
    game->data[index + 1] = (color >> 8) & 0xFF;
    game->data[index + 2] = (color >> 16) & 0xFF;
}

void draw_square(int x, int y, int size, int color, t_game *game)
{
    int i;

    i = -1;
    while(++i < size)
        put_pixel(x + i, y, color, game);
    i = -1;
    while(++i < size)
        put_pixel(x, y + i, color, game);
    i = -1;
    while(++i < size)
        put_pixel(x + size, y + i, color, game);
    i = -1;
    while(++i < size)
        put_pixel(x + i, y + size, color, game);
    i = -1;
}

void draw_map(t_game *game)
{
    char    **map;
    int     color;
    int     y;
    int     x;

    map = game->map;
    color = 0x0000FF;
    y = 0;
    while(map[y])
    {
        x = 0;
        while (map[y][x])
        {
            if(map[y][x] == '1' || map[y][x] == ' ')
                draw_square(x * BLOCK, y * BLOCK, BLOCK, color, game);
            x++;
        }
        y++;
    }
}

void clear_image(t_game *game)
{
    int y;
    int x;

    y = 0;
    while(y < HEIGHT)
    {
        x = 0;
        while(x < WIDTH)
        {
            put_pixel(x, y, 0, game);
            x++;
        }
        y++;
    }    
}

int touch(float rx, float ry, t_game *game)
{
    int map_x, map_y;

    map_x = (int)(rx / BLOCK);
    map_y = (int)(ry / BLOCK);

    if (map_x < 0 || map_x >= MAP_WIDTH || map_y < 0 || map_y >= MAP_HEIGHT)
        return 1;

    if (game->map[map_y] == NULL || game->map[map_y][map_x] == '\0')
        return 1;

    return (game->map[map_y][map_x] == '1');
}

char **get_map(void)
{
    char *raw_map[] = {
        "1111111111111111",
        "1000000000000001",
        "1000100000000001",
        "1000000000100001",
        "1000100000000001",
        "1000000000000001",
        "1000100000000001",
        "1010000000000001",
        "1000000000000001",
        "1000000000000001",
        "1000000000000001",
        "1000000011000001",
        "1000000000100001",
        "1000100000000001",
        "1000000000000001",
        "1111111111111111",
        NULL
    };
    int rows = 16;
    char **map = malloc(sizeof(char *) * (rows + 1));
    if (!map)
        return NULL;
    for (int i = 0; i < rows; i++)
    {
        map[i] = strdup(raw_map[i]);
        if (!map[i])
        {
            while (i > 0) free(map[--i]);
            free(map);
            return NULL;
        }
    }
    map[rows] = NULL;
    return map;
}

void load_textures(t_game *game)
{
	int i;
	char *texture_paths[4] = 
	{
		"textures/wall_north.xpm",
		"textures/wall_south.xpm",
		"textures/wall_east.xpm",
		"textures/wall_west.xpm"
	};
	i = 0;
	while(i < 4)
	{
		game->textures[i].img = mlx_xpm_file_to_image(game->mlx, texture_paths[i], &game->textures[i].width, &game->textures[i].height);
		if(!game->textures[i].img)
		{
			printf("Error loading texture: %s\n", texture_paths[i]);
			exit(1);
		}
		game->textures[i].data = mlx_get_data_addr(game->textures[i].img, &game->textures[i].bpp, &game->textures[i].size_line, &game->textures[i].endian);
		i++;
	}
}
void draw_vertical_line(int x, int lineH, int color, t_game *game)
{
    int y0 = (HEIGHT / 2) - (lineH / 2);
    int y1 = (HEIGHT / 2) + (lineH / 2);
    
    if (y0 < 0) y0 = 0;
    if (y1 > HEIGHT) y1 = HEIGHT;

    for (int y = y0; y < y1; y++)
    {
        put_pixel(x, y, color, game);
    }
}

/*void draw_vertical_line(int x, int lineH, int color, t_game *game)
{
   // (void)color; //no nos hace falta el color
    int y0 = (HEIGHT / 2) - (lineH / 2);
    int y1 = (HEIGHT / 2) + (lineH / 2);
    
    if (y0 < 0) y0 = 0;
    if (y1 > HEIGHT) y1 = HEIGHT;

    for (int y = y0; y < y1; y++)
    {
        put_pixel(x, y, color, game);
    }
}*/

void draw_line(int x0, int y0, int x1, int y1, int color, t_game *game)
{
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;
    
    while (x0 != x1 || y0 != y1)
    {
        put_pixel(x0, y0, color, game);
        int e2 = 2 * err;
        
        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
    put_pixel(x1, y1, color, game);
}

/*void draw_rays(t_player *player, t_game *game)
{
    int     r;
    int     dof; //distance of view
    float   ra;
    float   ry;
    float   rx;
    float   xo;
    float   yo;
    float   disV;
    float   disH;
    float   vx;
    float   vy;

    int     color;
    float   FOV = 60.0;
    float   rayStep = FOV / WIDTH;
    ra = fix_ang(player->angle - 30);//ray set back 30 degrees
    r = 0;
    while (r < WIDTH)
    {
        ra = player->angle + ((float)r * rayStep) - (float)30;

        //---Vertical--- 
        dof = 0;
        disV = 100000;
        float Tan = tan(deg_to_rad(ra));
        if(cos(deg_to_rad(ra)) > 0.001)//looking left
        {
            rx = (((int)player->x >> 6) <<6 ) + 64;
            ry = (player->x - rx) * Tan + player->y;
            xo = 64;
            yo = -xo * Tan;
        }
        else if(cos(deg_to_rad(ra)) < -0.001)//looking right
        {
            rx = (((int)player->x >> 6) << 6) -0.0001;
            ry = (player->x - rx) * Tan + player->y;
            xo = -64;
            yo = -xo * Tan;
        }
        else//looking up or down. no hit
        {
            rx = player->x;
            ry = player->y;
            dof =  WIDTH / FOV;
        }

        while (dof <  WIDTH)
        {
            printf("1 %i\n", dof);
            if (rx < 0 || ry < 0 || rx / BLOCK >= MAP_WIDTH || ry / BLOCK >= MAP_HEIGHT)
                break;
            if (touch(rx, ry, game))
            {
                dof = WIDTH;
                disV = (cos(deg_to_rad(ra)) * (rx - player->x)) - (sin(deg_to_rad(ra)) * (ry - player->y));
                color = 0xAAAAAA;
            }
            else
            {
                rx += xo;
                ry += yo;
                dof++;
            }
        }
        vx = rx;
        vy = ry;

        //---Horizontal---
        dof = 0;
        disH = 100000;
        Tan = 1.0/Tan;
        if(sin(deg_to_rad(ra)) > 0.001)//looking up
        {
            ry = (((int)player->y>>6)<<6) -0.0001;
            rx = (player->y-ry) * Tan + player->x;
            yo = -64;
            xo = -yo * Tan;
        }
        else if(sin(deg_to_rad(ra)) < -0.001)//looking down
        {
            ry = (((int)player->y >> 6) << 6)+64;
            rx = (player->y-ry) * Tan + player->x;
            yo = 64;
            xo = -yo * Tan;
        }
        else//looking straight left or right
        {
            rx = player->x;
            ry = player->y;
            dof =  WIDTH;
        }
        while (dof <  WIDTH)
        {
            printf("2 %i\n", dof);
            if (rx < 0 || ry < 0 || rx / BLOCK >= MAP_WIDTH || ry / BLOCK >= MAP_HEIGHT)
                break;
            if (touch(rx, ry, game))
            {
                dof = WIDTH;
                disH = cos(deg_to_rad(ra)) * (rx - player->x) - sin(deg_to_rad(ra)) * (ry - player->y);
                color = 0x777777;
            }
            else
            {
                rx += xo;
                ry += yo;
                dof++;
            }
        }
        if(disV<disH)//horizontal hit first
        {
            rx = vx;
            ry = vy;
            disH = disV;
        }
        //draw_line(player->x, player->y, rx, ry, 0xFF0000, game); //WAS COMENTED

        //Render on 3D
        disH = fix_dist(ra, disH, game);
        int lineH = (BLOCK * HEIGHT) / (disH);
        if (lineH > HEIGHT)
            lineH = HEIGHT;
        draw_vertical_line(r , lineH, color, game);//works (WIDTH / 60)
        r++;
    }
}*/

/*void    draw_rays(t_player *player, t_game *game)
{

    double  camerax = -directionx * 1.0; // for FOV 60 then * 0.66, for FOV 90 then * 1.0
    double  cameray = directiony * 1.0; // for FOV 60 then * 0.66, for FOV 90 then * 1.0
    double  camaraX;
    player->x = 9;
    player->y = 13;
    int i = 0;
    while (i < WIDTH)
    {
        camaraX = 2.0 * (player->x / WIDTH) - 1;
        double rayDirX = directionx + camerax * camaraX;
        double rayDirY = directiony + cameray * camaraX;
        //printf("%f, %f\n", player->x, player->y);
        int mapX = player->x; //starting grid cell
        int mapY = player->y; //starting grid cell
        //printf("player_pos x%f mx%i y%f my%i\n", player->x, mapX, player->y, mapY); //printf
        double deltaDistX = 1 / rayDirX; //delta distances
        if (deltaDistX < 0)
            deltaDistX = deltaDistX * -1;
        double deltaDistY = 1 / rayDirY; //delta distances
        if (deltaDistY < 0)
            deltaDistY = deltaDistY * -1;
        int stepX; //step direction
        int stepY; //step direction
        double sideDistX; //initial side distances
        double sideDistY; //initial side distances
        if (rayDirX >= 0)
        {
            stepX = 1;
            sideDistX = (mapX + 1.0 - player->x) * deltaDistX;
        }
        else
        {
            stepX = -1;
            sideDistX = (player->x - mapX) * deltaDistX;
        }
        if (rayDirY >= 0)
        {
            stepY = 1;
            sideDistY = (mapY + 1.0 - player->y) * deltaDistY;
        }
        else
        {
            stepY = -1;
            sideDistY = (player->y - mapY) * deltaDistY;
        }
        // actual dda
        int hitSide;
        while (1)
        {
            //printf("map y%i x%i step y%i x%i\n", mapY, mapX, stepY, stepX);
            if (sideDistX < sideDistY)
            {
                sideDistX += deltaDistX;
                mapX += stepX;
                hitSide = 0; //vertical wall
            }
            else
            {
                sideDistY += deltaDistY;
                mapY += stepY;
                hitSide = 1; //horizontal wall
            }
            //printf("aqui %i %i \n", mapY, mapX);
            if (game->map[mapY][mapX] == '1')
                break ;
        }
        float wallDist;
        if (hitSide == 0)
            wallDist = sideDistX - deltaDistX / rayDirX;
        else
            wallDist = sideDistY - deltaDistY / rayDirY;
        float lineHeight = HEIGHT / wallDist;
        double drawStart = (HEIGHT / 2) - (lineHeight / 2);
        double drawEnd = (HEIGHT / 2) + (lineHeight / 2);
        if (drawStart < 0)
            drawStart = 0;
        if (drawEnd > HEIGHT)
            drawEnd = HEIGHT;
        float wallX;
        if (hitSide == 0)
            wallX = player->y + wallDist * rayDirY;
        else
            wallX = player->x + wallDist * rayDirX;
        //printf("wallx %f %i\n", wallX, (int)wallX); //printf
        wallX = wallX - (int)wallX;
        float   textX = wallX * BLOCK;
        //printf("textX%f %i\n", textX, (int)textX);
        textX = (int)textX - 1; //empieza en 0
        if ((hitSide == 0 && stepX == -1) || (hitSide == 1 && stepY == 1))
            textX = BLOCK - textX - 1;
        float step = BLOCK / lineHeight;
    //    float textY = ((drawStart - (HEIGHT / 2) + (lineHeight / 2)) * step) - 1;
        float textY = (((drawStart - ((HEIGHT / 2) - (lineHeight / 2))) / lineHeight) * BLOCK) - 1;
        int y = drawStart;
        int texRow;
        int color;
        while (y < drawEnd)
        {
            texRow = textY;
            textY += step;
            color = 0xFFFFFF;
            //printf("pixel in x%f y%d, step %f\n", textX, texRow, step);
            put_pixel(textX, texRow, color, game);
            y++;
        }
        i++;
    }
}*/
// double  directionx = 0.0; //up=0 down=0 right=1 left=-1 //cos(deg_to_rad(3.0 * PI / 2.0)); // angle faced
// double  directiony = -1.0; //up=-1 down=1 right=0 left=0 //sin(deg_to_rad(3.0 * PI / 2.0)); // angle faced
int is_a_block(t_game *game, float ray_x, float ray_y)
{
    if (ray_x > MAP_WIDTH || ray_y > MAP_HEIGHT || ray_x < 0 || ray_y < 0)
        return (1);
    // printf("ray%f %f\n", ray_x, ray_y);
    return (game->map[(int)ray_y][(int)ray_x] == '1');
}

float   pitagoras(float horizontal_step, float vertical_step)
{
    float hipotenuse;

    hipotenuse = sqrt(horizontal_step * horizontal_step + vertical_step * vertical_step);
    return (hipotenuse);
}

float get_vertical_distance(t_player *player, t_game *game, float ray_angle)
{
    float   vertical_step;
    float   horizontal_step;
    float   ray_x;
    float   ray_y;
    float   ray_len;

    ray_x = player->x;
    ray_y = player->y;
    ray_len = 0;
    vertical_step = fabs(cos(deg_to_rad(ray_angle)) / sin(deg_to_rad(ray_angle)));
    horizontal_step = 1;
    while (ray_len < MAX_DIST_RAY)
    {
        ray_x += horizontal_step;
        ray_y += vertical_step;
        ray_len += pitagoras(horizontal_step, vertical_step);
        if (is_a_block(game, ray_x, ray_y) || is_a_block(game, ray_x - 1, ray_y))
        {
            printf("vertical is_block x%f y%f d%f# ", ray_x, ray_y, ray_len);
            return (ray_len);
        }
    }
    return (ray_len);
}

float get_horizontal_distance(t_player *player, t_game *game, float ray_angle)
{
    float   vertical_step;
    float   horizontal_step;
    float   ray_x;
    float   ray_y;
    float   ray_len;

    ray_x = player->x;
    ray_y = player->y;
    ray_len = 0;
    vertical_step = 1;
    horizontal_step = fabs(sin(deg_to_rad(ray_angle)) / cos(deg_to_rad(ray_angle)));
    while (ray_len < MAX_DIST_RAY)
    {
        ray_x += horizontal_step;
        ray_y += vertical_step;
        ray_len += pitagoras(horizontal_step, vertical_step);
        if (is_a_block(game, ray_x, ray_y) || is_a_block(game, ray_x, ray_y - 1))
        {
            printf("horizontal is_block x%f y%f d%f # ", ray_x, ray_y, ray_len);
            return (ray_len);
        }
    }
    return (ray_len);
}

void    draw_wall_lines(int wall_plane, float dist, int ray_iteration, t_game *game)
{
    int wall_height;
    int wall_y;

    wall_height = HEIGHT / dist;
    wall_y = (HEIGHT - wall_height) / 2;
    while (wall_height > 0)
    {
        if (wall_plane == IS_HORIZONTAL)
            put_pixel(ray_iteration, wall_y, 0xFFFFFF, game);
        else
            put_pixel(ray_iteration, wall_y, 0xAAAAAA, game);
        wall_y++;
        wall_height--;
    }
}

void    draw_a_ray(t_player *player, t_game *game, float ray_angle, int ray_iteration)
{
    float horizontal_dist;
    float vertical_dist;

    horizontal_dist = get_horizontal_distance(player, game, ray_angle);
    vertical_dist = get_vertical_distance(player, game, ray_angle);
    if (horizontal_dist < vertical_dist)
    {
        draw_wall_lines(IS_HORIZONTAL, horizontal_dist, ray_iteration, game);
    }
    else
    {
        draw_wall_lines(IS_VERTICAL, vertical_dist, ray_iteration, game);
    }
    
}

void    draw_rays(t_player *player, t_game *game)
{
    float   ray_angle;
    int     ray_iteration;

    ray_angle = - FOV / 2;
    ray_iteration = 0;
    while (ray_iteration < WIDTH)
    {
        printf("# ray dir %f # ", ray_angle);
        // printf("miau angle=%f %f\n", player->angle, ray_angle);
        // printf("miau delta x: %f delta y: %f\n", player->delta_x, player->delta_y);
        draw_a_ray(player, game, ray_angle + player->angle, ray_iteration);
        printf("\n");
        ray_iteration++;
        ray_angle += (float)FOV / (float)WIDTH;
    }
}

void init_game(t_game *game)
{
    init_player(&game->player);
    game->map = get_map();
    game->mlx = mlx_init();
    game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "DDA-RayCasting");
    game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
    game->data = mlx_get_data_addr(game->img, &game->bpp, &game->size_line, &game->endian);
	load_textures(game);
    mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
}

int draw_loop(t_game *game)
{
    t_player *player;

    player = &game->player;
    move_player(player);
    clear_image(game);
    if(DEBUG)
    {
        draw_square(player->x, player->y, 10, 0x00FF00, game);
        draw_map(game);
    }
    printf("\n\n\nplayer angle %f, player pos x: %f y: %f\n\n", player->angle, player->x, player->y);
    draw_rays(player, game);

    mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
	return (0);
}

int main(void)
{
    t_game game;

    init_game(&game);
    mlx_hook(game.win, 2, 1L<<0, key_press, &game.player);
    mlx_hook(game.win, 3, 1L<<1, key_release, &game.player);
    game.player.x = 2.0;
    game.player.y = 1.0;
    mlx_loop_hook(game.mlx, draw_loop, &game);
    mlx_loop(game.mlx);

    return (0);
}
