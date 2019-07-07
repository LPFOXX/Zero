#pragma once

extern zr::Application* zr::createApplication();

int main(int argc, char** argv)
{
	std::unique_ptr<zr::Application> app = std::unique_ptr<zr::Application>(zr::createApplication());
	app->run();
	return 0;
}

