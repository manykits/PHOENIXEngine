package org.appplay.lib;

import java.lang.reflect.Method;
import java.net.URI;
import java.nio.ByteBuffer;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Rect;
import android.net.http.SslError;
import android.util.Log;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.webkit.SslErrorHandler;
import android.webkit.WebChromeClient;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.FrameLayout;

public class AppPlayWebView extends WebView
{
    private static final String TAG = "appplay.ap";
    private int viewTag;
    private String jsScheme;
    private String szWebViewRect;
    public byte[] imageData;
    private FrameLayout.LayoutParams layoutParams;
	
    public AppPlayWebView(Context context) 
    {
        this(context, -1);
    }
    
    public AppPlayWebView(Context context, int viewTag)
    {
        super(context);
        
        this.viewTag = viewTag;
        this.jsScheme = "";
        this.szWebViewRect = "0-0-0-0";

        this.setFocusable(true);
        this.setFocusableInTouchMode(true);

        this.getSettings().setSupportZoom(false);
        this.getSettings().setBuiltInZoomControls(true);
        this.getSettings().setJavaScriptEnabled(true);
        this.addJavascriptInterface(new InJavaScriptLocalObj(), "local_obj");
        this.getSettings().setUseWideViewPort(true);
        
        // `searchBoxJavaBridge_` has big security risk. http://jvn.jp/en/jp/JVN53768697
        try 
        {
            Method method = this.getClass().getMethod("removeJavascriptInterface", new Class[]{String.class});
            method.invoke(this, "searchBoxJavaBridge_");
        }
        catch (Exception e) 
        { 
            Log.d(TAG, "This API level do not support `removeJavascriptInterface`");
        }

        this.setWebViewClient(new AppPlayWebViewClient());
        this.setWebChromeClient(new WebChromeClient());
    }
    
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) 
    {
    	if (keyCode == KeyEvent.KEYCODE_BACK) 
    	{
    		return AppPlayGLView.sTheAppPlayGLView.onKeyDown(keyCode, event);
    	}
    	
        return super.onKeyDown(keyCode, event);
    }
    
    public void setJavascriptInterfaceScheme(String scheme) {
        this.jsScheme = scheme != null ? scheme : "";
    }
    
    public void setScalesPageToFit(boolean scalesPageToFit) {
        this.getSettings().setSupportZoom(scalesPageToFit);
    } 
    
    private int imageWidth = 0;
    private int imageHeight = 0;
    private int imageWidthBytes = 0;
    public byte[] getWebViewImage() 
    {   
    	if (mIsPaused)
    	{   		
    		return null;	
    	}
   	
     	Bitmap bmp = getDrawingCache();
    	if (bmp != null)
    	{
    		ByteBuffer buf = ByteBuffer.allocate(bmp.getRowBytes() * bmp.getHeight());
    		bmp.copyPixelsToBuffer(buf);
    		imageData = buf.array();
    		
    		imageWidthBytes = bmp.getRowBytes();
    		imageWidth = bmp.getWidth();
    		imageHeight = bmp.getHeight();
    		
    		return imageData;
    	}

   		return null;
    }
    
    public int getImageWidth()
    {
    	return imageWidth;
    }
    
    public int getImageHeight()
    {
    	return imageHeight;
    }
    
    public int getImageWidthBytes()
    {
    	return imageWidthBytes;
    }
    
	class AppPlayWebViewClient extends WebViewClient 
    {
        @Override
        public boolean shouldOverrideUrlLoading(WebView view, String urlString)
        {
            URI uri = URI.create(urlString);
            if (uri != null && uri.getScheme().equals(jsScheme)) {
            	AppPlayWebViewNatives._onJsCallback(viewTag, urlString);
                return true;
            }
            return AppPlayWebViewNatives._shouldStartLoading(viewTag, urlString);
        }

        @Override
        public void onPageFinished(WebView view, String url) {
            super.onPageFinished(view, url);
            AppPlayWebViewNatives._didFinishLoading(viewTag, url);
        }

        @Override
        public void onReceivedError(WebView view, int errorCode, String description, String failingUrl) {
            super.onReceivedError(view, errorCode, description, failingUrl);
            
            AppPlayWebViewNatives._didFailLoading(viewTag, failingUrl);
        }

		@Override
		public void onReceivedSslError(WebView view, SslErrorHandler handler, SslError error) {
			handler.proceed();
		}
    }
    
	private boolean mIsPaused = false;
	
	@Override
	public void onPause()
	{
		Log.d("appplay.lib", "info - AppPlayWebView::onPause");

		super.onPause();
		
		mIsPaused = true;
	}

	@Override
	public void onResume()
	{
		Log.d("appplay.lib", "info - AppPlayWebView::onResume");
		
		super.onResume();
		
		mIsPaused = false;
	}    
    
    final class InJavaScriptLocalObj {
        public void showSource(String html) {
        	AppPlayWebViewNatives.didLoadHtmlSource(html);
        	AppPlayWebViewNatives.s_bWaitGetHemlSource = false;
        }
    }
    
    public void setWebViewRect(int left, int top, int maxWidth, int maxHeight) {
        fixSize(left, top, maxWidth, maxHeight);
        this.szWebViewRect = String.format("%d-%d-%d-%d", left, top, maxWidth, maxHeight);
    }
    
    public String getWebViewRectString(){
    	return szWebViewRect;
    }
    
    public int getViewTag() {
    	return viewTag;
    }
    
    private void fixSize(int left, int top, int width, int height) 
    {
    	layoutParams = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.MATCH_PARENT,
                FrameLayout.LayoutParams.MATCH_PARENT);
    	layoutParams.gravity = Gravity.LEFT|Gravity.TOP;
    	layoutParams.leftMargin = left;
        layoutParams.topMargin = top;
        layoutParams.width = width;
        layoutParams.height = height;
        this.setLayoutParams(layoutParams);
    }
    
	@Override
	public boolean onTouchEvent(final MotionEvent event)
	{
		Log.d("appplay.lib", "info - AppPlayWebView::onTouchEvent");
		
		return super.onTouchEvent(event);
	}
}
