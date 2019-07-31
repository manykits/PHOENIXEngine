// AppPlayer.hpp

#ifndef APPPLAYERAPP_HPP
#define APPPLAYERAPP_HPP

#include "PX2App.hpp"
using namespace PX2;

class AppPlayerApp : public App
{
public:
	PX2_DECLARE_APPLICATION(AppPlayerApp);
	
	AppPlayerApp ();
	virtual ~AppPlayerApp ();

	virtual bool Initlize();
};

PX2_REGISTER_APPLICATION(AppPlayerApp)

#endif