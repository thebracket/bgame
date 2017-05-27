#include "shader.hpp"

std::string shader_t::read_code_file(const char * filename) {
    std::string code;
    std::ifstream code_stream(filename, std::ios::in);
    if(code_stream.is_open()){
        std::string Line = "";
        while(getline(code_stream, Line))
            code += "\n" + Line;
        code_stream.close();
        return code;
    }else{
        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", filename);
        getchar();
        throw std::runtime_error("Unable to open file.");
    }
}

void shader_t::compile_shader(GLuint &id, std::string &code, const char * filename)
{
    GLint result = GL_FALSE;
    int info_log_length;

    // Compile it
    printf("Compiling shader : %s\n", filename);
    char const * source_ptr = code.c_str();
    glShaderSource(id, 1, &source_ptr , NULL);
    glCompileShader(id);

    // Check it
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &info_log_length);
    if ( info_log_length > 0 ){
        std::vector<char> shader_error_message(info_log_length+1);
        glGetShaderInfoLog(id, info_log_length, NULL, &shader_error_message[0]);
        printf("%s\n", &shader_error_message[0]);
    }
}

void shader_t::load_shader(const char * geometry_shader_path, const char * vertex_file_path,const char * fragment_file_path)
{
    const bool has_geometry = geometry_shader_path != nullptr;
    const bool has_vertex = vertex_file_path != nullptr;
    const bool has_fragment = fragment_file_path != nullptr;



    GLuint geometry_shader_id;
    GLuint vertex_shader_id;
    GLuint fragment_shader_id;

    if (has_vertex) vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    if (has_fragment) fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    if (has_geometry) geometry_shader_id = glCreateShader(GL_GEOMETRY_SHADER_EXT);

    if (has_geometry) {
        std::string code = read_code_file(geometry_shader_path);
        compile_shader(geometry_shader_id, code, geometry_shader_path);
    }
    if (has_vertex) {
        std::string code = read_code_file(vertex_file_path);
        compile_shader(vertex_shader_id, code, vertex_file_path);
    }
    if (has_fragment) {
        std::string code = read_code_file(fragment_file_path);
        compile_shader(fragment_shader_id, code, fragment_file_path);
    }

    // Link the program
    printf("Linking program\n");
    GLuint program_id = glCreateProgram();
    if (has_geometry) {
        glProgramParameteriEXT(program_id, GL_GEOMETRY_INPUT_TYPE_EXT, GL_POINTS);
        glProgramParameteriEXT(program_id, GL_GEOMETRY_OUTPUT_TYPE_EXT, GL_TRIANGLE_STRIP);
        glProgramParameteriEXT(program_id, GL_GEOMETRY_VERTICES_OUT_EXT, 14);

        glAttachShader(program_id, geometry_shader_id);
    }
    if (has_vertex) glAttachShader(program_id, vertex_shader_id);
    if (has_fragment) glAttachShader(program_id, fragment_shader_id);
    glLinkProgram(program_id);

    // Check the program
    GLint result = GL_FALSE;
    int info_log_length;

    glGetProgramiv(program_id, GL_LINK_STATUS, &result);
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
    if ( info_log_length > 0 ){
        std::vector<char> program_error_message(info_log_length+1);
        glGetProgramInfoLog(program_id, info_log_length, nullptr, &program_error_message[0]);
        printf("%s\n", &program_error_message[0]);
    }

    if (has_geometry) glDetachShader(program_id, geometry_shader_id);
    if (has_vertex) glDetachShader(program_id, vertex_shader_id);
    if (has_fragment) glDetachShader(program_id, fragment_shader_id);

    if (has_geometry) glDeleteShader(geometry_shader_id);
    if (has_vertex) glDeleteShader(vertex_shader_id);
    if (has_fragment) glDeleteShader(fragment_shader_id);

    shader_id = program_id;
}

void shader_t::activate() {
    glUseProgram(shader_id);
}

void shader_t::deactivate() {
    glUseProgram(0);
}