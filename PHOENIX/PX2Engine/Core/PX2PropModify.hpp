// PX2PropModify.hpp

#ifndef PX2PROPMODIFY_HPP
#define PX2PROPMODIFY_HPP

#include "PX2CorePre.hpp"
#include "PX2Log.hpp"

namespace PX2
{

#define PX2_DECLARE_PM_I(propName) \
public: \
	void Set##propName (int val); \
	int Get##propName () const; \
	bool IsHas##propName (const std::string &factorName); \
	void AddM##propName (const std::string &factorName, int val); \
	int GetM##propName (const std::string &factorName); \
	void RemoveM##propName (const std::string &factorName); \
	std::map<std::string, int> &GetMMap##propName (); \
	void CalM##propName (); \
	int GetM##propName () const; \
	virtual void OnCalM##propName (); \
private: \
	int m##propName; \
	std::map<std::string, int> mMMap##propName; \
	int mM##propName

#define PX2_INIT_PM_I(propName) \
	m##propName = 0; \
	mM##propName = 0

#define PX2_IMPLEMENT_PM_I(className, propName) \
	void className::Set##propName (int val) \
	{ \
		m##propName = val; \
		\
		CalM##propName (); \
	} \
	int className::Get##propName () const \
	{ \
		return m##propName; \
	} \
	bool className::IsHas##propName (const std::string &factorName) \
	{ \
		std::map<std::string, int>::iterator it = mMMap##propName.find(factorName); \
		return mMMap##propName.end() != it; \
	} \
	void className::AddM##propName (const std::string &factorName, int val) \
	{ \
		mMMap##propName[factorName] = val; \
		\
		CalM##propName (); \
	} \
	int className::GetM##propName (const std::string &factorName) \
	{ \
		std::map<std::string, int>::iterator it = mMMap##propName.find(factorName); \
		if (mMMap##propName.end() != it) \
		{ \
			return it->second; \
		} \
		else \
		{ \
			assertion(false, "%s does not exist.\n", factorName.c_str()); \
			PX2_LOG_ERROR("%s does not exist.", factorName.c_str()); \
		} \
		\
		return 0; \
	} \
	void className::RemoveM##propName (const std::string &factorName) \
	{ \
		std::map<std::string, int>::iterator it = mMMap##propName.find(factorName); \
		if (it != mMMap##propName.end()) \
		{ \
			mMMap##propName.erase(it); \
		} \
		\
		CalM##propName (); \
	} \
	std::map<std::string, int> &className::GetMMap##propName () \
	{ \
		return mMMap##propName;\
	} \
	void className::CalM##propName () \
	{ \
		mM##propName = Get##propName (); \
		\
		std::map<std::string, int>::iterator it = mMMap##propName.begin(); \
		for (; it!=mMMap##propName.end(); it++) \
		{ \
			mM##propName += it->second; \
		} \
		\
		OnCalM##propName (); \
		\
	} \
	int className::GetM##propName () const \
	{ \
		return mM##propName; \
	} \
	void className::OnCalM##propName () \
	{ \
	}

	#define PX2_IMPLEMENT1_PM_I(className, propName) \
	void className::Set##propName (int val) \
	{ \
		m##propName = val; \
		\
		CalM##propName (); \
	} \
	int className::Get##propName () const \
	{ \
		return m##propName; \
	} \
	bool className::IsHas##propName (const std::string &factorName) \
	{ \
		std::map<std::string, int>::iterator it = mMMap##propName.find(factorName); \
		return mMMap##propName.end() != it; \
	} \
	void className::AddM##propName (const std::string &factorName, int val) \
	{ \
		mMMap##propName[factorName] = val; \
		\
		CalM##propName (); \
	} \
	int className::GetM##propName (const std::string &factorName) \
	{ \
		std::map<std::string, int>::iterator it = mMMap##propName.find(factorName); \
		if (mMMap##propName.end() != it) \
		{ \
			return it->second; \
		} \
		else \
		{ \
			assertion(false, "%s does not exist.\n", factorName.c_str()); \
			PX2_LOG_ERROR("%s does not exist.", factorName.c_str()); \
		} \
		\
		return 0; \
	} \
	void className::RemoveM##propName (const std::string &factorName) \
	{ \
		std::map<std::string, int>::iterator it = mMMap##propName.find(factorName); \
		if (it != mMMap##propName.end()) \
		{ \
			mMMap##propName.erase(it); \
		} \
		\
		CalM##propName (); \
	} \
	std::map<std::string, int> &className::GetMMap##propName () \
	{ \
		return mMMap##propName;\
	} \
	void className::CalM##propName () \
	{ \
		mM##propName = Get##propName (); \
		\
		std::map<std::string, int>::iterator it = mMMap##propName.begin(); \
		for (; it!=mMMap##propName.end(); it++) \
		{ \
			mM##propName += it->second; \
		} \
		\
			OnCalM##propName (); \
		\
	} \
	int className::GetM##propName () const \
	{ \
		return mM##propName; \
	}

#define PX2_DECLARE_PM_F(propName) \
public: \
	void Set##propName (float val); \
	float Get##propName () const; \
	bool IsHas##propName (const std::string &factorName); \
	void AddM##propName (const std::string &factorName, float val); \
	float GetM##propName (const std::string &factorName); \
	void RemoveM##propName (const std::string &factorName); \
	std::map<std::string, float> &GetMMap##propName (); \
	void CalM##propName (); \
	float GetM##propName () const; \
	virtual void OnCalM##propName (); \
private: \
	float m##propName; \
	std::map<std::string, float> mMMap##propName; \
	float mM##propName

#define PX2_INIT_PM_F(propName) \
	m##propName = 0.0f; \
	mM##propName = 0.0f

#define PX2_IMPLEMENT_PM_F(className, propName) \
	void className::Set##propName (float val) \
	{ \
		m##propName = val; \
		\
		CalM##propName (); \
	} \
	float className::Get##propName () const \
	{ \
		return m##propName; \
	} \
	bool className::IsHas##propName (const std::string &factorName) \
	{ \
		std::map<std::string, float>::iterator it = mMMap##propName.find(factorName); \
		return mMMap##propName.end() != it; \
	} \
	void className::AddM##propName (const std::string &factorName, float val) \
	{ \
		 mMMap##propName[factorName] = val; \
		 \
		 CalM##propName (); \
	} \
	float className::GetM##propName (const std::string &factorName) \
	{ \
		std::map<std::string, float>::iterator it = mMMap##propName.find(factorName); \
		if (mMMap##propName.end() != it) \
		{ \
			return it->second; \
		} \
		else \
		{ \
			assertion(false, "%s does not exist.\n", factorName.c_str()); \
			PX2_LOG_ERROR("%s does not exist.", factorName.c_str()); \
		} \
		\
		return 0.0f; \
	} \
	void className::RemoveM##propName (const std::string &factorName) \
	{ \
		std::map<std::string, float>::iterator it = mMMap##propName.find(factorName); \
		if (it != mMMap##propName.end()) \
		{ \
			mMMap##propName.erase(it); \
		} \
		\
		CalM##propName (); \
	} \
	std::map<std::string, float> &className::GetMMap##propName () \
	{ \
		return mMMap##propName;\
	} \
	void className::CalM##propName () \
	{ \
		mM##propName = Get##propName (); \
		\
		std::map<std::string, float>::iterator it = mMMap##propName.begin(); \
		for (; it!=mMMap##propName.end(); it++) \
		{ \
			mM##propName += it->second; \
		} \
		\
			OnCalM##propName (); \
		\
	} \
	float className::GetM##propName () const \
	{ \
		return mM##propName; \
	} \
	void className::OnCalM##propName () \
	{ \
	}

#define PX2_IMPLEMENT1_PM_F(className, propName) \
	void className::Set##propName (float val) \
	{ \
		m##propName = val; \
		\
		CalM##propName (); \
	} \
	float className::Get##propName () const \
	{ \
		return m##propName; \
	} \
	bool className::IsHas##propName (const std::string &factorName) \
	{ \
		std::map<std::string, float>::iterator it = mMMap##propName.find(factorName); \
		return mMMap##propName.end() != it; \
	} \
	void className::AddM##propName (const std::string &factorName, float val) \
	{ \
		mMMap##propName[factorName] = val; \
		\
		CalM##propName (); \
	} \
	float className::GetM##propName (const std::string &factorName) \
	{ \
		std::map<std::string, float>::iterator it = mMMap##propName.find(factorName); \
		if (mMMap##propName.end() != it) \
		{ \
			return it->second; \
		} \
		else \
		{ \
			assertion(false, "%s does not exist.\n", factorName.c_str()); \
			PX2_LOG_ERROR("%s does not exist.", factorName.c_str()); \
		} \
		\
		return 0.0f; \
	} \
	void className::RemoveM##propName (const std::string &factorName) \
	{ \
		std::map<std::string, float>::iterator it = mMMap##propName.find(factorName); \
		if (it != mMMap##propName.end()) \
		{ \
			mMMap##propName.erase(it); \
		} \
		\
		CalM##propName (); \
	} \
	std::map<std::string, float> &className::GetMMap##propName () \
	{ \
		return mMMap##propName;\
	} \
	void className::CalM##propName () \
	{ \
		mM##propName = Get##propName (); \
		\
		std::map<std::string, float>::iterator it = mMMap##propName.begin(); \
		for (; it!=mMMap##propName.end(); it++) \
		{ \
			mM##propName += it->second; \
		} \
		\
			OnCalM##propName (); \
		\
	} \
	float className::GetM##propName () const \
	{ \
		return mM##propName; \
	}

}

#endif