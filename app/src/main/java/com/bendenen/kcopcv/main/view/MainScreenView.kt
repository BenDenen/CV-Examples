package com.bendenen.kcopcv.main.view

import com.bendenen.kcopcv.main.presenter.MainScreenPresenter
import com.bendenen.kcopcv.viper.ViperView

/**
 * Main screen view
 */
interface MainScreenView : ViperView {

    fun onViewTaken(presenter:MainScreenPresenter)
}