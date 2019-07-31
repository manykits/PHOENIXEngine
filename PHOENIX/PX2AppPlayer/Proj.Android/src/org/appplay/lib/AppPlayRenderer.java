package org.appplay.lib;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import org.appplay.ai.VoiceSDK;

import android.opengl.GLSurfaceView;
import android.util.Log;

class AppPlayRenderer implements GLSurfaceView.Renderer
{
    private AppPlayBaseActivity mActivity;
	private int mWidth;
	private int mHeight;

    public AppPlayRenderer(AppPlayBaseActivity activity)
    {
        mActivity = activity;
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config)
    { 
    	Log.d("appplay.lib", "begin - surface created, navtiveInit.");
    	
    	Log.d("appplay.lib", "AppPlayRenderer.onSurfaceCreated" + mWidth + " " + mHeight);
    	
    	AppPlayNatives.nativeInit(mWidth, mHeight);    	
		AppPlayNatives.nativeVoiceSetSDK(VoiceSDK.sPlatformStr);
    	
    	Log.d("appplay.lib", "end - nativeInit.");
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) 
    {
    	Log.d("appplay.lib", "AppPlayRenderer.onSurfaceChanged" + width + " " + height);
    	 
    	AppPlayNatives.nativeInit(mWidth, mHeight); 
    } 

    @Override
    public void onDrawFrame(GL10 gl)
    {
    	// draw
    	AppPlayNatives.nativeOdle(); 	
    }
    
    // -- funcions implemented ourself -- 
    
	public void SetSize(int w, int h)
    {
    	this.mWidth = w;
    	this.mHeight = h;
    	
    	Log.d("appplay.lib", "AppPlayRenderer.SetSize" + mWidth + " " + mHeight);
    }
    
}