package com.example.psl7504.sudokuchapter3;

import android.content.Context;
import android.media.MediaPlayer;

/**
 * Created by psl7504 on 11/9/17.
 */

public class Music {

    private static MediaPlayer mp = null;
    //currently playing song stops and new one plays
    public static void play(Context context, int resources){
        if(mp != null && mp.isPlaying())
            stop(context);
        if(Prefs.getHints(context)) {
            mp = MediaPlayer.create(context, resources);
            mp.setLooping(true);
            mp.start();
        }
    }

    //stops the music
    public static void stop(Context context){
        if(mp!=null){
            if(mp.isPlaying())
                mp.stop();
            mp.release();
            mp = null;
        }


    }




}
