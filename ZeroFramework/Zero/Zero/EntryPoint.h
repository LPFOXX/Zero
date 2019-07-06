#pragma once

extern zr::Application* zr::createApplication();

int main(int argc, char** argv)
{
	zr::Application* app = zr::createApplication();
	app->run();
	delete app;
	return 0;
}

