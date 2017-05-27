// GEOMETRY SHADER
// This takes a series of points, locations and colors as inputs. It outputs cubes in the appropriate locations.
#version 120
#extension GL_EXT_geometry_shader4: enable

/*
Vertex  Triangle    Face
------+-----------+-----
0 0 0
0 1 0
1 0 0  000 010 100  **0
1 1 0  100 010 110  **0
1 1 1  100 110 111  1**
0 1 0  111 110 010  *1*
0 1 1  111 010 011  *1*
0 0 1  011 010 001  0**
1 1 1  011 001 111  **1
1 0 1  111 001 101  **1
1 0 0  111 101 100  1**
0 0 1  100 101 001  *0*
0 0 0  100 001 000  *0*
0 1 0  000 001 010  0**
*/

void main()
{
   //gl_Position = gl_PositionIn[0];
   //EmitVertex();

    for (int i=0; i<gl_VerticesIn; ++i) {
        // 0 0 0
        gl_Position = gl_PositionIn[i];
        gl_Position.xyz -= 0.5f;
        EmitVertex();

        // 0 1 0
        gl_Position = gl_PositionIn[i];
        gl_Position.xz -= 0.5f; gl_Position.y += 0.5f;
        EmitVertex();

        // 1 0 0
        gl_Position = gl_PositionIn[i];
        gl_Position.yz -= 0.5f; gl_Position.x += 0.5f;
        EmitVertex();

        // 1 1 0
        gl_Position = gl_PositionIn[i];
        gl_Position.z -= 0.5f; gl_Position.xy += 0.5f;
        EmitVertex();

        // 1 1 1
        gl_Position = gl_PositionIn[i];
        gl_Position.xyz += 0.5f;
        EmitVertex();

        // 0 1 0
        gl_Position = gl_PositionIn[i];
        gl_Position.xy -= 0.5f; gl_Position.y += 0.5f;
        EmitVertex();

        // 0 1 1
        gl_Position = gl_PositionIn[i];
        gl_Position.x -= 0.5f; gl_Position.yz += 0.5f;
        EmitVertex();

        // 0 0 1
        gl_Position = gl_PositionIn[i];
        gl_Position.xy -= 0.5f; gl_Position.z += 0.5f;
        EmitVertex();

        // 1 1 1
        gl_Position = gl_PositionIn[i];
        gl_Position.xyz += 0.5f;
        EmitVertex();

        // 1 0 1
        gl_Position = gl_PositionIn[i];
        gl_Position.y -= 0.5f; gl_Position.xz += 0.5f;
        EmitVertex();

        // 1 0 0
        gl_Position = gl_PositionIn[i];
        gl_Position.yz -= 0.5f; gl_Position.x += 0.5f;
        EmitVertex();

        // 0 0 1
        gl_Position = gl_PositionIn[i];
        gl_Position.xy -= 0.5f; gl_Position.z += 0.5f;
        EmitVertex();

        // 0 0 0
        gl_Position = gl_PositionIn[i];
        gl_Position.xyz -= 0.5f;
        EmitVertex();

        // 0 1 0
        gl_Position = gl_PositionIn[i];
        gl_Position.xz -= 0.5f; gl_Position.y += 0.5f;
        EmitVertex();

        EndPrimitive();
    }


}