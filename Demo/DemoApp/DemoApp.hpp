#pragma once

#include <Core/CodeRedGraphics.hpp>

#include <Windows.h>

#include <string>
#include <chrono>

namespace Demo {

	using Time = std::chrono::high_resolution_clock;
	
	class DemoApp : public CodeRed::Noncopyable {
	public:
		explicit DemoApp(
			const std::string& name,
			const size_t width,
			const size_t height);

		~DemoApp() = default;
		
		void show() const;

		void hide() const;
		
		void runLoop();

		auto width() const noexcept -> size_t { return mWidth; }

		auto height() const noexcept -> size_t { return mHeight; }

		auto name() const noexcept -> std::string { return mName; }

		auto handle() const noexcept -> void* { return mHwnd; }
	protected:
		virtual void update(float delta) {}
		virtual void render(float delta) {}
		virtual void initialize() {}
		virtual void finalize() {}
	private:
		std::string mName;

		size_t mWidth;
		size_t mHeight;
		HWND mHwnd;

		bool mExisted;

		static void processMessage(DemoApp* app, const MSG& message);
	};
	
}