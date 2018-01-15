package com.bendenen.kcopcv.main.router

import com.bendenen.kcopcv.viper.ViperRouter

/**
 * Main screen router
 */
interface MainScreenRouter : ViperRouter {

    fun startCameraActivity()

    fun isCapturePermissionsGranted(): Boolean

    fun requestCapturePermissions()

    fun onRequestCaptureResult(requestCode: Int, permissions: Array<out String>, grantResults: IntArray): Boolean

}