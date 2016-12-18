package org.cocos2dx.cpp;

import java.util.UUID;

import android.content.Context;
import android.telephony.TelephonyManager;
import android.util.Log;

public class JniTools {

	private static AppActivity activity;
	
	public static void init(AppActivity act)
	{
		activity = act;
	}
	
	public static AppActivity getAppActivity()
	{
		return activity;
	}
	
	public static String getDeviceId()
	{
		TelephonyManager telephonyManager = (TelephonyManager)activity.getSystemService( Context.TELEPHONY_SERVICE );  
		String uuid = telephonyManager.getDeviceId();
		if(uuid == null || uuid.length() < 15)
		{
			uuid = UUID.randomUUID().toString().replace("-", "");
			uuid = uuid.substring(0, 15);
		}
		Log.e("--------------", "uuid="+uuid);
		return uuid;
	}
}
