// PX2Exception.hpp

#ifndef PX2EXCEPTION_HPP
#define PX2EXCEPTION_HPP

#include "PX2CorePre.hpp"
#include <typeinfo>

namespace PX2
{

	class PX2_ENGINE_ITEM Exception : public std::exception
	{
	public:
		Exception(const std::string& msg, int code = 0);
		Exception(const std::string& msg, const std::string& arg, int code = 0);
		Exception(const std::string& msg, const Exception& nested, int code = 0);
		Exception(const Exception& exc);

		~Exception() throw();

		Exception& operator = (const Exception& exc);

		virtual const char* name() const throw();
		virtual const char* className() const throw();
		virtual const char* what() const throw();

		const Exception* nested() const;
		const std::string& message() const;
		int code() const;

		std::string displayText() const;
		virtual Exception* clone() const;
		virtual void rethrow() const;

	protected:
		Exception(int code = 0);
		void message(const std::string& msg);
		void extendedMessage(const std::string& arg);

	private:
		std::string _msg;
		Exception* _pNested;
		int _code;
	};

#include "PX2Exception.inl"

#define PX2_DECLARE_EXCEPTION_CODE(API, CLS, BASE, CODE) \
	class API CLS: public BASE														\
			{																				\
	public:																			\
		CLS(int code = CODE);														\
		CLS(const std::string& msg, int code = CODE);								\
		CLS(const std::string& msg, const std::string& arg, int code = CODE);		\
		CLS(const std::string& msg, const PX2::Exception& exc, int code = CODE);	\
		CLS(const CLS& exc);														\
		~CLS() throw();																\
		CLS& operator = (const CLS& exc);											\
		const char* name() const throw();											\
		const char* className() const throw();										\
		PX2::Exception* clone() const;												\
		void rethrow() const;														\
			};

#define PX2_DECLARE_EXCEPTION(API, CLS, BASE) \
	PX2_DECLARE_EXCEPTION_CODE(API, CLS, BASE, 0)

#define PX2_IMPLEMENT_EXCEPTION(CLS, BASE, NAME)													\
	CLS::CLS(int code): BASE(code)																	\
			{																								\
			}																								\
	CLS::CLS(const std::string& msg, int code): BASE(msg, code)										\
			{																								\
			}																								\
	CLS::CLS(const std::string& msg, const std::string& arg, int code): BASE(msg, arg, code)		\
			{																								\
			}																								\
	CLS::CLS(const std::string& msg, const PX2::Exception& exc, int code): BASE(msg, exc, code)	\
			{																								\
			}																								\
	CLS::CLS(const CLS& exc): BASE(exc)																\
			{																								\
			}																								\
	CLS::~CLS() throw()																				\
			{																								\
			}																								\
	CLS& CLS::operator = (const CLS& exc)															\
			{																								\
		BASE::operator = (exc);																		\
		return *this;																				\
			}																								\
	const char* CLS::name() const throw()															\
			{																								\
		return NAME;																				\
			}																								\
	const char* CLS::className() const throw()														\
			{																								\
		return typeid(*this).name();																\
			}																								\
	PX2::Exception* CLS::clone() const																\
			{																								\
		return new CLS(*this);																		\
			}																								\
	void CLS::rethrow() const																		\
			{																								\
		throw *this;																				\
			}


	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, RuntimeException, Exception)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, NotFoundException, RuntimeException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, TimeoutException, RuntimeException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, IOException, RuntimeException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, DataException, RuntimeException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, DataFormatException, DataException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, SyntaxException, DataException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, OutOfMemoryException, RuntimeException)

	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, URISyntaxException, SyntaxException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, PathSyntaxException, SyntaxException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, FileException, IOException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, FileExistsException, FileException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, FileNotFoundException, FileException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, PathNotFoundException, FileException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, FileReadOnlyException, FileException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, FileAccessDeniedException, FileException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, CreateFileException, FileException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, OpenFileException, FileException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, WriteFileException, FileException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, ReadFileException, FileException)

}

#endif