package org.appplay.lib;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.xml.sax.SAXException;
import android.util.Log;

public class AppPlayVersion
{
	private String mLibSO_VersionCode;
	private String mLibSO_UpdateURL;
	private String mVersionStr;
	
	private int mMain = 0;
	private int mLib = 0;
	private int mRes = 0;
	
	private boolean mIsAPKNeedUpdate = false;
	private boolean mIsLibSONeedUpdate = false;
	private boolean mIsResNeedUpdate = false;
	
	public AppPlayVersion ()
	{
	}
	
	public boolean LoadUpdateVersionXML ()
	{
		String versionStr;
		
		// version file packaged in apk
		String localVersionStr = _LoadVersion(1);	
		Log.d("appplay.lib", "localVersionStr:" + localVersionStr);
		
		// version file already exist(download before)
		String updateVersionStr = _LoadVersion(2);
		Log.d("appplay.lib", "updateVersionStr:" + updateVersionStr);
		
		if (updateVersionStr!=null && updateVersionStr!="")
		{
			versionStr = updateVersionStr;
		}
		else
		{
			versionStr = localVersionStr;
		}
		
		// make mind which versionStr is the latest now
		Log.d("appplay.lib", "last versionStr:" + versionStr);	
		
		// split string to get each part
		String versionArray[] = versionStr.split("\\.");
		int curLatestVersionAPK = 0;
		int curLatestVersionLib = 0;
		int curLatestVersionRes = 0;
		Log.w("phoenix3d.px2", "versionArray arrayLength:" + versionArray.length);		
		if (3 == versionArray.length)
		{
			curLatestVersionAPK = Integer.parseInt(versionArray[0]);
			curLatestVersionLib = Integer.parseInt(versionArray[1]);
			curLatestVersionRes = Integer.parseInt(versionArray[2]);
		}
	    
	    // download the temporary version file from net
		if (AppPlayNetwork.DownloadFile(AppPlayMetaData.sURL_Version,
				AppPlayBaseActivity.sTheActivity.sVersion_Dir,
				AppPlayBaseActivity.sTheActivity.sVersion_Filename_Temp, null) )
		{
			String updateVersion_TempJStr = _LoadVersion(3);
			Log.d("appplay.ap", "updateVersion_TempJStr:" + updateVersion_TempJStr);
			
			String updateVersionArray[] = updateVersion_TempJStr.split("\\.");
			int updateVersionAPK = 0;
			int updateVersionLib = 0;
			int updateVersionRes = 0;
			Log.d("appplay.ap", "updateVersionArray arrayLength:" + updateVersionArray.length);
			if (3 == updateVersionArray.length)
			{
				updateVersionAPK = Integer.parseInt(updateVersionArray[0]);
				updateVersionLib = Integer.parseInt(updateVersionArray[1]);
				updateVersionRes = Integer.parseInt(updateVersionArray[2]);
			}
			
			if (updateVersionAPK > curLatestVersionAPK)
			{
				mIsAPKNeedUpdate = true;
			}
			else
			{
				mIsAPKNeedUpdate = false;
			}
			Log.d("appplay.ap", "IsAPKNeedUpdate:" + mIsAPKNeedUpdate);			
			
			if (updateVersionLib > curLatestVersionLib)
			{
				mIsLibSONeedUpdate = true;
			}
			else
			{
				mIsLibSONeedUpdate = false;
			}
			Log.d("appplay.ap", "ISLibSONeedUpdate:" + mIsLibSONeedUpdate);

			if (updateVersionRes > curLatestVersionRes)
			{
				mIsResNeedUpdate = true;
			}
			else
			{
				mIsResNeedUpdate = false;
			}
			Log.d("appplay.ap", "IsResNeedUpdate:" + mIsResNeedUpdate);
			
			return true;
		}		
		
		return false;
	}
	
	public boolean IsAPKNeedUpdate ()
	{
		return mIsAPKNeedUpdate;
	}
	
	public boolean IsLibSONeedUpdate ()
	{
		return mIsLibSONeedUpdate;
	}	
	
	public boolean IsResNeedUpdate ()
	{
		return mIsResNeedUpdate;
	}

	
	// Load version file
	// type = 1 from localfile packaged in apk
	// type = 2 from localfile download from net before
	// type = 3 temporary file download from net
	// 
	// the version file :
	// <Version value="1.0.0">
	// </Version>
	//
	public String _LoadVersion (int type)
	{
		String versionStr = "";
		String VER = "Version";
		
		DocumentBuilderFactory docFactory = DocumentBuilderFactory.newInstance();  
		DocumentBuilder docBuilder;		
		Document doc = null;
		InputStream inStream = null;
		try 
		{
			docBuilder = docFactory.newDocumentBuilder();

			if (1 == type) 
			{
				// the version file is put int "Data/" dir, whitch is your project data dir packaged into android asserts 
				inStream = AppPlayBaseActivity.sTheActivity.getResources().getAssets().open("Data/version.xml");

				doc = docBuilder.parse(inStream);

				// root
				Element rootEle = doc.getDocumentElement();
				
				// name
				String name = rootEle.getNodeName();
				if (name.equals(VER))
				{					
					// value
					versionStr = rootEle.getAttribute("value");	
				}
			} 
			else
			{	
				String filename = "";
				
				if (2 == type)
					filename = AppPlayBaseActivity.sTheActivity.sVersion_Filename;
				else if (3 == type)
					filename = AppPlayBaseActivity.sTheActivity.sVersion_Filename_Temp;
				
				File file = new File(filename);
				if (file.exists()) 
				{
					doc = docBuilder.parse(file);
					
					Element rootEle = doc.getDocumentElement();

					String name = rootEle.getNodeName();
					if (name.equals(VER))
					{
						versionStr = rootEle.getAttribute("value");	
					}
				}
			}
	    }
	    catch (ParserConfigurationException e1) 
	    {  
	    	e1.printStackTrace();  
	    }
	    catch (IOException e) 
	    {  
	    	e.printStackTrace();  
	    } 
	    catch (SAXException e)
	    {  
	    	e.printStackTrace();  
	    } 
	    
	    return versionStr;
	}
}
