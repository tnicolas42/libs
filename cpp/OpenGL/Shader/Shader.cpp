#include "Shader.hpp"
#include "Logging.hpp"

/*
	load shader source code to string
*/
void	fillShaderStr(std::string const vsPath, std::string const fsPath, std::string const gsPath,
std::string *vsCode, std::string *fsCode, std::string *gsCode
) {
	std::ifstream		vsFile;
	std::ifstream		fsFile;
	std::ifstream		gsFile;
	std::stringstream	sStream;

	vsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		vsFile.open(vsPath);
		sStream << vsFile.rdbuf();
		*vsCode = sStream.str();
		vsFile.close();

		sStream.clear();
		sStream.str(std::string());
		fsFile.open(fsPath);
		sStream << fsFile.rdbuf();
		*fsCode = sStream.str();
		fsFile.close();

		if (!gsPath.empty()) {
			sStream.clear();
			sStream.str(std::string());
			gsFile.open(gsPath);
			sStream << gsFile.rdbuf();
			*gsCode = sStream.str();
			gsFile.close();
		}
	}
	catch (std::ifstream::failure e) {
		logErr("failed to load Shader file: " << vsPath);
		throw Shader::ShaderCompileException();
	}
}

Shader::Shader(std::string const vsPath, std::string const fsPath, std::string const gsPath) {
	std::string	vsCode;
	std::string	fsCode;
	std::string	gsCode;
	const char	*vsData;
	const char	*fsData;
	const char	*gsData;
	uint32_t	vertex = 0;
	uint32_t	fragment = 0;
	uint32_t	geometry = 0;

	fillShaderStr(vsPath, fsPath, gsPath, &vsCode, &fsCode, &gsCode);

	// vertex shader
	vsData = vsCode.c_str();
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vsData, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");

	// fragment shader
	fsData = fsCode.c_str();
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fsData, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");

	// geometry shader
	if (!gsPath.empty()) {
		gsData = gsCode.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gsData, NULL);
		glCompileShader(geometry);
		checkCompileErrors(geometry, "GEOMETRY");
	}

	// shader Program
	id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	if (!gsPath.empty()) {
		glAttachShader(id, geometry);
	}
	glLinkProgram(id);
	checkCompileErrors(id, "PROGRAM");

	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (!gsPath.empty())
		glDeleteShader(geometry);
}

Shader::Shader(Shader const &src) {
	*this = src;
}

Shader::~Shader() {
	glDeleteProgram(id);
}

Shader &Shader::operator=(Shader const &rhs) {
	if (this != &rhs) {
		this->id = rhs.id;
	}
	return *this;
}

void	Shader::use() {
	glUseProgram(id);
}
void	Shader::unuse() {
	glUseProgram(0);
}
void	Shader::setBool(const std::string &name, bool value) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), static_cast<int>(value));
}
void	Shader::setInt(const std::string &name, int value) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}
void	Shader::setFloat(const std::string &name, float value) const {
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}
void	Shader::setDouble(const std::string &name, double value) const {
	glUniform1d(glGetUniformLocation(id, name.c_str()), value);
}
// ------------------------------------------------------------------------
void	Shader::setVec2(const std::string &name, float x, float y) const {
	glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
}
void	Shader::setVec2(const std::string &name, const glm::vec2 &vec) const {
	glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &vec[0]);
}
void	Shader::setVec2Double(const std::string &name, double x, double y) const {
	glUniform2d(glGetUniformLocation(id, name.c_str()), x, y);
}
void	Shader::setVec2Double(const std::string &name, const glm::tvec2<double> &vec) const {
	glUniform2dv(glGetUniformLocation(id, name.c_str()), 1, &vec[0]);
}
// ------------------------------------------------------------------------
void	Shader::setVec3(const std::string &name, float x, float y, float z) const {
	glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
}
void	Shader::setVec3(const std::string &name, const glm::vec3 &vec) const {
	glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &vec[0]);
}
void	Shader::setVec3Double(const std::string &name, double x, double y, double z) const {
	glUniform3d(glGetUniformLocation(id, name.c_str()), x, y, z);
}
void	Shader::setVec3Double(const std::string &name, const glm::tvec3<double> &vec) const {
	glUniform3dv(glGetUniformLocation(id, name.c_str()), 1, &vec[0]);
}
// ------------------------------------------------------------------------
void	Shader::setVec4(const std::string &name, float x, float y, float z, float w) const {
	glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
}
void	Shader::setVec4(const std::string &name, const glm::vec4 &vec) const {
	glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &vec[0]);
}
void	Shader::setVec4Double(const std::string &name, double x, double y, double z, double w) const {
	glUniform4d(glGetUniformLocation(id, name.c_str()), x, y, z, w);
}
void	Shader::setVec4Double(const std::string &name, const glm::tvec4<double> &vec) const {
	glUniform4dv(glGetUniformLocation(id, name.c_str()), 1, &vec[0]);
}
// ------------------------------------------------------------------------
void	Shader::setMat2(const std::string &name, const glm::mat2 &mat) const {
	glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void	Shader::setMat3(const std::string &name, const glm::mat3 &mat) const {
	glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void	Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

/*
	checking shader compilation/linking errors.
*/
void	Shader::checkCompileErrors(uint32_t shader, std::string type) {
	int		success;
	char	infoLog[1024];

	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			logErr("Shader Compilation, type: " << type << std::endl << infoLog);
			throw Shader::ShaderCompileException();
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			logErr("Shader Linking, type: " << type << std::endl << infoLog);
			throw Shader::ShaderCompileException();
		}
	}
}
