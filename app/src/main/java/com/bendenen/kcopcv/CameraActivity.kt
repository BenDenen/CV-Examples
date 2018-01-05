package com.bendenen.kcopcv

import android.support.v7.app.AppCompatActivity

/**
 * Activity to preview camera stream with processed frames
 */
class CameraActivity : AppCompatActivity() {

    companion object {

        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("native-lib")
        }
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String


}