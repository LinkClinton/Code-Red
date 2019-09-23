#include "NotSupportException.hpp"
#include "FailedException.hpp"
#include "Exception.hpp"

#include "../DebugReport.hpp"

CodeRed::Exception::Exception(const std::string& message)
	: exception(message.c_str()), mMesaage(message)
{
#ifdef __ENABLE__CODE__RED__DEBUG__
	DebugReport::error(mMesaage);
#endif
}

char const* CodeRed::Exception::what() const
{
	return mMesaage.c_str();
}

CodeRed::FailedException::FailedException(
	const std::vector<std::string>& messages, 
	const Type type) :
	Exception(
		DebugReport::push(DebugReport::select(type), messages)) {}

CodeRed::FailedException::FailedException(
	const std::vector<std::string>& messages, 
	const std::string& debugMessage,
	const Type type) :
	Exception(
		DebugReport::push(DebugReport::select(type), messages) 
		+ "\nDebug Message : " + debugMessage)
{
	
}

CodeRed::NotSupportException::NotSupportException(const Type type) :
	Exception(select(type)) {}

auto CodeRed::NotSupportException::select(const Type type) -> const char* 
{
	switch (type) {
	case Type::Any: return "We do not support this.";
	case Type::Method: return "We do not support this method.";
	case Type::Object: return "We do not support this object";
	case Type::Enum: return "We do not support this enum.";
	default:
		throw NotSupportException(Type::Enum);
	}
}