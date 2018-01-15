package com.bendenen.kcopcv.main.presenter

import com.bendenen.kcopcv.main.router.MainScreenRouter
import com.bendenen.kcopcv.main.view.MainScreenView

class MainScreenPresenterImpl() : MainScreenPresenter {

    var view: MainScreenView? = null
    var router: MainScreenRouter? = null

    override fun takeView(viperView: MainScreenView) {
        view = viperView
        view?.onViewTaken(this)
    }

    override fun takeRouter(viperRouter: MainScreenRouter) {
        router = viperRouter
    }

    override fun dropView() {
        view = null
    }

    override fun dropRouter() {
        router = null
    }

    override fun requestCameraScreen() {
        if(router?.isCapturePermissionsGranted()!!) {
            router?.startCameraActivity()
        } else {
            router?.requestCapturePermissions()
        }
    }

    override fun onRequestPermissionsResult(requestCode: Int, permissions: Array<out String>, grantResults: IntArray) {
        if (router?.onRequestCaptureResult(requestCode, permissions, grantResults)!!) {
            requestCameraScreen()
        }
    }


}