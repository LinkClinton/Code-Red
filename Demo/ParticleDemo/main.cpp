#include "ParticleDemoApp.hpp"

int main() {
	auto app = ParticleDemoApp("ParticleDemoApp", 1920, 1080);
	
	app.show();
	app.runLoop();
}