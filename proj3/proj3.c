//
//FILE NAME: proj3
//
//NAME: Hrklova Zuzana
//
//DATE: 11. 12.2019
//
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

enum border{left = 1, right = 2, upper_lower = 4};

typedef struct
{
	int rows;
	int cols;
	unsigned char *cells;
} Map;
void init_map(Map *map)
{
	map->rows = 0;
	map->cols = 0;
	map->cells = NULL;
}
void malloc_map(Map *map)
{
	map->cells = malloc(map->rows * map->cols * sizeof(unsigned char));
}
void map_set_value(Map *map, int row, int col, unsigned char cell_value)
{
	map->cells[row * map->cols + col] = cell_value;
}
void load_map(FILE *file, Map *map)
{
	fscanf(file, "%d %d", &map->rows, &map->cols);
	malloc_map(map);
	int cell_value = 0;
	if (map->cells != NULL)
	{
		for (int i = 0; i < map->rows; i++)//rows of the maze
		{	
			for (int j = 0; j < map->cols; j++)//columns of the maze
			{	
				fscanf(file, "%d", &cell_value);
				if ((cell_value != ' ') || (cell_value != '\n'))
				{
					map_set_value(map, i, j, cell_value);
				}
			}
		}
	}
}
void free_map( Map *map)
{
	free(map->cells);
}
bool isborder(Map *map, int r, int c, int border)//decides whether the given side of the triangle has a border
{
	int cell_value = map->cells[r * map->cols + c];
	if ((cell_value&border) > 0)
		return true;
	else
		return false;
}
bool test(Map *map) //tests the validity of the given maze
{
	for (int i=0; i< map->rows; i++)
	{
		for (int j=0; j< map->cols; j++)
		{
			if (j < map->cols-1)//ensures two cells next to each other have border on the same side
			{	
				bool first = isborder(map, i, j, right);
				bool second = isborder(map, i, j+1, left);
				if (first != second)
					return false;
			}
			if (i < map->rows - 1)//ensures that cells below and above share common border
			{	
				if ((i%2 == 1) && (j%2 == 0))//in the odd row, even column
				{
					bool first = isborder(map, i, j, upper_lower);
					bool second = isborder(map, i+1, j, upper_lower);
					if (first != second)
						return false;
				}
				if ((i%2 == 0) && (j%2 == 1))//in even row, odd column
				{
					bool first = isborder(map, i, j, upper_lower);
					bool second = isborder(map, i+1, j, upper_lower);
					if (first != second)
						return false;
				}
			}

		}
	}
	return true;//given maze is valid
}
int start_cell(Map *map, int row, int col)//finds out wheter it is possible to enter the maze through the cell given as an input and if so then through which border
{
	int border;
	if ((col == 1)&&(row == 1))
	{
		border = left;
		if (isborder(map, row-1, col-1, border) == true)
			border = upper_lower;
	}
	else if((col == map->cols)&&(row == 1)&&(col%2 == 0))
		border = right;
	else if((col == map->cols)&&(row == 1)&&(col%2 == 1))
	{
		border = right;
		if (isborder(map, row-1, col-1, border) == true)
			border = upper_lower;
	}
	else if ((row == map->rows)&&(col == 1)&&(row%2 == 0)) 
	{
		border = left;
		if (isborder(map, row-1, col-1, border) == true)
			border = upper_lower;
	}
	else if ((row == map->rows)&&(col == 1)&&(row%2 == 1))
		border = left;
	else if ((row == map->rows)&&(col == map->cols)&&(row%2 == 0))
	{
		border = right;
		if (isborder(map, row-1, col-1, border)==true)
			border = upper_lower;
	}
	else if ((row == map->rows)&&(col == map->cols)&&(row%2 == 1))
		border = right;
	else if (col == 1)
		border = left;
	else if (col == map->cols)
		border = right;
	else if ((row == 1)&&(col%2 == 1))
		border = upper_lower;
	else if ((row == map->rows)&&(row%2 == 0)&&(col%2 == 1))
		border = upper_lower;
	else if ((row == map->rows)&&(row%2 == 1)&&(col%2 == 0))
		border = upper_lower;
	else
	{
		printf("Not possible to enter the given maze through this cell\n");
		return 0;
	}
	return border;
}
void right_hand_rule(Map *map, int row, int col, int border)//finds the way out of the maze by right hand rule
{
	printf("%d, %d\n", row, col);
	while ((row > 0)&&(row <= map->rows)&&(col > 0)&&(col <= map->cols))
	{
		if (((row%2 == 1)&&(col%2 == 1))||((row%2 == 0)&&(col%2 == 0)))//if the cell has shape of a triangle upside down
		{
			if (border == 1)
			{
				if (isborder(map, row-1, col-1, 2) == false)
				{	col++;
					border = 1;
				}
				else if (isborder(map, row-1, col-1, 4) == false)
				{
					row = row - 1;
					border = 4;
				}	
				else if (isborder(map, row-1, col-1, 1) == false)
				{	
					col = col - 1;
					border = 2;
				}
			}
			else if (border == 2)
			{
				if (isborder(map, row-1, col-1, 4) == false)
				{	
					row = row - 1;
					border = 4;
				}
				else if (isborder(map, row-1, col-1, 1) == false)
				{	
					col = col - 1;
					border = 2;
				}
				else if (isborder(map, row-1, col-1, 2) == false)
				{	
					col++;
					border = 1;
				}
			}
			else if (border == 4)
			{
				if (isborder(map, row-1, col-1, 1) == false)
				{	
					col = col - 1;
					border = 2;
				}
				else if (isborder(map, row-1, col-1, 2) == false)
				{	
					col++;
					border = 1;
				}
				else if (isborder(map, row-1, col-1, 4) == false)
				{	
					row = row - 1;
					border = 4;
				}
			}
			if ((row > 0)&&(row <= map->rows)&&(col > 0)&&(col <= map->cols))
				printf("%d, %d\n", row, col);
		}
		else //if the cell has the shape of a normal triangle
		{
			if (border == 1)
			{	
				if (isborder(map, row-1, col-1, 4) == false)
				{	
					row++;
					border = 4;
				}
				else if (isborder(map, row-1, col-1, 2) == false)
				{
					col++;
					border = 1;
				}
				else if (isborder(map, row-1, col-1, 1) == false)
				{	
					col = col - 1;
					border = 2;
				}
			}
			else if (border == 2)
			{
				if (isborder(map, row-1, col-1, 1) == false)
				{	
					col = col - 1;
					border = 2;
				}
				else if (isborder(map, row-1, col-1, 4) == false)
				{	
					row++;
					border = 4;
				}
				else if (isborder(map, row-1, col-1, 2) == false)
				{	
					col++;
					border = 1;
				}
			}
			else if (border == 4)
			{
				if (isborder(map, row-1, col-1, 2) == false)
				{	
					col++;
					border = 1;
				}
				else if (isborder(map, row-1, col-1, 1) == false)
				{	
					col = col - 1;
					border = 2;
				}
				else if (isborder(map, row-1, col-1, 4) == false)
				{	
					row++;
					border = 4;
				}
			}
			if ((row > 0)&&(row <= map->rows)&&(col > 0)&&(col <= map->cols))
				printf("%d, %d\n", row, col);
		}
	}
}
void left_hand_rule(Map *map, int row, int col, int border)//finds the way out of the maze by left hand rule
{
	printf("%d, %d\n", row, col);
	while ((row > 0)&&(row <= map->rows)&&(col > 0)&&(col <= map->cols))
	{
		if (((row%2 == 1)&&(col%2 == 1))||((row%2 == 0)&&(col%2 == 0)))//if the cell has shape of a triangle upside down
 
		{
			if (border == 1)
			{
				if (isborder(map, row-1, col-1, 4) == false)
				{	row = row - 1;
					border = 4;
				}
				else if (isborder(map, row-1, col-1, 2) == false)
				{
					col++;
					border = 1;
				}	
				else if (isborder(map, row-1, col-1, 1) == false)
				{	
					col = col - 1;
					border = 2;
				}
			}
			else if (border == 2)
			{
				if (isborder(map, row-1, col-1, 1) == false)
				{	
					col = col - 1;
					border = 2;
				}
				else if (isborder(map, row-1, col-1, 4) == false)
				{	
					row = row - 1;
					border = 4;
				}
				else if (isborder(map, row-1, col-1, 2) == false)
				{	
					col++;
					border = 1;
				}
			}
			else if (border == 4)
			{
				if (isborder(map, row-1, col-1, 2) == false)
				{	
					col++;
					border = 1;
				}
				else if (isborder(map, row-1, col-1, 1) == false)
				{	
					col = col - 1;
					border = 2;
				}
				else if (isborder(map, row-1, col-1, 4) == false)
				{	
					row = row - 1;
					border = 4;
				}
			}
			if ((row > 0)&&(row <= map->rows)&&(col > 0)&&(col <= map->cols))
				printf("%d, %d\n", row, col);
		}
		else //if the cell has the shape of a normal triangle

		{
			if (border == 1)
			{	
				if (isborder(map, row-1, col-1, 2) == false)
				{	
					col++;
					border = 1;
				}
				else if (isborder(map, row-1, col-1, 4) == false)
				{
					row++;
					border = 4;
				}
				else if (isborder(map, row-1, col-1, 1) == false)
				{	
					col = col - 1;
					border = 2;
				}
			}
			else if (border == 2)
			{
				if (isborder(map, row-1, col-1, 4) == false)
				{	
					row++;
					border = 4;
				}
				else if (isborder(map, row-1, col-1, 1) == false)
				{	
					col = col - 1;
					border = 2;
				}
				else if (isborder(map, row-1, col-1, 2) == false)
				{	
					col++;
					border = 1;
				}
			}
			else if (border == 4)
			{
				if (isborder(map, row-1, col-1, 1) == false)
				{	
					col = col -1;
					border = 2;
				}
				else if (isborder(map, row-1, col-1, 2) == false)
				{	
					col++;
					border = 1;
				}
				else if (isborder(map, row-1, col-1, 4) == false)
				{	
					row++;
					border = 4;
				}
			}
			if ((row > 0)&&(row <= map->rows)&&(col > 0)&&(col <= map->cols))
				printf("%d, %d\n", row, col);
		}
	}
}


