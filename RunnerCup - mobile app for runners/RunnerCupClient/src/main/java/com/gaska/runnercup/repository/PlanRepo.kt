package com.gaska.runnercup.repository

import com.gaska.runnercup.data.local.models.LocalPlan
import com.gaska.runnercup.data.remote.models.User
import com.gaska.runnercup.utils.Result
import kotlinx.coroutines.flow.Flow

interface PlanRepo {

    suspend fun createUser(user: User):Result<String>
    suspend fun login(user: User):Result<String>
    suspend fun getUser():Result<User>
    suspend fun logout():Result<String>

    suspend fun createPlan(plan: LocalPlan): Result<String>
    suspend fun updatePlan(plan: LocalPlan): Result<String>
    suspend fun deletePlan(planId: Int)

    fun getAllPlans():Flow<List<LocalPlan>>
    suspend fun getAllPlansFromServer()

    suspend fun synchPlans()
}