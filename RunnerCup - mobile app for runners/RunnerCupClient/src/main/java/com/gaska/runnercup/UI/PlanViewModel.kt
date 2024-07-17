package com.gaska.runnercup.UI

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.gaska.runnercup.data.local.models.LocalPlan
import com.gaska.runnercup.repository.PlanRepo
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import javax.inject.Inject

@HiltViewModel
class PlanViewModel @Inject constructor(
    val planRepo: PlanRepo
):ViewModel() {

    val plans = planRepo.getAllPlans()
    var oldPlan: LocalPlan? = null

    fun createPlan(
        date: String?,
        distance: Double?,
        startPointLat: Double?,
        startPointLong: Double?,
        endPointLat: Double?,
        endPointLong: Double?
    ) = viewModelScope.launch(Dispatchers.IO){
        val localPlan = LocalPlan(
            date = date,
            distance = distance,
            startPointLat = startPointLat,
            startPointLong = startPointLong,
            endPointLat = endPointLat,
            endPointLong = endPointLong
        )
        planRepo.createPlan(localPlan)
    }

    fun updatePlan(
        date: String?,
        distance: Double?,
        startPointLat: Double?,
        startPointLong: Double?,
        endPointLat: Double?,
        endPointLong: Double?
    ) = viewModelScope.launch(Dispatchers.IO){

        if (date == oldPlan?.date
            && distance == oldPlan?.distance
            && startPointLat == oldPlan?.startPointLat
            && startPointLong == oldPlan?.startPointLong
            && endPointLat == oldPlan?.endPointLat
            && endPointLong == oldPlan?.endPointLong
            && oldPlan?.connected == true
        ) {
            return@launch
        }

        val localPlan = LocalPlan(
            ID = oldPlan!!.ID,
            date = date,
            distance = distance,
            startPointLat = startPointLat,
            startPointLong = startPointLong,
            endPointLat = endPointLat,
            endPointLong = endPointLong
        )
        planRepo.updatePlan(localPlan)
    }

    fun deletePlan(planId: Int) = viewModelScope.launch {
        planRepo.deletePlan(planId)
    }

    fun undoDelete(plan: LocalPlan) = viewModelScope.launch {
        planRepo.createPlan(plan)
    }

    fun synchPlans(onDone: (()->Unit)? = null ) = viewModelScope.launch {
        planRepo.synchPlans()
        onDone?.invoke()
    }

}