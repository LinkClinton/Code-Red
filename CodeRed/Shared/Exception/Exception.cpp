#include "Exception.hpp"

#include "../DebugReport.hpp"
#include "FailedException.hpp"
#include "NotSupportException.hpp"

CodeRed::Exception::Exception(const std::string& message)
	: exception(), mMesaage(message)
{
#ifdef __ENABLE__CODE__RED__DEBUG__
	DebugReport::output(message);
#endif
}

char const* CodeRed::Exception::what() const
{
	return mMesaage.c_str();
}

CodeRed::FailedException::FailedException(
	const DebugType type, 
	const std::vector<std::string>& messages) :
	Exception(DebugReport::makeError(type, messages))
{
	
}

CodeRed::FailedException::FailedException(
	const DebugType type, 
	const std::vector<std::string>& messages,
	const std::vector<std::string>& debug_message) :
	Exception(DebugReport::makeError(type, messages, debug_message))
{
}

CodeRed::NotSupportException::NotSupportException(const NotSupportType type) :
	Exception(DebugReport::makeError(select(type)))
{
}

auto CodeRed::NotSupportException::select(const NotSupportType type)
	-> std::string
{
	switch (type) {
	case NotSupportType::Any: return "we do not support it.";
	case NotSupportType::Enum: return "we do not support this enum.";
	case NotSupportType::Method: return "we do not support this method.";
	case NotSupportType::Object: return "we do not support this object.";
	default: return "the type is invalid.";
	}
}
