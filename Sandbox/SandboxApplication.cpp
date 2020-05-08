#include <pch.h>

#include "SandboxLayer.h"
#include "HUDLayer.h"
#include "SandboxApplication.h"
#include "SandboxLuaLayer.hpp"

#include "Zero/Zero/Core/EntryPoint.h"

#include "Zero/Zero/ImageWriter.h"

namespace lp
{
	SandboxApplication::SandboxApplication() :
		zr::Application()
	{
		ZR_PROFILER_FUNCTION();

		/*glm::uvec4 white(255U, 255U, 255U, 255U);
		glm::uvec4 yellow(255U, 255U, 0U, 255U);
		glm::uvec4 orange(255U, 150U, 0U, 255U);
		glm::uvec4 black(0U, 0U, 0U, 255U);


		const char* const logo[] =
		{
			"0000000000000000",
			"0..............0",
			"0..............0",
			"0....000000....0",
			"0...00....00...0",
			"0...00...000...0",
			"0...00..0000...0",
			"0...00.00000...0",
			"0...00000.00...0",
			"0...0000..00...0",
			"0...000...00...0",
			"0...00....00...0",
			"0....000000....0",
			"0..............0",
			"0..............0",
			"0000000000000000"
		};

		zr::Image image;
		image.create(16U, 16U, glm::uvec4(255U, 255U, 255U, 0U));
		for (unsigned y = 0; y < 16; ++y) {
			for (unsigned x = 0; x < 16; ++x) {
				if (logo[y][x] == '0') {
					image.setPixel(x, y, black);
				}
				else if(logo[y][x] == '1'){
					image.setPixel(x, y, orange);
				}
				else if (logo[y][x] == '2') {
					image.setPixel(x, y, yellow);
				}
				else {
					image.setPixel(x, y, glm::uvec4(255U, 255U, 255U, 0U));
				}
			}
		}*/

		//zr::ImageWriter::LoadDataIntoFile("logo.png", image);

		//zr::Application::GetWindow().setIcon("resources/textures/Checkerboard.png");
		//zr::Application::GetWindow().setIcon("resources/textures/ChernoLogo.png");
		zr::Application::GetWindow().setIcon("resources/textures/logo.png");
		//pushLayer(new SandboxLayer);
		pushLayer(new HUDLayer);
		//pushLayer(new SandboxLuaLayer);
	}

	SandboxApplication::~SandboxApplication()
	{
		ZR_PROFILER_FUNCTION();
	}
}

zr::Application* zr::createApplication()
{
	return new lp::SandboxApplication;
}
