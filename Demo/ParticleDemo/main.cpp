#include "ParticleDemoApp.hpp"

int main() {
	auto app = std::make_shared<ParticleDemoApp>("ParticleDemoApp", 1920, 1080);
	
	app->show();
	app->runLoop();
	app.reset();
}