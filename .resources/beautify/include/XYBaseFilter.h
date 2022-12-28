#pragma once
namespace xy {
	class XYBaseFilter {
		const char* vertexCode = 
			"#version 330 core\n"
			"layout(location = 0) in vec3 aPos;\n"
			"layout(location = 1) in vec2 aTexCoord;\n"
			"out vec2 texCoord;\n"
			"void main() {\n"
			"	gl_Position = vec4(aPos, 1.0);\n"
			"	texCoord = aTexCoord;\n"
			"}\n"
		;
		const char* fragmentCode =
			"#version 330 core\n"
			"out vec4 fragColor;\n"
			"in vec2 texCoord;\n"
			"uniform sampler2D texture;\n"
			"void main()\n"
			"{\n"
			"	fragColor = texture2D(texture, texCoord);\n"
			"}\n"
			;
		float vertices[20] = {
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f,0.0f,0.0f,
			-1.0f,1.0f,0.0f,0.0f,1.0f
		};
		unsigned int indices[6] = {
			0,1,3,1,2,3
		};
	public:
		void init();
		void destroy();
	public:
		unsigned int vertex_id = 0;
		unsigned int fragment_id = 0;
		unsigned int program_id = 0;
		unsigned int vbo = 0, vao = 0, ebo = 0;
	};
} 