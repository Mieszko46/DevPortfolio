package com.gaska.runnercup

import androidx.fragment.app.Fragment
import com.gaska.runnercup.data.local.models.LocalPlan

interface Communicator {
    fun PassLoginData(email: String)
    fun PassPlanToFragment(plan: LocalPlan, fragment: Fragment)
    fun Logout()
}