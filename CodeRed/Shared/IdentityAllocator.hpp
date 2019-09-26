#pragma once

#include <stack>

namespace CodeRed {

	template<typename Container,typename Identity = size_t>
	class IdentityAllocator {
	public:
		IdentityAllocator() = default;

		~IdentityAllocator() = default;

		void reset();
		
		auto allocate() -> Identity;

		auto free(const Identity& identity);

		auto container() -> Container&;
	private:
		std::stack<Identity> mFreeIdentities;
		Container mContainer;
	};

	template <typename Container, typename Identity>
	void IdentityAllocator<Container, Identity>::reset()
	{
		mContainer.clear();
		mFreeIdentities = std::stack<Identity>();
	}

	template <typename Container, typename Identity>
	auto IdentityAllocator<Container, Identity>::allocate() -> Identity
	{
		if (mFreeIdentities.empty()) return mContainer.size();

		const auto identity = mFreeIdentities.top(); mFreeIdentities.pop();

		return identity;
	}

	template <typename Container, typename Identity>
	auto IdentityAllocator<Container, Identity>::free(const Identity& identity)
	{
		mFreeIdentities.push(identity);
	}

	template <typename Container, typename Identity>
	auto IdentityAllocator<Container, Identity>::container() -> Container& 
	{
		return mContainer;
	}

}
