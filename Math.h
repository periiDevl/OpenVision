#ifndef MATH_H
#define MATH_H

class Math {
public:
    static float Deg(float radians)
    {
        return radians * 180 / 3.14159;
    }
    static float Rad(float radians)
    {
        return radians;
    }
    static float Lerp(float from, float to, float time)
    {
        return from + time * (to - from);
    }
    static float Power(float base, float exponent)
    {
        return pow(base, exponent);
    }
    static float Abs(float value)
    {
        return value >= 0 ? value : -value;
    }
    static float Sqrt(float value)
    {
        return sqrt(value);
    }
    static float Ceil(float value)
    {
        return ceil(value);
    }
    static float Floor(float value)
    {
        return floor(value);
    }
    static float Sin(float radians)
    {
        return sin(radians);
    }

    static float Cos(float radians)
    {
        return cos(radians);
    }

    static float Tan(float radians)
    {
        return tan(radians);
    }

};

#endif  // MATH_H
