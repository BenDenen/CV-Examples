package com.bendenen.kcopcv.main.presenter

import com.bendenen.kcopcv.main.router.MainScreenRouter
import com.bendenen.kcopcv.main.view.MainScreenView
import com.bendenen.kcopcv.viper.ViperPresenter

/**
 * Main screen presenter
 */
interface MainScreenPresenter : ViperPresenter<MainScreenView,MainScreenRouter> {

    fun requestCameraScreen()

    fun onRequestPermissionsResult(requestCode: Int, permissions: Array<out String>, grantResults: IntArray)

}