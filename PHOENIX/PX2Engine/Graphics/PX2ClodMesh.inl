// PX2ClodMesh.inl

//----------------------------------------------------------------------------
inline int ClodMesh::GetNumRecords () const
{
	return mRecordArray->GetNumRecords();
}
//----------------------------------------------------------------------------
inline int& ClodMesh::TargetRecord ()
{
	return mTargetRecord;
}
//----------------------------------------------------------------------------
inline int ClodMesh::GetAutomatedTargetRecord ()
{
	return mTargetRecord;
}
//----------------------------------------------------------------------------
