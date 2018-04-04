#include "shaders.hpp"
#include "gl_include.hpp"
#include <fstream>

namespace bengine {

	unsigned int load_shaders(const char * file_path)
	{
		glog(log_target::LOADER, log_severity::info, "Loading compute shader: {0}", file_path);

		// Create the shaders
		GLuint ShaderID = glCreateShader(GL_COMPUTE_SHADER);

		// Read the Vertex Shader code from the file
		std::string ShaderCode;
		std::ifstream ShaderStream(file_path, std::ios::in);
		if (ShaderStream.is_open()) {
			std::string Line = "";
			while (getline(ShaderStream, Line))
				ShaderCode += "\n" + Line;
			ShaderStream.close();
		}
		else {
			glog(log_target::LOADER, log_severity::error, "Impossible to open {0}. Are you in the right directory ? Don't forget to read the FAQ !\n", file_path);
			getchar();
			return 0;
		}

		GLint Result = GL_FALSE;
		int InfoLogLength;


		// Compile Vertex Shader
		char const * VertexSourcePointer = ShaderCode.c_str();
		glShaderSource(ShaderID, 1, &VertexSourcePointer, NULL);
		glCompileShader(ShaderID);

		// Check Vertex Shader
		glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(ShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
			glog(log_target::LOADER, log_severity::error, "{0}", &VertexShaderErrorMessage[0]);
		}

		// Link the program
		//printf("Linking program\n");
		GLuint ProgramID = glCreateProgram();
		glAttachShader(ProgramID, ShaderID);
		glLinkProgram(ProgramID);

		// Check the program
		glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
		glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
			glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
			glog(log_target::LOADER, log_severity::error, "{0}", &ProgramErrorMessage[0]);
		}

		glDetachShader(ProgramID, ShaderID);
		glDeleteShader(ShaderID);

		glog(log_target::LOADER, log_severity::info, "Setup as shader # {0}", ProgramID);

