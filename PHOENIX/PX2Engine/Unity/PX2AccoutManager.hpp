// PX2AccoutManager.hpp

#ifndef PX2ACCOUTMANAGER_HPP
#define PX2ACCOUTMANAGER_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2User.hpp"
#include "PX2Singleton_NeedNew.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM AccoutManager : public Singleton<AccoutManager>
	{
	public:
		AccoutManager();
		~AccoutManager();

		void Update(float appTime, float elapsedTime);

		// users
		void AddUser(User *user);
		void DeleteUser(User *user);
		User *GetUser(const int i) const;
		User *GetUser(const std::string &name) const;
		void DeleteAllUsers();

	protected:
		std::vector<UserPtr> mUsers;
	};

#define PX2_AM AccoutManager::GetSingleton()

}

#endif