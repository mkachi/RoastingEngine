#pragma once

struct AudioPosition
{
    union
    {
        struct
        {
            float x;
            float y;
            float z;
        };
        float toArray[3];
    };
    AudioPosition() : x(0.0f), y(0.0f), z(0.0f) {}
    AudioPosition(float xx, float yy, float zz)
    {
        x = xx;
        y = yy;
        z = zz;
    }
};