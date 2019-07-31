// PX2DigestEngine.hpp

#ifndef PX2DIGESTENGINE_HPP
#define PX2DIGESTENGINE_HPP

#include "PX2CorePre.hpp"

namespace PX2
{


	class PX2_ENGINE_ITEM DigestEngine
	{
	public:
		typedef std::vector<unsigned char> Digest;

		DigestEngine();
		virtual ~DigestEngine();

		void update(const void* data, std::size_t length);
		void update(char data);
		void update(const std::string& data);

		virtual std::size_t digestLength() const = 0;

		virtual void reset() = 0;

		virtual const Digest& digest() = 0;

		static std::string digestToHex(const Digest& bytes);
		static Digest digestFromHex(const std::string& digest);
		static bool constantTimeEquals(const Digest& d1, const Digest& d2);

	protected:
		virtual void updateImpl(const void* data, std::size_t length) = 0;

	private:
		DigestEngine(const DigestEngine&);
		DigestEngine& operator = (const DigestEngine&);
	};

	inline void DigestEngine::update(const void* data, std::size_t length)
	{
		updateImpl(data, length);
	}

	inline void DigestEngine::update(char data)
	{
		updateImpl(&data, 1);
	}

	inline void DigestEngine::update(const std::string& data)
	{
		updateImpl(data.data(), data.size());
	}


}

#endif
