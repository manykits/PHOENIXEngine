package org.appplay.lib;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.net.URI;
import java.util.Timer;
import java.util.TimerTask;

import android.content.Context;
import android.os.Handler;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.TextView;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;
import org.appplay.ap.*;

public class AppPlayUpdateLayout extends LinearLayout
{	
	private static final int SHOWCHECKING = 1;
	private static final int SHOWUPDATING = 2;
	private static final int PROCESS = 3;
	private static final int HIDE = 4;	
	
	public View TheRootView;
	private LayoutInflater mLayoutInflater;
	
	private AppPlayBaseActivity mTheActivity;
	private float mClientVersion = 0.0f;
	
	private TextView mTitle;
	private ProgressBar mProgress;

	public AppPlayUpdateLayout(Context context, int layoutId)
	{
		super(context);
		
		mTheActivity = (AppPlayBaseActivity)context;
		
		mLayoutInflater = ((LayoutInflater) getContext().getSystemService("layout_inflater"));
		TheRootView = mLayoutInflater.inflate(layoutId, this, true);		
		mTitle = (TextView) TheRootView.findViewById(R.id.AppPlayDownloadTitle);
		mProgress = (ProgressBar) TheRootView.findViewById(R.id.AppPlayUpdateProgressBar);
		
	}
	
	Handler mHandler = new Handler() 
	{
		public void handleMessage(Message msg)
		{
			switch (msg.what) 
			{
			case SHOWCHECKING:
				_ShowChecking();
				break;
			case SHOWUPDATING:
				_ShowUpdating();
				break;
			case PROCESS:
				_Process(msg.arg1);
				break;
			case HIDE:
				_Hide();
				break;
			default:
				break;
			}
		};
	};
	
	public void CheckVersion () 
	{
		if (!AppPlayNetwork.IsNetConnected(mTheActivity))
		{
			mTheActivity.Show_NoNetDlg();
		}
		
		Message msg = new Message();
		msg.what = SHOWCHECKING;
		mHandler.dispatchMessage(msg);

		AppPlayVersion version = new AppPlayVersion();
		boolean loadRet = version.LoadUpdateVersionXML();
		
		if (!loadRet)
		{
			mTheActivity.Show_ConnectResServerFailedDlg();
		}
		
		if (!version.IsAPKNeedUpdate())
		{ // apk version is same, judge whether to download LibSO
			
			if (version.IsLibSONeedUpdate() || version.IsResNeedUpdate())
			{
				if (!AppPlayNetwork.IsWifiConnected(mTheActivity))
				{
					mTheActivity.Show_NoWifiDialog();
				}
			}
			
			if (version.IsLibSONeedUpdate()) 
			{				
				TimerTask task = new TimerTask() 
				{				
					public void run() 
					{
						Message msg = new Message();
						msg.what = SHOWUPDATING;
						mHandler.dispatchMessage(msg);
						
						try
						{
							HttpClient client = new DefaultHttpClient();
							HttpGet get = new HttpGet(new URI(AppPlayMetaData.sURL_LibSO));
							HttpResponse response;
							try 
							{
								response = client.execute(get);
								HttpEntity entity = response.getEntity();
								long length = entity.getContentLength();
								InputStream is = entity.getContent();
								FileOutputStream fileOutputStream = null;
								if (is != null) 
								{
									File file = new File(mTheActivity.sLibSO_AppPlay_Filename);
									if (!file.exists()) 
									{
										new File(mTheActivity.sLibSO_AppPlay_Dir).mkdir();
										
										file.createNewFile();
									}
									
									fileOutputStream = new FileOutputStream(file);
									byte[] b = new byte[4 * 1024];
									int charb = -1;
									int count = 0;
									while ((charb = is.read(b)) != -1) 
									{
										fileOutputStream.write(b, 0, charb);
										count += charb;
										double progress = ((double) count / (double) length) * 100.0;

										msg = new Message();
										msg.what = PROCESS;
										msg.arg1 = (int) progress;
										mHandler.dispatchMessage(msg);
									}
								}
								
								fileOutputStream.flush();
								if (fileOutputStream != null) 
								{
									fileOutputStream.close();
								}
							} 
							catch (Exception e) 
							{
								e.printStackTrace();
							}	
						}
						catch (Exception e)
						{
							e.printStackTrace();
						}

						// simple re download version file to replace local version file
						AppPlayNetwork.DownloadFile(AppPlayMetaData.sURL_Version, 
								AppPlayBaseActivity.sTheActivity.sVersion_Dir,
								AppPlayBaseActivity.sTheActivity.sVersion_Filename, null);
						
						AppPlayBaseActivity.sTheActivity.Show_GLView();
					}
				};
				
				Timer timer = new Timer();
				timer.schedule(task, 100);		
			}
			else
			{
				AppPlayBaseActivity.sTheActivity.Show_GLView();
			}
		}
		else
		{ // apk version is not same, tell user to download it
			mTheActivity.Show_HasNewAPKDlg();
		}
	}
	
	private void _ShowChecking() 
	{
		mTheActivity.runOnUiThread(new Runnable() {
			public void run() 
			{
				mTitle.setVisibility(View.VISIBLE);
				mTitle.setText("检测版本...");
				invalidate();
			}
		});

	}

	private void _ShowUpdating() 
	{
		mTheActivity.runOnUiThread(new Runnable()
		{
			public void run() 
			{				
				mTitle.setVisibility(View.VISIBLE);
				mProgress.setVisibility(View.VISIBLE);
				mTitle.setText("更新核心程序...");
				invalidate();
			}
		});
	}

	private void _Hide()
	{
		mTheActivity.runOnUiThread(new Runnable() 
		{
			public void run() 
			{	
				mTitle.setVisibility(View.GONE);
				mProgress.setVisibility(View.GONE);
				invalidate();
			}
		});
	}

	private void _Process(final double process) 
	{
		mTheActivity.runOnUiThread(new Runnable() 
		{
			public void run() 
			{			
				mProgress.setProgress((int) process);
				mTitle.setText("更新核心程序..." + (int)process + "%");
				invalidate();
			}
		});
	}

}
