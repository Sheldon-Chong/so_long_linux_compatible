/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 18:47:16 by shechong          #+#    #+#             */
/*   Updated: 2023/12/07 10:40:02 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graphics.h"

void	enemy_move(t_enemy *enemy, t_xy pos, t_world *world)
{
	if (pos.x < world->dimensions.x
		&& pos.y < world->dimensions.y
		&& world->tgrid[pos.y][pos.x].type != 'S'
		&& world->tgrid[pos.y][pos.x].type != '1'
		&& world->tgrid[pos.y][pos.x].type != 'E')
	{
		world->tgrid[enemy->pos.y][enemy->pos.x].type = '0';
		world->tgrid[enemy->pos.y][enemy->pos.x].data = NULL;
		enemy->pos = pos;
		world->tgrid[pos.y][pos.x].type = 'S';
		world->tgrid[pos.y][pos.x].data = enemy;
	}	
}

int	decide_enemy_movement(t_world *world, t_enemy *enemy)
{
	if (world->player->pos.x - enemy->pos.x > 0)
		enemy_move(enemy, (t_xy){enemy->pos.x + 1, enemy->pos.y}, world);
	if (world->player->pos.x - enemy->pos.x < 0)
		enemy_move(enemy, (t_xy){enemy->pos.x - 1, enemy->pos.y}, world);
	if (world->player->pos.y - enemy->pos.y > 0)
		enemy_move(enemy, (t_xy){enemy->pos.x, enemy->pos.y + 1}, world);
	if (world->player->pos.y - enemy->pos.y < 0)
		enemy_move(enemy, (t_xy){enemy->pos.x, enemy->pos.y - 1}, world);
	return (1);
}

int	enemy_search4player(t_world *world, t_enemy *enemy)
{
	if (enemy->alert)
	{
		enemy->final_angle = vec2_to_angle(enemy->pos, world->player->pos);
		enemy->current_angle = vec2_to_angle(enemy->pos, world->player->pos);
	}
	if (ray_cast(world, enemy->pos, enemy->current_angle, 10) == 1
		|| ray_cast(world, enemy->pos, enemy->current_angle + 20, 10) == 1
		|| ray_cast(world, enemy->pos, enemy->current_angle - 20, 10) == 1)
	{
		enemy->player_found = 1;
		enemy->final_angle = enemy->current_angle;
		enemy->alert = 100;
	}
	else
		enemy->alert = 0;
	return (1);
}

void	enemy_track(t_world *world, t_display *display, t_enemy *enemy)
{
	if (enemy->pos.x == world->player->pos.x
		&& enemy->pos.y == world->player->pos.y)
		endgame(world, display);
	enemy->current_angle = enemy->current_angle
		+ (enemy->final_angle - enemy->current_angle) / 10;
	enemy->player_found = 0;
	enemy_search4player(world, enemy);
	if (ran_int(1, 100) == 1 && enemy->player_found == 0)
		enemy->final_angle += ran_int(-180, 180);
	if (enemy->player_found == 1
		&& (((t_timer *)(enemy->timers->data))->elapsed == 0))
		decide_enemy_movement(world, enemy);
	enemy->alert -= (enemy->alert > 0);
}

void	update_enemies(t_world *world, t_display *display)
{
	t_enemy		*enemy;
	t_object	*head;
	t_img		*char_array;

	char_array = empty_img(display->mlx, 100, 60);
	head = world->enemies;
	while (head)
	{
		enemy = (t_enemy *)(head->data);
		enemy_track(world, display, enemy);
		draw_fov(enemy, display, char_array);
		head = head->next;
		((t_timer *)(enemy->timers->data))->elapsed
			= (((t_timer *)(enemy->timers->data))->elapsed + 1) % 70;
	}
	free(char_array);
}
