#pragma once

#if defined(_MSC_VER) && !defined(NDEBUG)
#define GL_ERR_CHECK(func)\
{\
	while(glGetError() != GL_NO_ERROR);\
	func;\
	GLenum error = glGetError();\
	if(error != GL_NO_ERROR){\
		switch(error){\
			case GL_INVALID_ENUM:\
			{\
				std::cout << "[OpenGL Error] (GL_INVALID_ENUM - " << std::setbase(16) << error << ") " << __FILE__ << ": " << std::setbase(10) << __LINE__ << "\n"; break;\
			}\
			case GL_INVALID_VALUE:\
			{\
				std::cout << "[OpenGL Error] (GL_INVALID_VALUE - " << std::setbase(16) << error << ") " << __FILE__ << ": " << std::setbase(10) << __LINE__ << "\n"; break;\
			}\
			case GL_INVALID_OPERATION:\
			{\
				std::cout << "[OpenGL Error] (GL_INVALID_OPERATION - " << std::setbase(16) << error << ") " << __FILE__ << ": " << std::setbase(10) << __LINE__ << "\n"; break;\
			}\
			case GL_INVALID_FRAMEBUFFER_OPERATION:\
			{\
				std::cout << "[OpenGL Error] (GL_INVALID_FRAMEBUFFER_OPERATION - " << std::setbase(16) << error << ") " << __FILE__ << ": " << std::setbase(10) << __LINE__ << "\n"; break;\
			}\
			case GL_OUT_OF_MEMORY:\
			{\
				std::cout << "[OpenGL Error] (GL_OUT_OF_MEMORY - " << std::setbase(16) << error << ") " << __FILE__ << ": " << std::setbase(10) << __LINE__ << "\n"; break;\
			}\
			default:\
			{\
				std::cout << "[OpenGL Error] (" << std::setbase(16) << error << ") " << __FILE__ << ": " << std::setbase(10) << __LINE__ << "\n"; break;\
			}\
		}\
		__debugbreak();\
	}\
}\

#elif !defined(NDEBUG)
#define GL_ERR_CHECK(func)\
{\
	while(glGetError() != GL_NO_ERROR);\
	func;\
	GLenum error = glGetError();\
	if(error != GL_NO_ERROR){\
		switch(error){\
			case GL_INVALID_ENUM:\
			{\
				std::cout << "[OpenGL Error] (GL_INVALID_ENUM - " << std::setbase(16) << error << ") " << __FILE__ << ": " << std::setbase(10) << __LINE__ << "\n"; break;\
			}\
			case GL_INVALID_VALUE:\
			{\
				std::cout << "[OpenGL Error] (GL_INVALID_VALUE - " << std::setbase(16) << error << ") " << __FILE__ << ": " << std::setbase(10) << __LINE__ << "\n"; break;\
			}\
			case GL_INVALID_OPERATION:\
			{\
				std::cout << "[OpenGL Error] (GL_INVALID_OPERATION - " << std::setbase(16) << error << ") " << __FILE__ << ": " << std::setbase(10) << __LINE__ << "\n"; break;\
			}\
			case GL_INVALID_FRAMEBUFFER_OPERATION:\
			{\
				std::cout << "[OpenGL Error] (GL_INVALID_FRAMEBUFFER_OPERATION - " << std::setbase(16) << error << ") " << __FILE__ << ": " << std::setbase(10) << __LINE__ << "\n"; break;\
			}\
			case GL_OUT_OF_MEMORY:\
			{\
				std::cout << "[OpenGL Error] (GL_OUT_OF_MEMORY - " << std::setbase(16) << error << ") " << __FILE__ << ": " << std::setbase(10) << __LINE__ << "\n"; break;\
			}\
			default:\
			{\
				std::cout << "[OpenGL Error] (" << std::setbase(16) << error << ") " << __FILE__ << ": " << std::setbase(10) << __LINE__ << "\n"; break;\
			}\
		}\
	}\
}\

#else
#define GL_ERR_CHECK(func)\
{\
	func;\
}\

#endif