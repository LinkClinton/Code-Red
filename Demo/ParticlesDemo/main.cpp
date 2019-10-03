#include "ParticlesDemoApp.hpp"

int main() {
	auto app = ParticlesDemoApp("ParticlesDemoApp", 1920, 1080);
	
	app.show();
	app.runLoop();
}