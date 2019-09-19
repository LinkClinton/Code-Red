#pragma once

#include <Core/CodeRedGraphics.hpp>

#include <Windows.h>
#include <string>

namespace Demo {

	class DemoApp : public CodeRed::Noncopyable {
	public:
		explicit DemoApp(
			const std::string& name,
			const size_t width,
			const size_t height);

		~DemoApp() = default;
		
		void show();

		void hide();

		void runLoop();

		auto width() const noexcept -> size_t { return mWidth; }

		auto height() const noexcept -> size_t { return mHeight; }

		auto name() const noexcept -> std::string { return mName; }
	protected:
		virtual void update() {}
		virtual void render() {}
		virtual void initialize() {}
	protected:
		std::string mName;

		size_t mWidth;
		size_t mHeight;
		HWND mHwnd;

		bool mExisted;

		static void processMessage(DemoApp* app, const MSG& message);
	};
	
}