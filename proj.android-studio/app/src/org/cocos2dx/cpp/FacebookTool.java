package org.cocos2dx.cpp;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.util.Log;

import com.facebook.CallbackManager;
import com.facebook.FacebookCallback;
import com.facebook.FacebookException;
import com.facebook.FacebookSdk;
import com.facebook.appevents.AppEventsLogger;
import com.facebook.share.Sharer;
import com.facebook.share.model.ShareContent;
import com.facebook.share.model.ShareLinkContent;
import com.facebook.share.model.SharePhoto;
import com.facebook.share.model.SharePhotoContent;
import com.facebook.share.widget.ShareDialog;


import java.io.File;

/**
 * Created by guang on 16/12/6.
 */
public class FacebookTool {

    private static AppActivity activity;
    private CallbackManager mCallbackManager;
    private static FacebookTool s_instance;

    private FacebookTool(){}

    public static FacebookTool getInstance()
    {
        if(s_instance == null)
        {
            s_instance = new FacebookTool();
        }
        return s_instance;
    }

    public static void init(AppActivity act)
    {
        getInstance();
        activity = act;

        FacebookSdk.sdkInitialize(activity.getApplicationContext());
//        AppEventsLogger.activateApp(activity);
    }

    public CallbackManager getCallbackManager()
    {
        return mCallbackManager;
    }


    public void shareUrl() {
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                String url = "https://play.google.com/store/apps/details?id=com.qianqi.bubble.io&referrer=utm_source%3Dfbshare";
                ShareLinkContent content = new ShareLinkContent.Builder()
                        .setContentUrl(Uri.parse(url))
                        .setImageUrl(Uri.parse("http://104.238.126.116:8080/PaoPaoServer/1280.jpg"))
                        .build();

                ShareDialog shareDialog = new ShareDialog(activity);
                mCallbackManager = CallbackManager.Factory.create();
                shareDialog.registerCallback(mCallbackManager, new FacebookCallback<Sharer.Result>() {
                    @Override
                    public void onSuccess(Sharer.Result result) {
                        shareCallback("success");
                    }

                    @Override
                    public void onCancel() {
                        shareCallback("cancel");
                    }

                    @Override
                    public void onError(FacebookException error) {
                        shareCallback("fail");
                    }
                });

                shareDialog.show(content, ShareDialog.Mode.AUTOMATIC);
            }
        });
    }

    public void sharePic() {
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                String imagePath = activity.getApplicationContext().getFilesDir().getPath();
                File f = new File(imagePath,"ScreenShot.png");

                if(!f.exists())
                {
                    sharePicCallback("fail");
                    return;
                }
                Bitmap image = BitmapFactory.decodeFile(f.getPath());
                SharePhoto photo = new SharePhoto.Builder()
                        .setBitmap(image)
                        .build();
                SharePhotoContent content = new SharePhotoContent.Builder()
                        .addPhoto(photo)
                        .build();

                ShareDialog shareDialog = new ShareDialog(activity);
                mCallbackManager = CallbackManager.Factory.create();
                shareDialog.registerCallback(mCallbackManager, new FacebookCallback<Sharer.Result>() {
                    @Override
                    public void onSuccess(Sharer.Result result) {
                        sharePicCallback("success");
                    }

                    @Override
                    public void onCancel() {
                        sharePicCallback("cancel");
                    }

                    @Override
                    public void onError(FacebookException error) {
                        sharePicCallback("fail");
                    }
                });
                shareDialog.show(content, ShareDialog.Mode.AUTOMATIC);
            }
        });
    }

    public void shareCallback(final String s)
    {
        activity.runOnGLThread(new Runnable() {
            @Override
            public void run() {
                JniTools.shareResult(s);
            }
        });
    }

    public void sharePicCallback(final String s)
    {
        activity.runOnGLThread(new Runnable() {
            @Override
            public void run() {
                JniTools.sharePicResult(s);
            }
        });
    }
}
