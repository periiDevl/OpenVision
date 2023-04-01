#pragma once

float Deg(float radians)
{

	return radians * 3.14159 / 180;
}

float Lerp(float from, float to, float time)
{
	return from + time * (to - from);
}