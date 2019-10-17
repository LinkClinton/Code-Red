#include "TriangleDemoApp.hpp"

int main() {
	auto app = TriangleDemoApp("TriangleDemoApp", 1920, 1080);
	
	app.show();
	app.runLoop();
}