package org.cocos2dx.cpp;

import java.io.File;
import java.util.UUID;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;
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

	public static void showInterstitialAd()
	{
		AdTool.showAd();
	}

	public static void mode()
	{
		UMengTool.eventMode1();
	}

	public static void mode2()
	{
		UMengTool.eventMode2();
	}

	public static void ping(int ping)
	{
		UMengTool.eventPing(ping);
	}

//	public static void shareUrl()
//	{
//		activity.runOnUiThread(new Runnable() {
//			@Override
//			public void run() {
//
//				String imagePath = activity.getApplicationContext().getFilesDir().getPath();
//				File f = new File(imagePath,"ScreenShot.png");
//
//				if(!f.exists())
//				{
//					return;
//				}
//
//				Intent intent =new Intent();
//
//				intent.setAction(Intent.ACTION_SEND);
//				intent.setType("image/*");
//
//
//				Uri uri = Uri.fromFile(f);
//
//				intent.putExtra(Intent.EXTRA_STREAM, uri);
//				intent.putExtra("Kdescription", "我的蛇啊");
////				intent.putExtra(Intent.EXTRA_TEXT, "快来玩啊！");
////				intent.putExtra(Intent.EXTRA_SUBJECT, "www.baidu.com");
//
//				Intent chooserIntent = Intent.createChooser(intent, "分享到：");
//				activity.startActivity(chooserIntent);
//
//			}
//		});
//
//	}

	public static void shareUrl()
	{
		FacebookTool.getInstance().shareUrl();
	}

	public static void sharePic()
	{
		FacebookTool.getInstance().sharePic();
	}

	public native static void shareResult(String res);
	public native static void sharePicResult(String res);
}
