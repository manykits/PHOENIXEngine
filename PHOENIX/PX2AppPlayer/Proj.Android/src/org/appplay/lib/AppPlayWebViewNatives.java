package org.appplay.lib;

import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.FutureTask;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.util.SparseArray;
import android.view.View;
import android.widget.FrameLayout;

public class AppPlayWebViewNatives 
{
    private static SparseArray<AppPlayWebView> webViews;
    private static int viewTag = 0;
    
    private static final String TAG = "appplay.ap";
    private static Handler handler;
    private static AppPlayBaseActivity TheActivity;
    private static FrameLayout sLayout;

	
    public AppPlayWebViewNatives(FrameLayout layout)
    {
		Log.d("appplay.ap", "AppPlayWebViewNatives create");
    	
    	AppPlayWebViewNatives.sLayout = layout;
    	AppPlayWebViewNatives.handler = new Handler(Looper.myLooper());

    	AppPlayWebViewNatives.TheActivity = (AppPlayBaseActivity) AppPlayBaseActivity.sTheActivity;
    	AppPlayWebViewNatives.webViews = new SparseArray<AppPlayWebView>();
    }
    
    static boolean sbIsPause = false;
    
    public void pauseAll()
    {
    	for (int i=0; i<webViews.size(); i++)
    	{
    		AppPlayWebView view = webViews.get(i);
    		if (null != view)
    		{
    			view.onPause();
    		}
    	}    	
    	
    	sbIsPause = true;
    }
    
    public void resumeAll()
    {       	  	
    	for (int i=0; i<webViews.size(); i++)
    	{
    		AppPlayWebView view = webViews.get(i);
    		if (null != view)
    		{
    			view.onResume();
    		}
    	}
    	
    	sbIsPause = false;
    }
    
    private static native boolean shouldStartLoading(int index, String message);
    public static boolean _shouldStartLoading(int index, String message) {
        return !shouldStartLoading(index, message);
    }
    
    private static native void didFinishLoading(int index, String message);
    public static void _didFinishLoading(int index, String message) {
        didFinishLoading(index, message);
    }
    
    private static native void didFailLoading(int index, String message);
    public static void _didFailLoading(int index, String message) {
        didFailLoading(index, message);
    }
    
    private static native void onJsCallback(int index, String message);
    public static void _onJsCallback(int index, String message) {
        onJsCallback(index, message);
    }
    
    private static native void onSetByteArrayBuffer(int index, byte[] buf, int len);
    public static native void didLoadHtmlSource(String htmlSrc);
    public static Boolean s_bWaitGetHemlSource = false;
    public static native void pause();
    public static native void resume();
    
    public static void createWebView(final int index)
    {
        pause();
        AppPlayBaseActivity.sTheActivity.runOnUiThread(new Runnable() 
        {
            @Override
            public void run() {
            	AppPlayWebView webView = new AppPlayWebView(TheActivity, index);
                FrameLayout.LayoutParams lParams = new FrameLayout.LayoutParams(
                        FrameLayout.LayoutParams.WRAP_CONTENT,
                        FrameLayout.LayoutParams.WRAP_CONTENT);
                sLayout.addView(webView, lParams);
                webView.setDrawingCacheEnabled(true);
                webViews.put(index, webView);
            }
        });
        resume();
    }
    
    public static int createWebView() 
    {
        final int index = viewTag;
        createWebView(index);
        return viewTag++;
    }
    
