#include <zr_pch.h>

#include "Zero/Core/Log.h"
#include "Zero/Core/Profiller.h"
#include "OpenGLShader.h"
#include "GL_ERR_CHECK.h"

namespace zr
{
	static unsigned char ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return ShaderType::VertexShader;
		if (type == "fragment" || type == "pixel")
			return ShaderType::FragmentShader;
		if (type == "geometry" || type == "geom")
			return ShaderType::GeometryShader;

		return 0;
	}

	OpenGLShader::OpenGLShader() :
		Shader(),
		mVertexShaderId(0U),
		mFragmentShaderId(0U),
		mGeometryShaderId(0U),
		mProgramId(0U),
		mName()
	{
		ZR_PROFILER_FUNCTION();

	}

	OpenGLShader::OpenGLShader(const std::string& filePath) :
		Shader(),
		mVertexShaderId(0U),
		mFragmentShaderId(0U),
		mGeometryShaderId(0U),
		mProgramId(0U),
		mName()
	{
		ZR_PROFILER_FUNCTION();

		if (!loadFromFile(filePath)) {
			throw std::runtime_error("Can't load shader from file: " + filePath);
		}
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath) :
		Shader(),
		mName(name)
	{
		ZR_PROFILER_FUNCTION();

		if (!createVertexAndFragShaders(vertexShaderPath, fragmentShaderPath)) {
			throw std::runtime_error("Can't load shader from file: ");
		}

		if (!createAndLinkProgram(VertexShader | FragmentShader)) {
			throw std::runtime_error("Can't load shader from file: ");
		}
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& geometryShaderPath) :
		Shader(),
		mName(name)
	{
		ZR_PROFILER_FUNCTION();

		if (!createVertexAndFragShaders(vertexShaderPath, fragmentShaderPath)) {
			throw std::runtime_error("Can't load shader from file: " + vertexShaderPath + " OR " + fragmentShaderPath);
		}

		if (!createGeometryShader(geometryShaderPath)) {
			throw std::runtime_error("Can't load shader from file: " + geometryShaderPath);
		}

		if (!createAndLinkProgram(VertexShader | FragmentShader | GeometryShader)) {
			throw std::runtime_error("Can't load shader from file: ");
		}
	}

	OpenGLShader::~OpenGLShader()
	{
		ZR_PROFILER_FUNCTION();

		if (mProgramId != 0U) {
			GL_ERR_CHECK(glDeleteProgram(mProgramId));
			mProgramId = 0U;
		}
	}

	const std::string& OpenGLShader::getName() const
	{
		return mName;
	}

	bool OpenGLShader::loadFromFile(const std::string& filePath)
	{
		std::string source = readFile(filePath);
		if (source.empty()) return false;

		auto& shaderSources = preProcess(source);

		// Extract name from filepath
		auto lastSlash = filePath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filePath.rfind('.');
		auto count = lastDot == std::string::npos ? filePath.size() - lastSlash : lastDot - lastSlash;
		auto& shaderName = filePath.substr(lastSlash, count);

		bool hasGeometryShader = shaderSources.find(ShaderType::GeometryShader) != shaderSources.end();
		bool returnResult = false;
		if (hasGeometryShader) {
			return loadFromStrings(shaderName, shaderSources[ShaderType::VertexShader], shaderSources[ShaderType::FragmentShader], shaderSources[ShaderType::GeometryShader]);
		}
		
		return loadFromStrings(shaderName, shaderSources[ShaderType::VertexShader], shaderSources[ShaderType::FragmentShader]);
	}

	bool OpenGLShader::loadFromFiles(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
	{
		if (!createVertexAndFragShaders(vertexShaderPath, fragmentShaderPath)) {
			return false;
		}

		if (!createAndLinkProgram(VertexShader | FragmentShader)) {
			return false;
		}
		mName = name;
		return true;
	}

	bool OpenGLShader::loadFromFiles(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& geometryShaderPath)
	{
		if (!createVertexAndFragShaders(vertexShaderPath, fragmentShaderPath)) {
			return false;
		}

		if (!createGeometryShader(geometryShaderPath)) {
			return false;
		}

		if (!createAndLinkProgram(VertexShader | FragmentShader | GeometryShader)) {
			return false;
		}
		mName = name;
		return true;
	}

	bool OpenGLShader::loadFromStrings(const std::string& name, const std::string& vertexShader, const std::string& fragmentShader)
	{
		if (vertexShader.size() != 0 && fragmentShader.size() != 0) {
			const char* vertexShaderSource = vertexShader.c_str();
			// Compile vertex shader and check for errors
			GL_ERR_CHECK(mVertexShaderId = glCreateShader(GL_VERTEX_SHADER));
			GL_ERR_CHECK(glShaderSource(mVertexShaderId, 1, &vertexShaderSource, NULL));
			GL_ERR_CHECK(glCompileShader(mVertexShaderId));
			if (!checkCompilationError(VertexShader, mVertexShaderId)) {
				if (mVertexShaderId) {
					GL_ERR_CHECK(glDeleteShader(mVertexShaderId));
					mVertexShaderId = 0;
				}
				return false;
			}

			const char* fragmentShaderSource = fragmentShader.c_str();
			// Compile fragment shader and check for errors
			GL_ERR_CHECK(mFragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER));
			GL_ERR_CHECK(glShaderSource(mFragmentShaderId, 1, &fragmentShaderSource, NULL));
			GL_ERR_CHECK(glCompileShader(mFragmentShaderId));
			if (!checkCompilationError(FragmentShader, mFragmentShaderId)) {
				if (mFragmentShaderId) {
					GL_ERR_CHECK(glDeleteShader(mVertexShaderId));
					GL_ERR_CHECK(glDeleteShader(mFragmentShaderId));

					mVertexShaderId = 0;
					mFragmentShaderId = 0;
				}
				return false;
			}

			if (!createAndLinkProgram(VertexShader | FragmentShader)) {
				return false;
			}
		}
		mName = name;
		return true;
	}

	bool OpenGLShader::loadFromStrings(const std::string& name, const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader)
	{
		if (vertexShader.size() != 0 && fragmentShader.size() != 0 && geometryShader.size() != 0) {
			const char* vertexShaderSource = vertexShader.c_str();
			// Compile vertex shader and check for errors
			GL_ERR_CHECK(mVertexShaderId = glCreateShader(GL_VERTEX_SHADER));
			GL_ERR_CHECK(glShaderSource(mVertexShaderId, 1, &vertexShaderSource, NULL));
			GL_ERR_CHECK(glCompileShader(mVertexShaderId));
			if (!checkCompilationError(VertexShader, mVertexShaderId)) {
				if (mVertexShaderId) {
					GL_ERR_CHECK(glDeleteShader(mVertexShaderId));
					mVertexShaderId = 0;
				}
				return false;
			}

			const char* fragmentShaderSource = fragmentShader.c_str();
			// Compile fragment shader and check for errors
			GL_ERR_CHECK(mFragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER));
			GL_ERR_CHECK(glShaderSource(mFragmentShaderId, 1, &fragmentShaderSource, NULL));
			GL_ERR_CHECK(glCompileShader(mFragmentShaderId));
			if (!checkCompilationError(FragmentShader, mFragmentShaderId)) {
				if (mFragmentShaderId) {
					GL_ERR_CHECK(glDeleteShader(mVertexShaderId));
					GL_ERR_CHECK(glDeleteShader(mFragmentShaderId));

					mVertexShaderId = 0;
					mFragmentShaderId = 0;
				}
				return false;
			}

			const char* geometryShaderSource = geometryShader.c_str();
			// Compile vertex shader and check for errors
			GL_ERR_CHECK(mGeometryShaderId = glCreateShader(GL_GEOMETRY_SHADER));
			GL_ERR_CHECK(glShaderSource(mGeometryShaderId, 1, &geometryShaderSource, NULL));
			GL_ERR_CHECK(glCompileShader(mGeometryShaderId));
			if (!checkCompilationError(GeometryShader, mGeometryShaderId)) {
				if (mGeometryShaderId) {
					GL_ERR_CHECK(glDeleteShader(mVertexShaderId));
					GL_ERR_CHECK(glDeleteShader(mFragmentShaderId));
					GL_ERR_CHECK(glDeleteShader(mGeometryShaderId));

					mVertexShaderId = 0;
					mFragmentShaderId = 0;
					mGeometryShaderId = 0;
				}
				return false;
			}

			if (!createAndLinkProgram(VertexShader | FragmentShader | GeometryShader)) {
				return false;
			}
		}
		mName = name;
		return true;
	}

	void OpenGLShader::bind() const
	{
		ZR_PROFILER_FUNCTION();

		GL_ERR_CHECK(glUseProgram(mProgramId));
	}

	void OpenGLShader::unbind() const
	{
		ZR_PROFILER_FUNCTION();

		GL_ERR_CHECK(glUseProgram(0));
	}

	void OpenGLShader::setUniform(const char* uniformName, float value) const
	{
		ZR_PROFILER_FUNCTION();

		int uniformLocation = getUniformLocation(uniformName);
		if (uniformLocation != -1) {
			GL_ERR_CHECK(glUniform1f(uniformLocation, value));
		}
	}

	void OpenGLShader::setUniform(const char* uniformName, float valueX, float valueY) const
	{
		ZR_PROFILER_FUNCTION();

		int uniformLocation = getUniformLocation(uniformName);
		if (uniformLocation != -1) {
			GL_ERR_CHECK(glUniform2f(uniformLocation, valueX, valueY));
		}
	}

	void OpenGLShader::setUniform(const char* uniformName, float valueX, float valueY, float valueZ) const
	{
		ZR_PROFILER_FUNCTION();

		int uniformLocation = getUniformLocation(uniformName);
		if (uniformLocation != -1) {
			GL_ERR_CHECK(glUniform3f(uniformLocation, valueX, valueY, valueZ));
		}
	}

	void OpenGLShader::setUniform(const char* uniformName, float valueX, float valueY, float valueZ, float valueW) const
	{
		ZR_PROFILER_FUNCTION();

		int uniformLocation = getUniformLocation(uniformName);
		if (uniformLocation != -1) {
			GL_ERR_CHECK(glUniform4f(uniformLocation, valueX, valueY, valueZ, valueW));
		}
	}

	void OpenGLShader::setUniform(const char* uniformName, int value) const
	{
		ZR_PROFILER_FUNCTION();

		int uniformLocation = getUniformLocation(uniformName);
		if (uniformLocation != -1) {
			GL_ERR_CHECK(glUniform1i(uniformLocation, value));
		}
	}

	void OpenGLShader::setUniform(const char* uniformName, int valueX, int valueY) const
	{
		ZR_PROFILER_FUNCTION();

		int uniformLocation = getUniformLocation(uniformName);
		if (uniformLocation != -1) {
			GL_ERR_CHECK(glUniform2i(uniformLocation, valueX, valueY));
		}
	}

	void OpenGLShader::setUniform(const char* uniformName, int valueX, int valueY, int valueZ) const
	{
		ZR_PROFILER_FUNCTION();

		int uniformLocation = getUniformLocation(uniformName);
		if (uniformLocation != -1) {
			GL_ERR_CHECK(glUniform3i(uniformLocation, valueX, valueY, valueZ));
		}
	}

	void OpenGLShader::setUniform(const char* uniformName, int valueX, int valueY, int valueZ, int valueW) const
	{
		ZR_PROFILER_FUNCTION();

		int uniformLocation = getUniformLocation(uniformName);
		if (uniformLocation != -1) {
			GL_ERR_CHECK(glUniform4i(uniformLocation, valueX, valueY, valueZ, valueW));
		}
	}

	void OpenGLShader::setUniform(const char* uniformName, bool value) const
	{
		ZR_PROFILER_FUNCTION();

		int uniformLocation = getUniformLocation(uniformName);
		if (uniformLocation != -1) {
			GL_ERR_CHECK(glUniform1i(uniformLocation, value));
		}
	}

	void OpenGLShader::setUniform(const char* uniformName, bool valueX, bool valueY) const
	{
		ZR_PROFILER_FUNCTION();

		int uniformLocation = getUniformLocation(uniformName);
		if (uniformLocation != -1) {
			GL_ERR_CHECK(glUniform2i(uniformLocation, valueX, valueY));
		}
	}

	void OpenGLShader::setUniform(const char* uniformName, bool valueX, bool valueY, bool valueZ) const
	{
		ZR_PROFILER_FUNCTION();

		int uniformLocation = getUniformLocation(uniformName);
		if (uniformLocation != -1) {
			GL_ERR_CHECK(glUniform3i(uniformLocation, valueX, valueY, valueZ));
		}
	}

	void OpenGLShader::setUniform(const char* uniformName, bool valueX, bool valueY, bool valueZ, bool valueW) const
	{
		ZR_PROFILER_FUNCTION();

		int uniformLocation = getUniformLocation(uniformName);
		if (uniformLocation != -1) {
			GL_ERR_CHECK(glUniform4i(uniformLocation, valueX, valueY, valueZ, valueW));
		}
	}

	void OpenGLShader::setUniform(const char* uniformName, const glm::vec2& value) const
	{
		ZR_PROFILER_FUNCTION();

		int uniformLocation = getUniformLocation(uniformName);
		if (uniformLocation != -1) {
			GL_ERR_CHECK(glUniform2f(uniformLocation, value.x, value.y));
		}
	}

	void OpenGLShader::setUniform(const char* uniformName, const glm::vec3& value) const
	{
		ZR_PROFILER_FUNCTION();

		int uniformLocation = getUniformLocation(uniformName);
		if (uniformLocation != -1) {
			GL_ERR_CHECK(glUniform3f(uniformLocation, value.x, value.y, value.z));
		}
	}

	void OpenGLShader::setUniform(const char* uniformName, const glm::vec4& value) const
	{
		ZR_PROFILER_FUNCTION();

		int uniformLocation = getUniformLocation(uniformName);
		if (uniformLocation != -1) {
			GL_ERR_CHECK(glUniform4f(uniformLocation, value.x, value.y, value.z, value.w));
		}
	}

	void OpenGLShader::setUniform(const char* uniformName, const glm::mat3& value) const
	{
		ZR_PROFILER_FUNCTION();

		int uniformLocation = getUniformLocation(uniformName);
		if (uniformLocation != -1) {
			GL_ERR_CHECK(glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value)));
		}
	}

	void OpenGLShader::setUniform(const char* uniformName, const glm::mat4& value) const
	{
		ZR_PROFILER_FUNCTION();

		int uniformLocation = getUniformLocation(uniformName);
		if (uniformLocation != -1) {
			GL_ERR_CHECK(glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value)));
		}
	}

	void OpenGLShader::setUniform(const char* uniformName, const Material& value) const
	{
		ZR_PROFILER_FUNCTION();

		std::string name(uniformName);
		setUniform((name + ".Ambient").c_str(), value.Ambient);
		setUniform((name + ".Diffuse").c_str(), value.Diffuse);
		setUniform((name + ".Emissive").c_str(), value.Emissive);
		setUniform((name + ".Specular").c_str(), value.Specular);
		setUniform((name + ".Shininess").c_str(), value.Shininess);
	}

	void OpenGLShader::setUniform(const char* uniformName, const Light& value) const
	{
		ZR_PROFILER_FUNCTION();

		std::string name(uniformName);
		setUniform((name + ".Ambient").c_str(), value.Ambient);
		setUniform((name + ".Diffuse").c_str(), value.Diffuse);
		setUniform((name + ".Specular").c_str(), value.Specular);

		if (value.Type == Light::LightType::Directional) {
			DirectionalLight* light = (DirectionalLight*)& value;
			setUniform((name + ".Direction").c_str(), light->Direction);
			setUniform((name + ".Type").c_str(), Light::LightType::Directional);
		}
	}

	void OpenGLShader::setUniform(const char* uniformName, const std::vector<glm::mat4>& value) const
	{
		ZR_PROFILER_FUNCTION();

		int uniformLocation = getUniformLocation(uniformName);
		if (uniformLocation != -1) {
			GL_ERR_CHECK(glUniformMatrix4fv(uniformLocation, (GLsizei)value.size(), GL_FALSE, glm::value_ptr(value[0])));
		}
	}

	std::string OpenGLShader::readFile(const std::string& filePath)
	{
		ZR_PROFILER_FUNCTION();

		std::string result;
		std::ifstream in(filePath, std::ios::in | std::ios::binary);
		if (in) {
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1) {
				result.resize(in.tellg());
				in.seekg(0, std::ios::beg);
				in.read(&result[0], result.size());
				in.close();
			}
		}
		return result;
	}

	std::unordered_map<unsigned char, std::string> OpenGLShader::preProcess(const std::string& source)
	{
		ZR_PROFILER_FUNCTION();

		std::unordered_map<unsigned char, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos) {
			size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
			size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
			std::string type = source.substr(begin, eol - begin);

			size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
			pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line
			shaderSources[ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	bool OpenGLShader::checkCompilationError(ShaderType shaderType, GLuint shaderId) const
	{
		std::stringstream errorStream;

		int success;
		GL_ERR_CHECK(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success));

		if (shaderType == VertexShader) {
			errorStream << "Vertex ";
		}
		else if (shaderType == FragmentShader) {
			errorStream << "Fragment ";
		}
		else if (shaderType == GeometryShader) {
			errorStream << "Geometry ";
		}

		if (!success) {
			int maxLength = 0;
			GL_ERR_CHECK(glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength));

			std::vector<char> info(maxLength);

			GL_ERR_CHECK(glGetShaderInfoLog(shaderId, maxLength, &maxLength, &info[0]));
			errorStream << "compilation failed:\n" << info.data();
			ZR_CORE_ERROR("[SHADER] {0}", errorStream.str());
			return false;
		}

		return true;
	}

	bool OpenGLShader::createVertexAndFragShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
	{
		std::ifstream vertexShaderFile;
		std::ifstream fragmentShaderFile;

		vertexShaderFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);
		fragmentShaderFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);

		try {
			vertexShaderFile.open(vertexShaderPath.c_str(), std::ios_base::in);
			fragmentShaderFile.open(fragmentShaderPath.c_str(), std::ios_base::in);

			std::stringstream vertexStringStream;
			std::stringstream fragmentStringStream;

			vertexStringStream << vertexShaderFile.rdbuf();
			vertexShaderFile.close();

			fragmentStringStream << fragmentShaderFile.rdbuf();
			fragmentShaderFile.close();

			std::string vertexShaderSource = vertexStringStream.str();
			std::string fragmentShaderSource = fragmentStringStream.str();

			const char* vertexShader = vertexShaderSource.c_str();
			const char* fragmentShader = fragmentShaderSource.c_str();

			// Compile vertex shader and check for errors
			GL_ERR_CHECK(mVertexShaderId = glCreateShader(GL_VERTEX_SHADER));
			GL_ERR_CHECK(glShaderSource(mVertexShaderId, 1, &vertexShader, NULL));
			GL_ERR_CHECK(glCompileShader(mVertexShaderId));
			if (!checkCompilationError(VertexShader, mVertexShaderId)) {
				if (mVertexShaderId) {
					GL_ERR_CHECK(glDeleteShader(mVertexShaderId));
					mVertexShaderId = 0;
				}
				return false;
			}

			// Compile fragment shader and check for errors
			GL_ERR_CHECK(mFragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER));
			GL_ERR_CHECK(glShaderSource(mFragmentShaderId, 1, &fragmentShader, NULL));
			GL_ERR_CHECK(glCompileShader(mFragmentShaderId));
			if (!checkCompilationError(FragmentShader, mFragmentShaderId)) {
				if (mFragmentShaderId) {
					GL_ERR_CHECK(glDeleteShader(mVertexShaderId));
					GL_ERR_CHECK(glDeleteShader(mFragmentShaderId));

					mVertexShaderId = 0;
					mFragmentShaderId = 0;
				}
				return false;
			}
		}
		catch (...) {
			return false;
		}
		return true;
	}

	bool OpenGLShader::createGeometryShader(const std::string& geometryShaderPath)
	{
		std::ifstream geometryShaderFile;

		geometryShaderFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);

		try {
			geometryShaderFile.open(geometryShaderPath.c_str(), std::ios_base::in);

			std::stringstream geometryStringStream;

			geometryStringStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();

			std::string geometryStringSource = geometryStringStream.str();

			const char* geometryShader = geometryStringSource.c_str();

			// Compile vertex shader and check for errors
			GL_ERR_CHECK(mGeometryShaderId = glCreateShader(GL_GEOMETRY_SHADER));
			GL_ERR_CHECK(glShaderSource(mGeometryShaderId, 1, &geometryShader, NULL));
			GL_ERR_CHECK(glCompileShader(mGeometryShaderId));
			if (!checkCompilationError(GeometryShader, mGeometryShaderId)) {
				if (mGeometryShaderId) {
					GL_ERR_CHECK(glDeleteShader(mVertexShaderId));
					GL_ERR_CHECK(glDeleteShader(mFragmentShaderId));
					GL_ERR_CHECK(glDeleteShader(mGeometryShaderId));

					mVertexShaderId = 0;
					mFragmentShaderId = 0;
					mGeometryShaderId = 0;
				}
				return false;
			}
		}
		catch (...) {
			return false;
		}

		return true;
	}

	bool OpenGLShader::createAndLinkProgram(unsigned char whichShaders)
	{
		ZR_PROFILER_FUNCTION();

		if (whichShaders == 0) {
			return false;
		}

		GL_ERR_CHECK(mProgramId = glCreateProgram());
		GL_ERR_CHECK(glAttachShader(mProgramId, mVertexShaderId));
		GL_ERR_CHECK(glAttachShader(mProgramId, mFragmentShaderId));
		if (whichShaders & GeometryShader) {
			GL_ERR_CHECK(glAttachShader(mProgramId, mGeometryShaderId));
		}

		// Attach shaders, link the program and check for errors
		int success;
		GL_ERR_CHECK(glLinkProgram(mProgramId));
		GL_ERR_CHECK(glGetProgramiv(mProgramId, GL_LINK_STATUS, &success));

		if (!success) {
			int maxLength;
			GL_ERR_CHECK(glGetProgramiv(mProgramId, GL_INFO_LOG_LENGTH, &maxLength));
			std::vector<char> info(maxLength);
			GL_ERR_CHECK(glGetProgramInfoLog(mProgramId, 1024, &maxLength, &info[0]));
			GL_ERR_CHECK(glDeleteProgram(mProgramId));
			mProgramId = 0U;

			GL_ERR_CHECK(glDeleteShader(mVertexShaderId));
			GL_ERR_CHECK(glDeleteShader(mFragmentShaderId));
			mVertexShaderId = 0U;
			mFragmentShaderId = 0U;
			if (whichShaders & GeometryShader) {
				GL_ERR_CHECK(glDeleteShader(mGeometryShaderId));
				mGeometryShaderId = 0U;
			}
			return false;
		}

		// Always detach and delete shaders after a successful link.
		GL_ERR_CHECK(glDetachShader(mProgramId, mVertexShaderId));
		GL_ERR_CHECK(glDeleteShader(mVertexShaderId));
		GL_ERR_CHECK(glDetachShader(mProgramId, mFragmentShaderId));
		GL_ERR_CHECK(glDeleteShader(mFragmentShaderId));

		if (whichShaders & GeometryShader) {
			GL_ERR_CHECK(glDetachShader(mProgramId, mGeometryShaderId));
			GL_ERR_CHECK(glDeleteShader(mGeometryShaderId));
		}

		return true;
	}

	int OpenGLShader::getUniformLocation(const char* uniformName) const
	{
		if (mUniformLocationCache.find(uniformName) != mUniformLocationCache.end()) {
			return mUniformLocationCache[uniformName];
		}

		int uniformLocation;
		GL_ERR_CHECK(uniformLocation = glGetUniformLocation(mProgramId, uniformName));
		mUniformLocationCache[uniformName] = uniformLocation;
		return uniformLocation;
	}
}
