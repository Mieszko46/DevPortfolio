package com.gaska.runnercup.UI.activities

import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import androidx.fragment.app.Fragment
import com.gaska.runnercup.Communicator
import com.gaska.runnercup.R
import com.gaska.runnercup.UI.fragments.*
import com.gaska.runnercup.data.local.models.LocalPlan
import dagger.hilt.android.AndroidEntryPoint

@AndroidEntryPoint
class LoginActivity : AppCompatActivity(), Communicator {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_login)

        val transaction = this.supportFragmentManager.beginTransaction()
        transaction.replace(R.id.LoginFragmentContainer, LoginFragment())
        transaction.commit()
    }

    override fun PassLoginData(email: String) {
        val intent = Intent(this, MainActivity::class.java)
        intent.putExtra("EXTRA_EMAIL", email)
        startActivity(intent)
    }

    override fun PassPlanToFragment(plan: LocalPlan, fragment: Fragment) {
        return
    }

    override fun Logout() {
        return
    }
}