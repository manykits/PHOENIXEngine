// PX2User.hpp

#ifndef PX2USER_HPP
#define PX2USER_HPP

#include "PX2UnityPre.hpp"
#include "PX2Object.hpp"
#include "PX2UserActor.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM User : public Object
	{
	public:
		User();
		virtual ~User();

		virtual void Update(float appTime, float elapsedTime);

		void SetUserName(const std::string &useName);
		const std::string &GetUserName() const;

		void AddUserActor(UserActor *userActor);
		void DeleteUserActor(UserActor *userActor);
		UserActor *GetUserActor(int index) const;
		UserActor *GetUserActorByName(const std::string &useName) const;
		int GetNumUserActors() const;

	protected:
		std::string mName;
		std::string mUserName;
		std::vector<UserActorPtr> mUserActors;
	};

	typedef Pointer0<User> UserPtr;

}

#endif