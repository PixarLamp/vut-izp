//
//FILE NAME: proj2
//
//NAME: Hrklova Zuzana
//
//DATE: 26.11.2019
//
//

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define UT (0.0258563)
#define I0 (1e-12)
double diode (double u0, double r, double eps) //counts approximate diode operating point and returns Up if found
{	
	double x_1 = 0;
	double x_2 = u0;	
	double prev_middle = 0;
	int no_of_repetitions = 0;
	int max_repetitions = 1000000;
	double middle = (x_1 + x_2)/2; //finding middle of the given interval
	while (no_of_repetitions < max_repetitions)
	{
		middle = (x_1 + x_2)/2; //finding middle of the given interval
		double result = ((I0 * (exp(middle/UT) - 1)) - (u0 - middle)/r); //calculates value for the point in the middle of the interval
		double difference = middle - prev_middle;
		if (difference < 0) //absolute value of the difference of middles
			difference *= -1;
		if (difference >= eps) //if the difference (between current and previous middle) is bigger or equal to epsilon, shortens the interval and proceeds with the calculations 
		{
			if (result < 0)//shortens the interval
				x_1 = middle;
			else
				x_2 = middle;
			prev_middle = middle;
			no_of_repetitions += 1;
		}
		else //if the difference is smaller returns the Up
			return middle;
	}
	fprintf(stderr, "the result is only approximate due to max. repetitions reached\n");
	return middle;
}

int main(int argc, char *argv[])
{
	if (argc != 4)//invalid input - undesirable amount of arguments
	{
		fprintf(stderr, "invalid arguments\n");
		return 1;
	}
	double U_O = atof(argv[1]);
	double R = atof(argv[2]);
	double EPS = atof(argv[3]);
	
	if (EPS <= 0){//invalid input
		fprintf(stderr, "invalid arguments\n");
		return 1;
	}
	if (U_O <= 0){//invalid input
		fprintf(stderr, "invalid arguments\n");
		return 1;
	}
	if (R <= 0){//invalid input
		fprintf(stderr, "invalid arguments\n");
		return 1;
	}
	double Up = diode(U_O, R, EPS);
	double Ip = I0 * (exp(Up/UT) - 1);
	printf("Up = %g V\nIp = %g A\n", Up, Ip);
	return 0;
}