int main(int argc, char *argv[])
{
	if (strcmp(argv[1], "--help") == 0)
	{	
		printf("--test file.txt(containing the maze) -> finds out whether or not the given maze is valid\n--rpath Rows Cols file.txt -> finds the way out of the maze by the right hand rule\n--lpath Rows Cols file.txt -> finds the way out of the maze by the left hand rule\n");
		return 0;
	}
	Map map;
	FILE *file;
	int index;
	if (strcmp(argv[1], "--test") == 0)
		index = 2;
	if ((strcmp(argv[1], "--rpath") == 0) || (strcmp(argv[1], "--lpath") == 0))
	{	
		index = 4;
	}
	file = fopen(argv[index], "r");
	if (file == NULL)
		return 1;
	init_map( &map);
	load_map(file, &map);
	fclose(file);
	if (test(&map) == true)
	{
		if (index == 2)
			printf("valid\n");
		else if (index == 4)
		{
			int row = atoi(argv[2]);
			int col = atoi(argv[3]);
			if ((row > map.rows)||(col > map.cols))
			{       
				printf("Not possible to enter given maze through this cell\n");
				return 1;
			}	
			int border = start_cell(&map, row, col);
			if (border == 0)
				return 1;
			if (isborder(&map, row-1, col-1, border) == true)
			{
				printf("Not possible to enter the given maze through this cell\n");
				return 1;
			}
			if (strcmp(argv[1], "--rpath") == 0)
				right_hand_rule(&map, row, col, border);
			else
				 left_hand_rule(&map, row, col, border);
		}
	}
	else if (index == 2)
		printf("INVALID\n");
	free_map(&map);
	return 0;
}
