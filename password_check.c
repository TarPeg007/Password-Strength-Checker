/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   password_check.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfellahi <sfellahi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 21:51:51 by sfellahi          #+#    #+#             */
/*   Updated: 2025/01/03 21:52:02 by sfellahi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <mlx.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

# define WINDOW_WIDTH 800
# define WINDOW_HEIGHT 800
# define MIN_LENGTH 8
# define MAX_INPUT 100

# define COLOR_BG 0x001E1E1E
# define COLOR_TEXT 0x00FFFFFF
# define COLOR_WEAK 0x00FF4444
# define COLOR_MODERATE 0x00FFAA44
# define COLOR_STRONG 0x0044FF44
# define COLOR_INPUT_BG 0x002D2D2D

typedef struct s_data
{
	void	*mlx;
	void	*win;
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	char	input_buffer[MAX_INPUT];
	int		input_pos;
	int		analyzing;
}	t_data;

typedef struct s_rect
{
	int	width;
	int	height;
	int	color;
}	t_rect;

typedef struct s_metrics
{
	int	length;
	int	upper;
	int	lower;
	int	digits;
	int	special;
	int	score;
}	t_metrics;

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	if (x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT)
	{
		dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
		*(unsigned int *)dst = color;
	}
}

void	draw_rectangle(t_data *data, int x, int y, t_rect rect)
{
	int	i;
	int	j;

	i = y;
	while (i < y + rect.height)
	{
		j = x;
		while (j < x + rect.width)
		{
			my_mlx_pixel_put(data, j, i, rect.color);
			j++;
		}
		i++;
	}
}

int	check_length(const char *password)
{
	return (strlen(password) >= MIN_LENGTH);
}

int	count_uppercase(const char *password)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (password[i] != '\0')
	{
		if (isupper(password[i]))
			count++;
		i++;
	}
	return (count);
}

int	count_lowercase(const char *password)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (password[i] != '\0')
	{
		if (islower(password[i]))
			count++;
		i++;
	}
	return (count);
}

int	count_digits(const char *password)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (password[i] != '\0')
	{
		if (isdigit(password[i]))
			count++;
		i++;
	}
	return (count);
}

int	count_special(const char *password)
{
	int		i;
	int		count;
	char	*special_chars;

	i = 0;
	count = 0;
	special_chars = "!@#$%^&*()-_=+[]{}|;:'\",.<>?/`~";
	while (password[i] != '\0')
	{
		if (strchr(special_chars, password[i]))
			count++;
		i++;
	}
	return (count);
}

void	draw_strength_indicators(t_data *data, int score, int y)
{
	if (score == 5)
		mlx_string_put(data->mlx, data->win, 200, y + 200,
			COLOR_STRONG, "STRONG");
	else if (score >= 3)
		mlx_string_put(data->mlx, data->win, 200, y + 200,
			COLOR_MODERATE, "MODERATE");
	else
		mlx_string_put(data->mlx, data->win, 200, y + 200,
			COLOR_WEAK, "WEAK");
}

void	draw_analysis_metrics(t_data *data, t_metrics metrics, int y)
{
	char	buf[100];
	int		color;

	color = metrics.length ? COLOR_STRONG : COLOR_WEAK;
	sprintf(buf, "Length: %s", metrics.length ? "Sufficient" : "Too short");
	mlx_string_put(data->mlx, data->win, 200, y, color, buf);
	color = metrics.upper ? COLOR_STRONG : COLOR_WEAK;
	sprintf(buf, "Uppercase letters: %d", metrics.upper);
	mlx_string_put(data->mlx, data->win, 200, y + 30, color, buf);
	color = metrics.lower ? COLOR_STRONG : COLOR_WEAK;
	sprintf(buf, "Lowercase letters: %d", metrics.lower);
	mlx_string_put(data->mlx, data->win, 200, y + 60, color, buf);
	color = metrics.digits ? COLOR_STRONG : COLOR_WEAK;
	sprintf(buf, "Digits: %d", metrics.digits);
	mlx_string_put(data->mlx, data->win, 200, y + 90, color, buf);
	color = metrics.special ? COLOR_STRONG : COLOR_WEAK;
	sprintf(buf, "Special characters: %d", metrics.special);
	mlx_string_put(data->mlx, data->win, 200, y + 120, color, buf);
}

void	draw_password_analysis(t_data *data)
{
	t_metrics	metrics;
	char		dots[MAX_INPUT + 1];
	int			y;

	metrics.length = check_length(data->input_buffer);
	metrics.upper = count_uppercase(data->input_buffer);
	metrics.lower = count_lowercase(data->input_buffer);
	metrics.digits = count_digits(data->input_buffer);
	metrics.special = count_special(data->input_buffer);
	metrics.score = metrics.length + (metrics.upper > 0) + (metrics.lower > 0)
		+ (metrics.digits > 0) + (metrics.special > 0);
	draw_rectangle(data, 0, 0, (t_rect){WINDOW_WIDTH, WINDOW_HEIGHT, COLOR_BG});
	draw_rectangle(data, 200, 100, (t_rect){400, 40, COLOR_INPUT_BG});
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	mlx_string_put(data->mlx, data->win, 300, 50, COLOR_TEXT,
		"Password Strength Checker");
	memset(dots, '*', data->input_pos);
	dots[data->input_pos] = '\0';
	mlx_string_put(data->mlx, data->win, 210, 120, COLOR_TEXT, dots);
	y = 200;
	draw_analysis_metrics(data, metrics, y);
	mlx_string_put(data->mlx, data->win, 200, y + 170, COLOR_TEXT,
		"Password Strength:");
	draw_strength_indicators(data, metrics.score, y);
}

int	key_press(int keycode, t_data *data)
{
	if (keycode == 53)
	{
		mlx_destroy_window(data->mlx, data->win);
		exit(0);
	}
	else if ((keycode >= 32 && keycode <= 126) && data->input_pos < MAX_INPUT - 1)
	{
		data->input_buffer[data->input_pos++] = keycode;
		data->input_buffer[data->input_pos] = '\0';
		draw_password_analysis(data);
	}
	else if (keycode == 51 && data->input_pos > 0)
	{
		data->input_buffer[--data->input_pos] = '\0';
		draw_password_analysis(data);
	}
	return (0);
}

int	main(void)
{
	t_data	data;

	data.mlx = mlx_init();
	if (!data.mlx)
		return (1);
	data.win = mlx_new_window(data.mlx, WINDOW_WIDTH, WINDOW_HEIGHT,
			"Password Strength Checker");
	if (!data.win)
		return (1);
	data.img = mlx_new_image(data.mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!data.img)
		return (1);
	data.addr = mlx_get_data_addr(data.img, &data.bits_per_pixel,
			&data.line_length, &data.endian);
	data.input_pos = 0;
	data.input_buffer[0] = '\0';
	draw_password_analysis(&data);
	mlx_hook(data.win, 2, 1L << 0, key_press, &data);
	mlx_loop(data.mlx);
	return (0);
}