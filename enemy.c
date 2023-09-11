#include "graphics.h"

void	move_enemy(t_enemy *enemy, t_xy pos, t_world *world)
{
	if (pos.x < world->dimensions.x && pos.y < world->dimensions.y
		&& world->tgrid[pos.y][pos.x].type != 'S'
		&& world->tgrid[pos.y][pos.x].type != '1' )
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
		move_enemy(enemy, (t_xy){enemy->pos.x + 1, enemy->pos.y}, world);
	if (world->player->pos.x - enemy->pos.x < 0)
		move_enemy(enemy, (t_xy){enemy->pos.x - 1, enemy->pos.y}, world);
	if (world->player->pos.y - enemy->pos.y > 0)
		move_enemy(enemy, (t_xy){enemy->pos.x, enemy->pos.y + 1}, world);
	if (world->player->pos.y - enemy->pos.y < 0)
		move_enemy(enemy, (t_xy){enemy->pos.x, enemy->pos.y - 1}, world);
	return (1);
}

int	enemy_search4player(t_world *world, t_enemy *enemy)
{
	if (enemy->alert > 0)
	{
		enemy->final_angle = pts2angle(enemy->pos, world->player->pos);
		enemy->c_ang = pts2angle(enemy->pos, world->player->pos);
	}
	if (ray_cast(world, enemy->pos, enemy->c_ang, 10) == 1
		|| ray_cast(world, enemy->pos, enemy->c_ang + 20, 10) == 1
		|| ray_cast(world, enemy->pos, enemy->c_ang - 20, 10) == 1)
	{
		enemy->player_found = 1;
		enemy->final_angle = enemy->c_ang;
		enemy->alert = 100;
	}
	else
		enemy->alert = 0;
	return (1);
}

void	update_enemy(t_world *world, t_display *display, t_grid_d grid)
{
	t_enemy		*enemy;
	t_object	*head;
	t_data		*char_array;

	char_array = new_img(display->mlx, 100, 60);
	head = world->enemies;
	while (head)
	{
		enemy = (t_enemy *)(head->data);
		if (enemy->pos.x == world->player->pos.x
			&& enemy->pos.y == world->player->pos.y)
			endgame(world, display);
		enemy->c_ang = enemy->c_ang + (enemy->final_angle - enemy->c_ang) / 10;
		draw_fov(enemy, display, grid, char_array);
		enemy->player_found = 0;
		enemy_search4player(world, enemy);
		if (ran_int(1, 100) == 1 && enemy->player_found == 0)
			enemy->final_angle += ran_int(-180, 180);
		if (enemy->player_found == 1
			&& (((t_timer *)(enemy->timers->data))->elapsed == 0))
			decide_enemy_movement(world, enemy);
		enemy->alert -= (enemy->alert > 0);
		head = head->next;
		((t_timer *)(enemy->timers->data))->elapsed ++;
		if (((t_timer *)(enemy->timers->data))->elapsed > 70)
			((t_timer *)(enemy->timers->data))->elapsed = 0;
	}
	free(char_array);
}

int	update_animations(t_display *display, t_world *world)
{
	t_object	*head;
	t_animator	*animator;

	head = display->animations;
	while (head)
	{
		animator = ((t_animator *)(head->data));
		animator->frame_timer = (animator->frame_timer + 1) % animator->speed;
		if (animator->frame_timer == 0)
			animator->current_frame = (animator->current_frame + 1) % 2;
		update_enemy(world, display, (t_grid_d){21, 21, ((display->width) / 2)
			- display->camera->pos.x, ((display->height) / 2)
			- display->camera->pos.y});
		head = head->next;
	}
	return (1);
}