#pragma once

namespace gl_frustrum {

    extern float frustum[6][4];

    void extract();

    inline bool point_in_frustrum( float x, float y, float z ) noexcept
    {
        int p;

        for( p = 0; p < 6; p++ )
            if( frustum[p][0] * x + frustum[p][1] * y + frustum[p][2] * z + frustum[p][3] <= 0 )
                return false;
        return true;
    }
}