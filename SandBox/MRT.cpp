#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>

#define WIDTH 800
#define HEIGHT 800

GLFWwindow* InitWindow(int width, int height, std::string const& name) {
	int success = glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* m_Window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(m_Window);
	return m_Window;
}

void ReadShaderFromFile(const std::string& filepath, std::string& m_ShaderSource)
{
	std::ifstream in(filepath, std::ios::in | std::ios::binary);
	if (in)
	{
		in.seekg(0, std::ios::end);
		m_ShaderSource.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&m_ShaderSource[0], m_ShaderSource.size());
		in.close();
	}
}

GLenum ShaderTypeFromString(const std::string& type)
{
	if (type == "vertex")
		return GL_VERTEX_SHADER;
	if (type == "fragment" || type == "pixel")
		return GL_FRAGMENT_SHADER;

	return GL_NONE;
}

GLuint CompileAndUploadShader(std::string const& m_ShaderSource)
{
	std::unordered_map<GLenum, std::string> shaderSources;

	const char* typeToken = "#type";
	size_t typeTokenLength = strlen(typeToken);
	size_t pos = m_ShaderSource.find(typeToken, 0);
	while (pos != std::string::npos)
	{
		size_t eol = m_ShaderSource.find_first_of("\r\n", pos);
		size_t begin = pos + typeTokenLength + 1;
		std::string type = m_ShaderSource.substr(begin, eol - begin);

		size_t nextLinePos = m_ShaderSource.find_first_not_of("\r\n", eol);
		pos = m_ShaderSource.find(typeToken, nextLinePos);
		shaderSources[ShaderTypeFromString(type)] = m_ShaderSource.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? m_ShaderSource.size() - 1 : nextLinePos));
	}

	std::vector<GLuint> shaderRendererIDs;

	GLuint program = glCreateProgram();
	for (auto& kv : shaderSources)
	{
		GLenum type = kv.first;
		std::string& source = kv.second;

		GLuint shaderRendererID = glCreateShader(type);
		const GLchar* sourceCstr = (const GLchar*)source.c_str();
		glShaderSource(shaderRendererID, 1, &sourceCstr, 0);

		glCompileShader(shaderRendererID);

		GLint isCompiled = 0;
		glGetShaderiv(shaderRendererID, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shaderRendererID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shaderRendererID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(shaderRendererID);

		}

		shaderRendererIDs.push_back(shaderRendererID);
		glAttachShader(program, shaderRendererID);
	}

	// Link our program
	glLinkProgram(program);

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		// We don't need the program anymore.
		glDeleteProgram(program);
		// Don't leak shaders either.
		for (auto id : shaderRendererIDs)
			glDeleteShader(id);
	}

	// Always detach shaders after a successful link.
	for (auto id : shaderRendererIDs)
		glDetachShader(program, id);

	return program;

}

static void AttachColorTexture(GLuint framebuffer, GLuint texture, int samples, GLenum format, uint32_t width, uint32_t height, int index) {
	// 根据传递的参数进行texture的创建和绑定
	if (samples == 1) {
		glTextureStorage2D(texture, 1, format, width, height);
		glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	else {
		glTextureStorage2DMultisample(texture, samples, format, width, height, GL_FALSE);
	}
	glNamedFramebufferTexture(framebuffer, GL_COLOR_ATTACHMENT0 + index, texture, 0);
}
static void AttachDepthTexture(GLuint framebuffer, GLuint texture, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height) {
	// 根据传递的参数进行texture的创建和绑定
	if (samples == 1) {
		glTextureStorage2D(texture, 1, format, width, height);
		glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	else {
		glTextureStorage2DMultisample(texture, samples, format, width, height, GL_FALSE);
	}
	glNamedFramebufferTexture(framebuffer, attachmentType, texture, 0);
}

int main() {
	// init opengl
	GLFWwindow* m_Window = InitWindow(WIDTH, HEIGHT, "Check");
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);
	// create opengl asset: vao vbo ebo shader

	static float vertices[] = {
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
	 -1.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};
	static unsigned int indices[] = {
		0, 1, 2, 2, 3, 0
	};

	GLuint vbo;
	glCreateBuffers(1, &vbo);
	glNamedBufferStorage(vbo, sizeof(vertices), vertices, GL_DYNAMIC_STORAGE_BIT);
	GLuint ebo;
	glCreateBuffers(1, &ebo);
	glNamedBufferStorage(ebo, sizeof(indices), indices, GL_DYNAMIC_STORAGE_BIT);
	GLuint vao;
	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,
		3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0
	);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,
		2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))
	);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	std::string shader_path = "assets/shaders/check.glsl";
	std::string shader_source;
	ReadShaderFromFile(shader_path, shader_source);
	GLuint program = CompileAndUploadShader(shader_source);

	glEnable(GL_DEPTH_TEST);
	
	GLuint framebuffer;
	glCreateFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	GLuint texture1, texture2, depth;
	glCreateTextures(GL_TEXTURE_2D, 1, &texture1);
	glCreateTextures(GL_TEXTURE_2D, 1, &texture2);
	glCreateTextures(GL_TEXTURE_2D, 1, &depth);
	AttachColorTexture(framebuffer, texture1, 1, GL_RGBA8, WIDTH, HEIGHT, 0);
	AttachColorTexture(framebuffer, texture2, 1, GL_RGBA32F, WIDTH, HEIGHT, 1);
	AttachDepthTexture(framebuffer, depth, 1, GL_DEPTH_COMPONENT32F, GL_DEPTH_ATTACHMENT, WIDTH, HEIGHT);

	static const GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, draw_buffers);


	while (true) {
		glUseProgram(program);
		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		glViewport(0, 0, WIDTH, HEIGHT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		glReadBuffer(GL_COLOR_ATTACHMENT0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBlitNamedFramebuffer(
			framebuffer, 0,
			0, 0, WIDTH, HEIGHT,
			0, 0, WIDTH, HEIGHT,
			GL_COLOR_BUFFER_BIT, GL_LINEAR
		);
		

		glfwPollEvents();
		glfwSwapBuffers(m_Window);

	}

}