package org.appplay.lib;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;

import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;

public class AppPlayNetwork
{
	public AppPlayNetwork() {}
	
	public static boolean IsNetConnected(Context context)
	{
		ConnectivityManager connectivityManager = (ConnectivityManager) context
				.getSystemService(Context.CONNECTIVITY_SERVICE);

		NetworkInfo networkinfo = connectivityManager.getActiveNetworkInfo();
		if (networkinfo!=null && networkinfo.isAvailable())
		{
			return true;
		}

		return false;
	}

	public static boolean IsWifiConnected(Context context)
	{
		ConnectivityManager connectivityManager = (ConnectivityManager) context
				.getSystemService(Context.CONNECTIVITY_SERVICE);
		NetworkInfo wifiNetworkInfo = connectivityManager
				.getNetworkInfo(ConnectivityManager.TYPE_WIFI);
		if (wifiNetworkInfo!=null && wifiNetworkInfo.isConnected()) {
			return true;
		}

		return false;
	}
	
	public static boolean DownloadFile (final String url, String dir, String filename, DownloadProcess callback)
	{
		boolean bRet = false;
		HttpClient client = new DefaultHttpClient();
		HttpGet get = new HttpGet(url);
		HttpResponse response;
		try 
		{
			response = client.execute(get);
			HttpEntity entity = response.getEntity();
			long length = entity.getContentLength();
			InputStream in = entity.getContent();
			FileOutputStream fileOutputStream = null;
			if (in != null) 
			{
				File file = new File(filename);

				if (!file.exists()) 
				{
					new File(dir).mkdir();
					file.createNewFile();
				}
				fileOutputStream = new FileOutputStream(file);
				byte[] b = new byte[4 * 1024];
				int charb = -1;
				int count = 0;
				while ((charb = in.read(b)) != -1) 
				{
					fileOutputStream.write(b, 0, charb);
					count += charb;
					double progress = ((double) count / (double) length) * 100;
					
					if (null != callback)
						callback.process(progress);
				}
			}
			
			fileOutputStream.flush();
			if (fileOutputStream != null)
			{
				fileOutputStream.close();
			}
			
			bRet = true;
		} 
		catch (Exception e)
		{
			e.printStackTrace();
		}
		
		return bRet;
	}
	
	public interface DownloadProcess 
	{
		void process(double process);
	}
}
