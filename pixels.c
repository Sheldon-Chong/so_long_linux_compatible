/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixels.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 18:46:29 by shechong          #+#    #+#             */
/*   Updated: 2023/12/07 10:40:02 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graphics.h"

void	draw_rect(t_img *img, t_xy dimensions, t_xy pos, int color)
{
	int	x;
	int	y;

	x = -1;
	while (++x < dimensions.x)
	{
		y = -1;
		while (++y < dimensions.y)
			put_pixel(img, x + pos.x, y + pos.y, color);
	}
}

void	put_pixel(t_img *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

void	draw_line(t_img *img, t_xy start, t_xy end, int color)
{
	int		steps;
	int		i2;
	float	x;
	float	y;

	steps = abs(end.y - start.y);
	if (abs(end.x - start.x) > abs(end.y - start.y))
		steps = abs(end.x - start.x);
	x = start.x;
	y = start.y;
	i2 = -1;
	while (++i2 <= steps)
	{
		put_pixel(img, (int)x, (int)y, color);
		x += (float)(end.x - start.x) / steps;
		y += (float)(end.y - start.y) / steps;
	}
}
