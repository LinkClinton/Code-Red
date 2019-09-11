#pragma once

namespace CodeRed {

	class Noncopyable {
	public:
		Noncopyable() = default;
		virtual ~Noncopyable() = default;

		Noncopyable(const Noncopyable&) = delete;
		Noncopyable(Noncopyable&&) noexcept = default;

		Noncopyable& operator=(const Noncopyable&) = delete;
		Noncopyable& operator=(Noncopyable&&) = default;
	};
	
}