package org.appplay.lib;

import android.content.Context;
import android.util.AttributeSet;
import android.view.KeyEvent;
import android.widget.EditText;

public class AppPlayEditText extends EditText
{
	
	private AppPlayGLView mView;

	public AppPlayEditText(Context context)
	{
		super(context);
	}
	
	public AppPlayEditText (Context context, AttributeSet attrs) 
	{
		super(context, attrs);
	}
	
	public AppPlayEditText (Context context, AttributeSet attrs, int defStyle) 
	{
		super(context, attrs, defStyle);
	}
	
	public void SetGLView(AppPlayGLView view)
	{
		mView = view;
	}
	
	public boolean onKeyDown (int keyCode, KeyEvent event) 
	{
		super.onKeyDown(keyCode, event);
		
		/* Let GlSurfaceView get focus if back key is input. */
		if (keyCode == KeyEvent.KEYCODE_BACK) 
		{
			mView.requestFocus();
		}
		
		return true;
	}
}


