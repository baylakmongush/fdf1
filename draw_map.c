/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/02 17:30:52 by npetrell          #+#    #+#             */
/*   Updated: 2019/12/02 18:08:04 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

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

    z1 = (map_struct->map[y1 / map_struct->zoom][x1 / map_struct->zoom]) * map_struct->zoom;
    z2 = map_struct->map[y2 / map_struct->zoom][x2 / map_struct->zoom] * map_struct->zoom;
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
                drawLine(i * map_struct->zoom, j * map_struct->zoom,
                (i + 1) * map_struct->zoom, j * map_struct->zoom, map_struct);
            if (j < map_struct->height - 1)
                drawLine(i * map_struct->zoom, j * map_struct->zoom,
                i * map_struct->zoom, (j + 1) * map_struct->zoom, map_struct);
            i++;
        }
        j++;
    }
}