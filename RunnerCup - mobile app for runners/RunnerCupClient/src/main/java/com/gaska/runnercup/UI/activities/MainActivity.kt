package com.gaska.runnercup.UI.activities

import android.content.Intent
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.fragment.app.Fragment
import androidx.fragment.app.FragmentTransaction
import com.gaska.runnercup.Communicator
import com.gaska.runnercup.R
import com.gaska.runnercup.UI.fragments.*
import com.gaska.runnercup.data.local.models.LocalPlan
import com.google.android.material.bottomnavigation.BottomNavigationView
import dagger.hilt.android.AndroidEntryPoint


@AndroidEntryPoint
class MainActivity : AppCompatActivity(), Communicator {

    private val activityFragment = ActivityFragment()
    private val plansFragment = PlansListFragment()
//    private val analysisFragment = AnalysisFragment()
    private val profileFragment = ProfileFragment()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        replaceFragment(plansFragment)

        val bottomNavigation = findViewById<BottomNavigationView>(R.id.BottomNavigation)
        bottomNavigation.setOnItemSelectedListener {
            when(it.itemId) {
                R.id.ic_plans -> replaceFragment(plansFragment)
                R.id.ic_activity -> replaceFragment(activityFragment)
//                R.id.ic_analysis -> replaceFragment(analysisFragment)
                R.id.ic_profile -> replaceFragment(profileFragment)
            }
            true
        }
    }

    private fun replaceFragment(fragment: Fragment){
        val transaction = this.supportFragmentManager.beginTransaction()
        transaction.replace(R.id.MainFragmentContainer, fragment)
        transaction.commit()
    }

    override fun PassLoginData(email: String) {
        return
    }

    override fun PassPlanToFragment(plan: LocalPlan, fragment: Fragment) {
        val bundle = Bundle()
        bundle.putSerializable("BUNDLE_PLAN", plan)
        fragment.arguments = bundle
        val transaction = this.supportFragmentManager.beginTransaction()
        transaction.replace(R.id.MainFragmentContainer, fragment)
        transaction.setTransition(FragmentTransaction.TRANSIT_FRAGMENT_FADE)
        transaction.addToBackStack(null)
        transaction.commit()
    }

    override fun Logout() {
        val intent = Intent(this, LoginActivity::class.java)
        intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK or Intent.FLAG_ACTIVITY_CLEAR_TASK)
        startActivity(intent)
    }
}