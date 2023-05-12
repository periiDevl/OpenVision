#ifndef OV_H
#define OV_H

class OV {
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


};

#endif