		return ProgramID;
	}


    unsigned int load_shaders(const char * vertex_file_path,const char * fragment_file_path)
    {
		glog(log_target::LOADER, log_severity::info, "Loading shader: {0} {1}", vertex_file_path, fragment_file_path);

        // Create the shaders
        GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

        // Read the Vertex Shader code from the file
        std::string VertexShaderCode;
        std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
        if(VertexShaderStream.is_open()){
            std::string Line = "";
            while(getline(VertexShaderStream, Line))
                VertexShaderCode += "\n" + Line;
            VertexShaderStream.close();
        }else{
            glog(log_target::LOADER, log_severity::error, "Impossible to open {0}. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
            getchar();
            return 0;
        }

        // Read the Fragment Shader code from the file
        std::string FragmentShaderCode;
        std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
        if(FragmentShaderStream.is_open()){
            std::string Line = "";
            while(getline(FragmentShaderStream, Line))
                FragmentShaderCode += "\n" + Line;
            FragmentShaderStream.close();
        }

        GLint Result = GL_FALSE;
        int InfoLogLength;


        // Compile Vertex Shader
        char const * VertexSourcePointer = VertexShaderCode.c_str();
        glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
        glCompileShader(VertexShaderID);

        // Check Vertex Shader
        glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if ( InfoLogLength > 0 ){
            std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
            glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
			glog(log_target::LOADER, log_severity::error, "{0}", &VertexShaderErrorMessage[0]);
        }



        // Compile Fragment Shader
        //printf("Compiling shader : %s\n", fragment_file_path);
        char const * FragmentSourcePointer = FragmentShaderCode.c_str();
        glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
        glCompileShader(FragmentShaderID);

        // Check Fragment Shader
        glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if ( InfoLogLength > 0 ){
            std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
            glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
			glog(log_target::LOADER, log_severity::error, "{0}", &FragmentShaderErrorMessage[0]);
        }

        // Link the program
        //printf("Linking program\n");
        GLuint ProgramID = glCreateProgram();
        glAttachShader(ProgramID, VertexShaderID);
        glAttachShader(ProgramID, FragmentShaderID);
        glLinkProgram(ProgramID);

        // Check the program
        glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
        glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if ( InfoLogLength > 0 ){
            std::vector<char> ProgramErrorMessage(InfoLogLength+1);
            glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
			glog(log_target::LOADER, log_severity::error, "{0}", &ProgramErrorMessage[0]);
        }

        glDetachShader(ProgramID, VertexShaderID);
        glDetachShader(ProgramID, FragmentShaderID);

        glDeleteShader(VertexShaderID);
        glDeleteShader(FragmentShaderID);

		glog(log_target::LOADER, log_severity::info, "Setup as shader # {0}", ProgramID);

        return ProgramID;
    }

	unsigned int load_shaders(const char * vertex_file_path, const char * fragment_file_path, const char * geometry_file_path)
	{
		// Create the shaders
		GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		GLuint GeometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);

		// Read the Vertex Shader code from the file
		std::string VertexShaderCode;
		std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
		if (VertexShaderStream.is_open()) {
			std::string Line = "";
			while (getline(VertexShaderStream, Line))
				VertexShaderCode += "\n" + Line;
			VertexShaderStream.close();
		}
		else {
			glog(log_target::LOADER, log_severity::error, "Impossible to open {0}. Are you in the right directory ? Don't forget to read the FAQ !", vertex_file_path);
			getchar();
			return 0;
		}

		// Read the Fragment Shader code from the file
		std::string FragmentShaderCode;
		std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
		if (FragmentShaderStream.is_open()) {
			std::string Line = "";
			while (getline(FragmentShaderStream, Line))
				FragmentShaderCode += "\n" + Line;
			FragmentShaderStream.close();
		}

		// Read the Geometry Shader code from the file
		std::string GeometryShaderCode;
		std::ifstream GeometryShaderStream(geometry_file_path, std::ios::in);
		if (GeometryShaderStream.is_open()) {
			std::string Line = "";
			while (getline(GeometryShaderStream, Line))
				GeometryShaderCode += "\n" + Line;
			GeometryShaderStream.close();
		}

		GLint Result = GL_FALSE;
		int InfoLogLength;


		// Compile Vertex Shader
		//printf("Compiling shader : %s\n", vertex_file_path);
		char const * VertexSourcePointer = VertexShaderCode.c_str();
		glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
		glCompileShader(VertexShaderID);

		// Check Vertex Shader
		glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
			glog(log_target::LOADER, log_severity::error, "{0}", &VertexShaderErrorMessage[0]);
		}



		// Compile Fragment Shader
		//printf("Compiling shader : %s\n", fragment_file_path);
		char const * FragmentSourcePointer = FragmentShaderCode.c_str();
		glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
		glCompileShader(FragmentShaderID);

		// Check Fragment Shader
		glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
			glog(log_target::LOADER, log_severity::error, "{0}", &FragmentShaderErrorMessage[0]);
		}

		// Compile Geometry Shader
		//printf("Compiling shader : %s\n", geometry_file_path);
		char const * GeometrySourcePointer = GeometryShaderCode.c_str();
		glShaderSource(GeometryShaderID, 1, &GeometrySourcePointer, NULL);
		glCompileShader(GeometryShaderID);

		// Check Geometry Shader
		glGetShaderiv(GeometryShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(GeometryShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> GeoemetryShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(GeometryShaderID, InfoLogLength, NULL, &GeoemetryShaderErrorMessage[0]);
			glog(log_target::LOADER, log_severity::error, "{0}", &GeoemetryShaderErrorMessage[0]);
		}

		// Link the program
		//printf("Linking program\n");
		GLuint ProgramID = glCreateProgram();
		glAttachShader(ProgramID, VertexShaderID);
		glAttachShader(ProgramID, FragmentShaderID);
		glAttachShader(ProgramID, GeometryShaderID);
		glLinkProgram(ProgramID);

		// Check the program
		glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
		glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
			glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
			glog(log_target::LOADER, log_severity::error, "{0}", &ProgramErrorMessage[0]);
		}


		glDetachShader(ProgramID, VertexShaderID);
		glDetachShader(ProgramID, FragmentShaderID);
		glDetachShader(ProgramID, GeometryShaderID);

		glDeleteShader(VertexShaderID);
		glDeleteShader(FragmentShaderID);
		glDeleteShader(GeometryShaderID);

		//std::cout << "Setup as shader #" << ProgramID << "\n";

		return ProgramID;
	}

	void base_shader_t::load_shader(const std::string vertex, const std::string fragment) {
		shader_id = load_shaders(vertex.c_str(), fragment.c_str());
	}

	void base_shader_t::load_shader(const std::string vertex, const std::string fragment, const std::string geometry) {
		shader_id = load_shaders(vertex.c_str(), fragment.c_str(), geometry.c_str());
	}

	void base_shader_t::use() const {
		glUseProgram(shader_id);
	}

	int base_shader_t::get_uniform(const std::string name) const {
		const auto result = glGetUniformLocation(shader_id, name.c_str());
		glog(log_target::LOADER, log_severity::info, "Uniform [{0}] assigned to shader {1}, slot {2}", name, shader_id, result);
		glCheckError();
		return result;
	}

	unsigned int base_shader_t::get_block_index(const std::string &name) const
    {
		const auto result = glGetUniformBlockIndex(shader_id, name.c_str());
		glog(log_target::LOADER, log_severity::info, "Uniform block {0} assigned to index {1}", name, result);
		glCheckError();
		return result;
    }

	unsigned int base_shader_t::get_resource_index(const std::string &name) const
    {
		const auto result = glGetProgramResourceIndex(shader_id, GL_SHADER_STORAGE_BLOCK, name.c_str());
		glog(log_target::LOADER, log_severity::info, "Uniform block {0} assigned to index {1}", name, result);
		glCheckError();
		return result;
    }
}