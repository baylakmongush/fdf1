/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/22 15:52:44 by npetrell          #+#    #+#             */
/*   Updated: 2019/12/02 19:15:48 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H
# include "libft/includes/libft.h"
# include <fcntl.h>
# include <mlx.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>

typedef struct	fdf_l
{
	int			**map;
	int			width;
	int			height;
	void		*mlx_ptr;
	void		*window;
	void		*window1;
	int			color;
	int			move_x;
	int			move_y;
	int			zoom;
}				fdf_t;

void			check_valid(char *file_name);
void			draw_line(int x1, int y1, int x2, int y2, fdf_t *map_struct);
void			draw_map(fdf_t *map_struct);
void			ft_createmap(fdf_t **map_struct, char *file);
int				key_press(int key_code, fdf_t *map);
int				mouse_press(int button, int x, int y, fdf_t *map);

#endif
