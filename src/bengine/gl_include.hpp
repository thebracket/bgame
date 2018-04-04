#pragma once

// This provides a header to include GL/GLEW/GLFW3 in the right order.
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../utils/system_log.hpp"

inline GLenum glCheckError_(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
		glog(log_target::GAME, log_severity::error, "{0} | {1} |{2}", error, file, line);
    }
    return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)
//#define glCheckError()