    public static void removeWebView(final int index) 
    {
    	TheActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
            	AppPlayWebView webView = webViews.get(index);
                if (webView != null) {
                    webViews.remove(index);
                    sLayout.removeView(webView);
                    Log.d(TAG, "removeWebView");
                    webView.destroy();
                }
            }
        });
    }
    
    public static void removeAllWebViews() {
    	TheActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
            	for (int i=0; i<webViews.size(); i++)
        		{
            		AppPlayWebView webView = webViews.get(webViews.keyAt(i));
        			if (webView != null) {
                        sLayout.removeView(webView);
                        Log.d(TAG, "removeWebView");
                        webView.destroy();
                    }
        		}
            	webViews.clear();
            }
        });
    }
    
    public static void setVisible(final int index, final boolean visible) {
    	TheActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
            	AppPlayWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.setVisibility(visible ? View.VISIBLE : View.INVISIBLE);
                }
            }
        });
    }
    
    public static void setWebViewRect(final int index, final int left, final int top, final int maxWidth, final int maxHeight) {
    	TheActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
            	AppPlayWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.setWebViewRect(left, top, maxWidth, maxHeight);
                }
            }
        });
    }
    
    public static void setJavascriptInterfaceScheme(final int index, final String scheme) {
    	TheActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
            	AppPlayWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.setJavascriptInterfaceScheme(scheme);
                }
            }
        });
    }
    
    public static void loadHTMLString(final int index, final String htmlString, final String data, final String baseUrl) {
    	TheActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
            	AppPlayWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.loadDataWithBaseURL(baseUrl, data, null,null,null);
                }
            }
        });
    }
    
    public static void loadUrl(final int index, final String url) {
    	TheActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
            	AppPlayWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.loadUrl(url);
                }
            }
        });
    }
    
    public static void loadUrlWithRect(final int index, final String url, final String size) {
    	TheActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
            	System.out.println("xxxxxxxxxxxxxxxxxxxxxxxxxx loadUrlWithRect");
            	System.out.println(url);
            	System.out.println(size);
            	AppPlayWebView webView = webViews.get(index);
                if (webView != null) {
					String[] s = size.split("-");
					webView.setWebViewRect(Integer.parseInt(s[0]), Integer.parseInt(s[1]), Integer.parseInt(s[2]), Integer.parseInt(s[3]));
					webView.loadUrl(url);
                }
            }
        });
    }
    
    public static void loadFile(final int index, final String filePath) {
    	TheActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
            	AppPlayWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.loadUrl(filePath);
                }
            }
        });
    }
    
    public static void getWebViewImage(final int index) {  

    	if (sbIsPause)
    		return;    	
    	
    	Callable<byte[]> callable = new Callable<byte[]>() {
            @Override
            public byte[] call() throws Exception {            	
            	
            	AppPlayWebView webView = webViews.get(index);
                if (webView!=null)
                {               	
                	return webView.getWebViewImage();
                }
                
				return null;
            }
        };
        try {        	
        	byte[] imageData = callInMainThread(callable);   	
        	if (null != imageData)
        	{
            	onSetByteArrayBuffer(index, imageData, imageData.length);	
        	}        	
        } catch (Exception e) 
        {
        }        
	}
    
    public static void stopLoading(final int index) {
    	TheActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
            	AppPlayWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.stopLoading();
                }
            }
        });

    }
    
    public static void reload(final int index) {
    	TheActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
            	AppPlayWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.reload();
                }
            }
        });
    }
    
    public static <T> T callInMainThread(Callable<T> call) throws ExecutionException, InterruptedException, TimeoutException {
        FutureTask<T> task = new FutureTask<T>(call);
        handler.post(task);
        return task.get(100, TimeUnit.MILLISECONDS);
    }
    
    public static boolean canGoBack(final int index) throws TimeoutException {
        Callable<Boolean> callable = new Callable<Boolean>() {
            @Override
            public Boolean call() throws Exception {
            	AppPlayWebView webView = webViews.get(index);
                return webView != null && webView.canGoBack();
            }
        };
        try {
            return callInMainThread(callable);
        } catch (ExecutionException e) {
            return false;
        } catch (InterruptedException e) {
            return false;
        }
    }
    
    public static boolean canGoForward(final int index) throws TimeoutException {
        Callable<Boolean> callable = new Callable<Boolean>() {
            @Override
            public Boolean call() throws Exception {
            	AppPlayWebView webView = webViews.get(index);
                return webView != null && webView.canGoForward();
            }
        };
        try {
            return callInMainThread(callable);
        } catch (ExecutionException e) {
            return false;
        } catch (InterruptedException e) {
            return false;
        }
    }
    
    public static void goBack(final int index) {
    	TheActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
            	AppPlayWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.goBack();
                }
            }
        });
    }
    
    public static void goForward(final int index) {
    	TheActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
            	AppPlayWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.goForward();
                }
            }
        });
    }
    
    public static void evaluateJS(final int index, final String js) {
    	s_bWaitGetHemlSource = false;
    	TheActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
            	AppPlayWebView webView = webViews.get(index);
                if (webView != null) {
                	s_bWaitGetHemlSource = true;
                	webView.loadUrl("javascript:"+js);
                }
            }
        });
    	
        try {
        	while (s_bWaitGetHemlSource) {
            	Thread.sleep(100);
            }
        } catch (Exception e) {
        }
	}
    
    public static void setScalesPageToFit(final int index, final boolean scalesPageToFit) {
    	TheActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
            	AppPlayWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.setScalesPageToFit(scalesPageToFit);
                }
            }
        });
    }
    
    public static String[] getAllWebviews() {

    	String [] strs = new String[1 + webViews.size() * 3];
    	
    	int index = 0;
    	strs[index++] = String.valueOf(viewTag);
    	
    	for (int i=0; i<webViews.size(); i++) {
    		AppPlayWebView webView = webViews.get(webViews.keyAt(i));
            if (webView != null) {
            	strs[index++] = String.valueOf(webView.getViewTag());
				strs[index++] = webView.getUrl();
            	strs[index++] = webView.getWebViewRectString();
            }
    	}
    	return strs;
    }
    
    public static void setAllWebviews(String[] strs) 
    {
    	removeAllWebViews();
    	
    	int cnt = (strs.length-1) / 3;
    	viewTag = Integer.parseInt(strs[0]);
    	
    	for (int i=0; i<cnt; i++) {
    		int index = Integer.parseInt(strs[3*i+1]);
    		createWebView(index);
    		loadUrlWithRect(index, strs[3*i+2], strs[3*i+3]);
    		System.out.println("setAllWebviews  create...");
    	}
    }
}
