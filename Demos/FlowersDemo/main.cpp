#include "FlowersDemoApp.hpp"

int main() {
	auto app = FlowersDemoApp("FlowersDemoApp", 1920, 1080);

	app.show();
	app.runLoop();
}