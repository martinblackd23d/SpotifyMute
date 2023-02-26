package com.example.spotifymuteandroid;

//import android.app.Notification;
import android.content.Context;
import android.content.Intent;
import android.media.AudioManager;
//import android.os.Bundle;
import android.os.IBinder;
import android.service.notification.NotificationListenerService;
import android.service.notification.StatusBarNotification;
//import android.text.SpannableString;
//import android.util.Log;

import java.util.Objects;

public class NotificationListenerInstance extends NotificationListenerService {

    @Override
    public IBinder onBind(Intent intent) {
        return super.onBind(intent);
    }

    @Override
    public void onListenerConnected() {

        StatusBarNotification[] notifications;
        int num;
        //Bundle extras;
        String title;
        boolean isad = false;
        AudioManager amanager=(AudioManager)getSystemService(Context.AUDIO_SERVICE);
        //Log.d("Current Track", "Program has successfully started");
        while (true) {
            //Log.d("Current Track", "while loop entered");
            notifications = getActiveNotifications();
            //Log.d("Current Track", "notifications created");
            num = notifications.length;

            for (int i = 0; i < num; i++) {
                //Log.d("Current Track", "entered for loop");
                //extras = notifications[i].getNotification().extras;
                //Log.d("Current Track", "extras were created");
                //title = extras.getString(Notification.EXTRA_TITLE);
                //Log.d("Current Track", "title was created");
                title = String.valueOf(notifications[i].getNotification().extras.get("android.text"));
                //Log.d("Current Track", title);
                if (Objects.equals("Advertisement", title)) {
                    isad = true;
                    amanager.setStreamMute(AudioManager.STREAM_MUSIC, true);
                    break;
                }
            }

            while (isad) {
                notifications = getActiveNotifications();
                num = notifications.length;
                isad = false;

                for (int i = 0; i < num; i++) {
                    //extras = notifications[i].getNotification().extras;
                    //title = extras.getString(Notification.EXTRA_TITLE);
                    title = String.valueOf(notifications[i].getNotification().extras.get("android.text"));
                    //Log.d("Current Track", title);
                    if (Objects.equals("Advertisement", title)) {
                        isad = true;
                        break;
                    }
                }
                if (!isad) {
                    try {
                        Thread.sleep(1000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    amanager.setStreamMute(AudioManager.STREAM_MUSIC, false);
                    break;
                }
                try {
                    Thread.sleep(100);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

    }
}
