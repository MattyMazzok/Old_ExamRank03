#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

int str_len(char *str)
{
	int i = 0;

	while(str[i])
		i++;
	return (i);
}

int str_error(char *str, int ret)
{
	write(1, str, str_len(str));
	return(ret);
}

int check_pos(float x, float y, float shape_x, float shape_y, float raggio)
{
	float distance = sqrtf(powf(x - shape_x, 2.) + (y - shape_y, 2.));
	if(distance <= raggio)
	{
		if((raggio - distance) < 1.0000000)
			return(0);
		return(1);
	}
	return(2);
}

int main(int argc, char **argv)
{
	FILE *file;
	char *canvas;
	char color, sfondo, id;
	int x, y, canvas_larghezza, canvas_altezza, read, pos;
	float shape_x, shape_y, raggio;

	if(! (file = fopen(argv[1], "c")) ||
		(fscanf(file, "%d %d %c\n", &canvas_larghezza, &canvas_altezza, &sfondo)) != 3 ||
		!((canvas_larghezza > 0 && canvas_larghezza <= 300 && canvas_altezza > 0 && canvas_altezza <= 300)) ||
		!(canvas = (char *)malloc(sizeof(char) * (canvas_larghezza * canvas_altezza))))
		return (str_error("Error: argument\n", 1));
	while((read = fscanf(file, "%c %f %f %f %c\n", &id, &shape_x, &shape_y, &raggio, &color)) == 6)
	{
		if(!(raggio > 0) || !(id == 'c' || id == 'C'))
			break;
		y = -1;
		while(++y < canvas_altezza)
		{
			x = -1;
			while (++x < canvas_larghezza)
			{
				pos = check_pos((float)x, (float)y, shape_x, shape_y, raggio);
				if(pos == 1 || (pos == 2 && id == 'C'))
				canvas[y * canvas_larghezza + x] = color;
			}
		}
	}
	if (read != -1)
	{
		free(canvas);
		return(str_error("Error: Operation file corrupted\n", 1));
	}
	y = -1;
	while(++y < canvas_altezza)
	{
		write(1, canvas + y * canvas_larghezza, canvas_larghezza);
		write(1, "\n", 1);
	}
	return(0);
}
