// PX2RenderWindow.inl

//----------------------------------------------------------------------------
inline Renderer *RenderWindow::GetRenderer()
{
	return mRenderer;
}
//----------------------------------------------------------------------------
inline const APoint &RenderWindow::GetPosition() const
{
	return mPosition;
}
//----------------------------------------------------------------------------
inline const Sizef& RenderWindow::GetScreenSize() const
{
	return mScreenSize;
}
//----------------------------------------------------------------------------
inline bool RenderWindow::IsMaxSize() const
{
	return mIsMaxSize;
}
//----------------------------------------------------------------------------
inline bool RenderWindow::IsShow() const
{
	return mIsShow;
}
//----------------------------------------------------------------------------
inline bool RenderWindow::IsCaptureMouse() const
{
	return mIsCaptureMouse;
}
//----------------------------------------------------------------------------
inline RenderWindow *RenderWindow::GetParentRenderWindow()
{
	return mParentRenderWindow;
}
//----------------------------------------------------------------------------