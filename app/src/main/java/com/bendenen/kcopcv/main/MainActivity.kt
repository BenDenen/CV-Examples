package com.bendenen.kcopcv.main

import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.view.Menu
import android.view.MenuItem
import com.bendenen.kcopcv.kotlin_opencv_counters.R
import com.bendenen.kcopcv.main.presenter.MainScreenPresenter
import com.bendenen.kcopcv.main.presenter.MainScreenPresenterImpl
import com.bendenen.kcopcv.main.router.MainScreenRouter
import com.bendenen.kcopcv.main.router.MainScreenRouterImpl
import com.bendenen.kcopcv.main.view.MainScreenView

import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity(), MainScreenView {

    lateinit var presenter:MainScreenPresenter
    lateinit var router:MainScreenRouter

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        setSupportActionBar(toolbar)

        presenter = MainScreenPresenterImpl()
        router = MainScreenRouterImpl(this)

    }

    override fun onStart() {
        super.onStart()
        presenter.takeRouter(router)
        presenter.takeView(this)
    }

    override fun onStop() {
        presenter.dropView()
        presenter.dropRouter()
        super.onStop()
    }

    override fun onCreateOptionsMenu(menu: Menu): Boolean {
        // Inflate the menu; this adds items to the action bar if it is present.
        menuInflater.inflate(R.menu.menu_main, menu)
        return true
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        return when (item.itemId) {
            R.id.action_settings -> true
            else -> super.onOptionsItemSelected(item)
        }
    }

    override fun onViewTaken(presenter: MainScreenPresenter) {
        fab.setOnClickListener {
            presenter.requestCameraScreen()
        }
    }

}
