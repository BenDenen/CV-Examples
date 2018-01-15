package com.bendenen.kcopcv.viper

/**
 * Base Viper Preseter Interface
 */
interface ViperPresenter<in V, in R> where V : ViperView, R : ViperRouter {

    fun takeView(viperView: V)

    fun takeRouter(viperRouter: R)

    fun dropView()

    fun dropRouter()

}