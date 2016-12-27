package org.cocos2dx.cpp;

import com.umeng.analytics.MobclickAgent;
import com.umeng.analytics.game.UMGameAgent;

import java.util.HashMap;

/**
 * Created by guang on 16/12/8.
 */
public class UMengTool {
    private static AppActivity activity;

    public static void init(AppActivity act)
    {
        activity = act;

        MobclickAgent.setScenarioType(activity, MobclickAgent.EScenarioType.E_UM_GAME);
        UMGameAgent.init( activity );
//        UMGameAgent.setDebugMode( true );
    }

    public static void onResume()
    {
        UMGameAgent.onResume(activity);
    }

    public static void onPause()
    {
        UMGameAgent.onPause(activity);
    }

    public static void eventMode1()
    {
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                MobclickAgent.onEvent(activity,"mode");
            }
        });
    }

    public static void eventMode2()
    {
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                MobclickAgent.onEvent(activity,"mode2");
            }
        });
    }

    public static void eventPing(final int ping)
    {
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                HashMap<String, String> map_value = new HashMap<String, String>();
                map_value.put("__ct__" , String.valueOf(ping));
                MobclickAgent.onEvent(activity,"ping",map_value);
            }
        });
    }
}
