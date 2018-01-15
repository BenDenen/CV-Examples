package com.bendenen.kcopcv.main.router

import android.Manifest
import android.content.pm.PackageManager
import android.os.Build
import android.support.v4.app.ActivityCompat
import android.support.v4.content.ContextCompat
import android.util.Log
import com.bendenen.kcopcv.camera.CameraActivity
import com.bendenen.kcopcv.main.MainActivity

class MainScreenRouterImpl(private val activity: MainActivity) : MainScreenRouter {

    companion object {

        val CAPTURE_PERMISSIONS = arrayOf(Manifest.permission.CAMERA, Manifest.permission.WRITE_EXTERNAL_STORAGE)
        const val CAPTURE_PERMISSIONS_REQUEST = 101

    }

    private val TAG = MainScreenRouterImpl::class.java.simpleName

    override fun isCapturePermissionsGranted(): Boolean {
        return if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            CAPTURE_PERMISSIONS.none {
                ContextCompat.checkSelfPermission(activity, it) != PackageManager.PERMISSION_GRANTED
            }
        } else {
            true
        }
    }

    override fun requestCapturePermissions() {
        ActivityCompat.requestPermissions(activity, CAPTURE_PERMISSIONS, CAPTURE_PERMISSIONS_REQUEST)
    }

    override fun startCameraActivity() {
        val intent = CameraActivity.createIntent(activity)
        activity.startActivity(intent)
    }

    override fun onRequestCaptureResult(requestCode: Int, permissions: Array<out String>, grantResults: IntArray): Boolean {
        return if (requestCode == CAPTURE_PERMISSIONS_REQUEST) {
            Log.d(TAG, "grantResults.length == " + grantResults.size)
            (grantResults.size == CAPTURE_PERMISSIONS.size) && grantResults.none { it != PackageManager.PERMISSION_GRANTED }
        } else {
            false
        }
    }
}