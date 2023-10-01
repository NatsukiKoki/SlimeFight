#pragma once
#define _USE_MATH_DEFINES
#include<math.h>
#include<random>
#include<numbers>
#include<SFML/Graphics.hpp>
class Mathmatic {
public:
	static double Box_Muller_Random(double avg, double std_deviation)
	{
		//srand(time(0));
		double u1 = rand() * (1.0 / RAND_MAX);
		double u2 = rand() * (1.0 / RAND_MAX);
		double tmp1 = 0.0;
		double tmp2 = 0.0;
		if (u1 > 0.0 && u1 < 1.0 && u2>0.0 && u2 < 1.0)
		{
			tmp1 = log(u1);
			tmp1 *= -2;
			if (tmp1 > 0.0)
				tmp1 = sqrt(tmp1);
			tmp2 = 2 * M_PI * u2;
			tmp2 = cos(tmp2);
			tmp1 *= tmp2;
		}
		tmp1 *= std_deviation;
		tmp1 += avg;
		return tmp1;

	}

	static sf::Vector2f normailze(sf::Vector2f &c)
	{
		sf::Vector2f a=c;
		float mol = 0.0;
		mol += a.x * a.x + a.y * a.y;
		mol = sqrtf(mol);
		if (mol < 0.000001)
			return a;
		a.x = a.x / mol;
		a.y = a.y / mol;
		return a;
	}
	static float mol(sf::Vector2f kk)
	{
		return sqrtf(kk.x * kk.x + kk.y * kk.y);
	}

};
