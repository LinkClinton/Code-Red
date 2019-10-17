#include "EffectPassDemoApp.hpp"

int main() {
	auto app = EffectPassDemoApp("EffectPassDemoApp", 1920, 1080);

	app.show();
	app.runLoop();
}