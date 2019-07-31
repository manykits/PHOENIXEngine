// PX2ServerDBManager.hpp

#ifndef PX2SERVERDBMANAGER_HPP
#define PX2SERVERDBMANAGER_HPP

#include "PX2ServerPre.hpp"
#include "PX2Singleton.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM DBManager
	{
		PX2_SINGLETION(DBManager);

	public:
		DBManager(const char *ip, const char *dbname, const char *user, const char *pwd, 
			char *pshmem, int shmem_len);
	};

}

#endif