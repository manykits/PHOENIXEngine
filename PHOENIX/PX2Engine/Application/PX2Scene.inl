// PX2Scene.inl

//----------------------------------------------------------------------------
inline EnvirParamController *Scene::GetEnvirParamController()
{
	return mEnvirParam;
}
//----------------------------------------------------------------------------
inline AmbientRegionController *Scene::GetAmbientRegionController()
{
	return mAmbientRegionController;
}
//----------------------------------------------------------------------------
inline std::vector<CameraPtr> &Scene::GetCameras()
{
	return mCameras;
}
//----------------------------------------------------------------------------
inline Node *Scene::GetPathRoot()
{
	return mPathRoot;
}
//----------------------------------------------------------------------------
inline Node *Scene::GetAreaRoot()
{
	return mAreaRoot;
}
//----------------------------------------------------------------------------
inline AIAgentWorld *Scene::GetAIAgentWorld()
{
	return mAIAgentWorld;
}
//----------------------------------------------------------------------------