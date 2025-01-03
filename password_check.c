#include <mlx.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define MIN_LENGTH 8
#define MAX_INPUT 100

typedef struct {
    void    *mlx;
    void    *win;
    void    *img;
    char    *addr;
    int     bits_per_pixel;
    int     line_length;
    int     endian;
    char    input_buffer[MAX_INPUT];
    int     input_pos;
    int     analyzing;
} t_data;

// Color definitions
#define COLOR_BG 0x001E1E1E
#define COLOR_TEXT 0x00FFFFFF
#define COLOR_WEAK 0x00FF4444
#define COLOR_MODERATE 0x00FFAA44
#define COLOR_STRONG 0x0044FF44
#define COLOR_INPUT_BG 0x002D2D2D

// Function to put pixel in image
void my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
    char    *dst;

    if (x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT)
    {
        dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
        *(unsigned int*)dst = color;
    }
}

// Function to draw filled rectangle
void draw_rectangle(t_data *data, int x, int y, int width, int height, int color)
{
    for (int i = y; i < y + height; i++)
    {
        for (int j = x; j < x + width; j++)
        {
            my_mlx_pixel_put(data, j, i, color);
        }
    }
}

// Password checking functions
int check_length(const char *password)
{
    return strlen(password) >= MIN_LENGTH;
}

int count_uppercase(const char *password) {
    int count = 0;
    for (int i = 0; password[i] != '\0'; i++)
        if (isupper(password[i])) count++;
    return count;
}

int count_lowercase(const char *password) {
    int count = 0;
    for (int i = 0; password[i] != '\0'; i++)
        if (islower(password[i])) count++;
    return count;
}

int count_digits(const char *password) {
    int count = 0;
    for (int i = 0; password[i] != '\0'; i++)
        if (isdigit(password[i])) count++;
    return count;
}

int count_special(const char *password) {
    int count = 0;
    for (int i = 0; password[i] != '\0'; i++)
        if (strchr("!@#$%^&*()-_=+[]{}|;:'\",.<>?/`~", password[i])) count++;
    return count;
}

void draw_password_analysis(t_data *data)
{
    int length = check_length(data->input_buffer);
    int upper = count_uppercase(data->input_buffer);
    int lower = count_lowercase(data->input_buffer);
    int digits = count_digits(data->input_buffer);
    int special = count_special(data->input_buffer);
    int score = length + (upper > 0) + (lower > 0) + (digits > 0) + (special > 0);
    
    // Clear previous content
    draw_rectangle(data, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, COLOR_BG);
    
    // Draw password field background
    draw_rectangle(data, 200, 100, 400, 40, COLOR_INPUT_BG);
    
    // Update the window with the image
    mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
    
    // Draw text elements
    mlx_string_put(data->mlx, data->win, 300, 50, COLOR_TEXT, "Password Strength Checker");
    
    // Draw password dots
    char dots[MAX_INPUT + 1];
    memset(dots, '*', data->input_pos);
    dots[data->input_pos] = '\0';
    mlx_string_put(data->mlx, data->win, 210, 120, COLOR_TEXT, dots);
    
    // Draw analysis results
    char buf[100];
    int y = 200;
    int color;
    
    sprintf(buf, "Length: %s", length ? "Sufficient" : "Too short");
    color = length ? COLOR_STRONG : COLOR_WEAK;
    mlx_string_put(data->mlx, data->win, 200, y, color, buf);
    
    sprintf(buf, "Uppercase letters: %d", upper);
    color = upper ? COLOR_STRONG : COLOR_WEAK;
    mlx_string_put(data->mlx, data->win, 200, y + 30, color, buf);
    
    sprintf(buf, "Lowercase letters: %d", lower);
    color = lower ? COLOR_STRONG : COLOR_WEAK;
    mlx_string_put(data->mlx, data->win, 200, y + 60, color, buf);
    
    sprintf(buf, "Digits: %d", digits);
    color = digits ? COLOR_STRONG : COLOR_WEAK;
    mlx_string_put(data->mlx, data->win, 200, y + 90, color, buf);
    
    sprintf(buf, "Special characters: %d", special);
    color = special ? COLOR_STRONG : COLOR_WEAK;
    mlx_string_put(data->mlx, data->win, 200, y + 120, color, buf);
    
    mlx_string_put(data->mlx, data->win, 200, y + 170, COLOR_TEXT, "Password Strength:");
    if (score == 5)
        mlx_string_put(data->mlx, data->win, 200, y + 200, COLOR_STRONG, "STRONG");
    else if (score >= 3)
        mlx_string_put(data->mlx, data->win, 200, y + 200, COLOR_MODERATE, "MODERATE");
    else
        mlx_string_put(data->mlx, data->win, 200, y + 200, COLOR_WEAK, "WEAK");
}

int key_press(int keycode, t_data *data)
{
    if (keycode == 53)  // ESC key
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
    else if (keycode == 51 && data->input_pos > 0)  // Backspace
    {
        data->input_buffer[--data->input_pos] = '\0';
        draw_password_analysis(data);
    }
    return (0);
}

int main(void)
{
    t_data data;
    
    data.mlx = mlx_init();
    if (!data.mlx)
        return (1);
    data.win = mlx_new_window(data.mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "Password Strength Checker");
    if (!data.win)
        return (1);
        
    data.img = mlx_new_image(data.mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!data.img)
        return (1);
        
    data.addr = mlx_get_data_addr(data.img, &data.bits_per_pixel, &data.line_length, &data.endian);
    data.input_pos = 0;
    data.input_buffer[0] = '\0';
    
    // Draw initial screen
    draw_password_analysis(&data);
    
    mlx_hook(data.win, 2, 1L<<0, key_press, &data);
    mlx_loop(data.mlx);
    
    return (0);
}