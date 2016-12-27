package org.cocos2dx.cpp;

import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.InterstitialAd;

/**
 * Created by guang on 16/12/26.
 */
public class AdTool {

    private static AppActivity activity;
    private static InterstitialAd mInterstitialAd;
    public static void init(AppActivity act) {
        activity = act;
        initAd();
    }


    private static void initAd()
    {
        mInterstitialAd = new InterstitialAd(activity);
        mInterstitialAd.setAdUnitId("ca-app-pub-6717712754766811/6460581287");

        mInterstitialAd.setAdListener(new AdListener() {
            @Override
            public void onAdClosed() {
                preLoadAd();
            }
        });

        preLoadAd();
    }

    private static void preLoadAd()
    {
        AdRequest adRequest = new AdRequest.Builder()
                .build();

        mInterstitialAd.loadAd(adRequest);
    }

    public static void showAd()
    {
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {

                if (mInterstitialAd.isLoaded()) {
                    mInterstitialAd.show();
                } else {
                    preLoadAd();
                }
            }
        });
    }
}
