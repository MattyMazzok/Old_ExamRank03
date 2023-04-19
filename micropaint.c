#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

int str_len(char *str)
{
	int i = 0;

	while (str[i])
		i++;
	return(i);
}

int str_error(char* str, int ret)
{
	write(1, str, str_len(str));
	return(ret);
}

int check_pos(float x, float y, float shape_x, float shape_y, float shape_larghezza, float shape_altezza)
{
	if(x < shape_x || x > shape_x + shape_larghezza || y < shape_y || x > shape_y + shape_altezza)
		return (0);
	else if (x - shape_x < 1.0000000 || (shape_x + shape_larghezza) - x < 1.0000000 ||
			y - shape_y < 1.0000000 || (shape_y + shape_altezza) - y < 1.0000000)
			return (1);
	return(2);
}

int main(int argc, char **argv)
{
	char *canvas;
	FILE *file;
	int	read, pos, canvas_larghezza, canvas_altezza, x, y;
	char sfondo, id, color;
	float shape_x, shape_y, shape_larghezza, shape_altezza;

	if(!(file = fopen(argv[1], "r")) ||
		!(fscanf(file, "%d %d %c\n", &canvas_larghezza, &canvas_larghezza, &sfondo) == 3) ||
		!((canvas_larghezza > 0 && canvas_larghezza <= 300) || (canvas_altezza > 0 && canvas_altezza <= 300)) ||
		!(canvas = (char *)malloc(sizeof(char), (canvas_larghezza * canvas_altezza))))
		return(str_error("Error: argument\n", 1));
	memset(canvas, sfondo, canvas_larghezza * canvas_altezza);
	while((read = fscanf(file, "%c %f %f %f %f %c", &id, &shape_x, &shape_y, &shape_larghezza, &shape_altezza, &color)) == 6)
	{
		if (!(shape_larghezza > 0 && shape_altezza > 0) || !(id == 'R' || id == 'r'))
			break;
		y = -1;
		while(++y < canvas_altezza)
		{
			x = -1;
			while(++x < canvas_larghezza)
			{
				pos = check_pos((float)x, (float)y, shape_x, shape_y, shape_larghezza, shape_altezza);
				if(pos == 1 || (pos == 2 && id == 'R'))
					canvas[y * canvas_larghezza +x] = color;
			}
		}
	}
	if(read != -1)
	{
		free(canvas);
		return(str_error("Error: Operation file corrupted\n", 1));
	}
	y = -1;
	while (++y < canvas_altezza)
	{
		write(1, canvas + y *canvas_larghezza, canvas_larghezza);
		write(1, "\n", 1);
	}
	free(canvas);
	fclose(file);
	return(0);
}
