/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/22 15:52:44 by npetrell          #+#    #+#             */
/*   Updated: 2019/12/01 22:30:55 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include <stdio.h>
#define IMJ "./*jpeg"
// gcc -I minilibx -L minilibx -lmlx -framework OpenGL -framework Appkit main.c libft/libft.a
// gcc -I minilibx_macos minilibx_macos/libmlx.a -framework OpenGL -framework Appkit main.c libft/libft.a

void     iso(int *x, int *y, int z)
{
    int previous_x;
    int previous_y;

    previous_x = *x;
    previous_y = *y;
    *x = (previous_x - previous_y) * cos(0.5);
    *y = -z + (previous_x + previous_y) * sin(0.5);
}

void        drawLine(int x1, int y1, int x2, int y2, fdf_t *map_struct)
{
    int     deltaX;
    int     deltaY;
    int     signX;
    int     signY;
    int     error;
    int     error2;
    int     z1;
    int     z2;
    int     max;

    z1 = map_struct->map[y1 / map_struct->zoom][x1 / map_struct->zoom];
    z2 = map_struct->map[y2 / map_struct->zoom][x2 / map_struct->zoom];
    map_struct->color = (z1 || z2) ? 0x800080 : 0xffffff;
    iso(&x1, &y1, z1);
    iso(&x2, &y2, z2);
    x1 += map_struct->move_x;
    y1 += map_struct->move_y;
    x2 += map_struct->move_x;
    y2 += map_struct->move_y;
    deltaX = abs(x2 - x1);
    deltaY = abs(y2 - y1);
    signX = x1 < x2 ? 1 : -1;
    signY = y1 < y2 ? 1 : -1;
    error = deltaX - deltaY;
    mlx_pixel_put(map_struct->mlx_ptr, map_struct->window, x2, y2, map_struct->color);
    while ((x1 - x2) || (y1 - y2))
    {
        mlx_pixel_put(map_struct->mlx_ptr, map_struct->window, x1, y1, map_struct->color);
        error2 = error * 2;
        if (error2 > -deltaY)
        {
            error -= deltaY;
            x1 += signX;
        }
        if (error2 < deltaX) 
        {
            error += deltaX;
            y1 += signY;
        }
    }
}

void        draw_map(fdf_t *map_struct)
{
    int     i;
    int     j;
    int     z1;
    int     z2;

    j = 0;
    while (j < map_struct->height)
    {
        i = 0;
        while (i < map_struct->width)
        {
            if (i < map_struct->width - 1)
                drawLine(i * map_struct->zoom, j * map_struct->zoom, (i + 1) * map_struct->zoom, j * map_struct->zoom, map_struct);
            if (j < map_struct->height - 1)
                drawLine(i * map_struct->zoom, j * map_struct->zoom, i * map_struct->zoom, (j + 1) * map_struct->zoom, map_struct);
            i++;
        }
        j++;
    }
}

void       ft_makestruct(fdf_t **map_struct, int **map, int width, int height)
{
    *map_struct = (fdf_t*)malloc(sizeof(fdf_t));
    (*map_struct)->height = height;
    (*map_struct)->map = map;
    (*map_struct)->width = width;
}

int         *ft_splitstring(char *line, int min_count)
{
    int     *str_int;
    int     i;
    int     j;

    str_int = (int*)malloc(sizeof(int) * min_count);
    i = -1;
    j = -1;
    while (line[++i] && j < min_count)
    {
        if (line[i] != 32)
        {
            if ((line[i - 1] >= '0' && line[i - 1] <= '9') ||
(line[i - 1] >= 'a' && line[i - 1] <= 'z')
|| (line[i - 1] >= 'A' && line[i - 1] <= 'Z') || (line[i - 1] == ','))
                i++;
            else
                str_int[++j] = ft_atoi(&line[i]);
        }
    }
    return (str_int);    
}

int         ft_count_space(char *line)
{
    int     space;
    int     i;

    i = 0;
    space = 0;
    while (line[i])
    {
        if (line[i] == 32)
            space++;
        i++;
    }
    return (space);
}

void        ft_createmap(fdf_t **map_struct, char *file)
{
    int     fd;
    int     **map;
    char    *line;
    char    *line1;
    int     str;
    int     min_count;
    int     count;
    int     space;
    int     i;

    str = 0;
    fd = open(file, O_RDONLY);
	while (get_next_line(fd, &line) > 0)
    {
        str++;
        space = ft_count_space(line);
        count = ft_strlen(line) - space;
        if (str == 1)
            min_count = count;
        if (count < min_count)
            min_count = count;
        free(line);  
    }
    close(fd);
    map = (int**)malloc(sizeof(int*) * str);
    fd = open(file, O_RDONLY);
    i = 0;
	while (get_next_line(fd, &line1) > 0)
    {
        map[i] = ft_splitstring(line1, min_count);
        i++;
        free(line1);
    }
    close(fd);
    ft_makestruct(map_struct, map, min_count, str);
}

int     key_press(int key_code, fdf_t *map)
{
    if (key_code == 125 || key_code == 1)
        map->move_y += 20;
    if (key_code == 126 || key_code == 13)
        map->move_y -= 20;
    if (key_code == 123 || key_code == 0)
        map->move_x -= 20;
    if (key_code == 124 || key_code == 2)
        map->move_x += 20;
    if (key_code == 53)
        exit(0);
    if (key_code == 69)
        map->zoom += 10;
    if (key_code == 78 && map->zoom > 0)
        map->zoom -= 10;
    if (key_code == 4)
    {
        map->window1 = mlx_new_window(map->mlx_ptr, 800, 200, "HELP");
        mlx_string_put(map->mlx_ptr, map->window1, 0, 0, 0xccffff, "1. PRESS buttons 'UP', 'LEFT',");
        mlx_string_put(map->mlx_ptr, map->window1, 310, 0, 0xccccff, "'RIGHT', 'DOWN', if you want to move map,");
        mlx_string_put(map->mlx_ptr, map->window1, 0, 40, 0xcc99ff, "or 'W', 'A', 'D', 'S'.");
        mlx_string_put(map->mlx_ptr, map->window1, 0, 80, 0xcc66ff, "2. PRESS buttons '+', '-', ");
        mlx_string_put(map->mlx_ptr, map->window1, 270, 80, 0xcc33ff, "if you want to zoom up or down map.");
        mlx_string_put(map->mlx_ptr, map->window1, 0, 120, 0xcc00ff, "3. PRESS 'ESC', ");
        mlx_string_put(map->mlx_ptr, map->window1, 160, 120, 0x9933cc, "if you want to exit.");
    }
    draw_map(map);
    mlx_clear_window(map->mlx_ptr, map->window);
    mlx_string_put(map->mlx_ptr, map->window, 10, 10, 0xfff000, "HELP");
    mlx_string_put(map->mlx_ptr, map->window, 60, 10, 0xfff000, "(Press 'H')");
    return (0);
}

int mouse_press(int button, int x, int y, fdf_t *map)
{
    if (button == 1 && x > 10 && x < 30 && y > 10 && y < 30)
        map->window1 = mlx_new_window(map->mlx_ptr, 500, 500, "FDF");
    return (0);
}

int			 main(int argc, char **argv)
{
    fdf_t   *map_struct;
    int     x0;
    int     y0;
    int     x1;
    int     y1;
    int     color;

	if (argc == 2)
	{
        ft_createmap(&map_struct, argv[1]);
        map_struct->mlx_ptr = mlx_init();
        map_struct->window = mlx_new_window(map_struct->mlx_ptr, 1000, 1000, "FDF");
        map_struct->move_y = 200;
        map_struct->move_x = 200;
        map_struct->zoom = 1;
        draw_map(map_struct);
        mlx_key_hook(map_struct->window, key_press, map_struct);
        mlx_string_put(map_struct->mlx_ptr, map_struct->window, 10, 10, 0xfff00f, "HELP");
        mlx_string_put(map_struct->mlx_ptr, map_struct->window, 60, 10, 0xfff000, "(Press 'H')");
   //     mlx_string_put(map_struct->mlx_ptr, map_struct->window1, 10, 10, 0xfff00f, "HELP");
    //    mlx_mouse_hook(map_struct->window, mouse_press, map_struct);
     //   mlx_key_hook(map_struct->window1, key_press1, map_struct);
   //     mlx_string_put(map_struct->mlx_ptr, map_struct->window1, 10, 10, 0xfff00f, "HELP");
        mlx_loop(map_struct->mlx_ptr);
    }
    return (0);
}
