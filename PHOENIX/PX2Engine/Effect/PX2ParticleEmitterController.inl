// PX2ParticleEmitterController.inl

//----------------------------------------------------------------------------
inline int ParticleEmitterController::GetMaxNumParticles () const
{
	return mMaxNumParticles;
}
//----------------------------------------------------------------------------
inline TRecyclingArray<ParticleObject>* &ParticleEmitterController::GetArray ()
{
	return mParticleArray;
}
//----------------------------------------------------------------------------