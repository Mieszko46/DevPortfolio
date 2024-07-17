package com.gaska.runnercup.repository

import com.gaska.runnercup.data.local.dao.PlanDao
import com.gaska.runnercup.data.local.models.LocalPlan
import com.gaska.runnercup.data.remote.PlanApi
import com.gaska.runnercup.data.remote.models.RemotePlan
import com.gaska.runnercup.data.remote.models.User
import com.gaska.runnercup.utils.SessionManager
import com.gaska.runnercup.utils.isNetworkConnected
import com.gaska.runnercup.utils.Result
import kotlinx.coroutines.flow.Flow
import javax.inject.Inject

class PlanRepolmpl @Inject constructor(
    val planApi: PlanApi,
    val planDao: PlanDao,
    val sessionManager: SessionManager
): PlanRepo {

    override suspend fun createUser(user: User): Result<String> {
        return try {
            if(!isNetworkConnected(sessionManager.context)) {
                Result.Error<String>("No internet connection!")
            }

            val result = planApi.createAccount(user)
            if(result.success) {
                sessionManager.updateSession(result.message, user.name ?:"", user.email)
                Result.Success("User created successfully!")
            } else {
                Result.Error<String>(result.message)
            }
        } catch (e:Exception) {
            e.printStackTrace()
            Result.Error<String>(e.message ?: "Some problem occurred!")
        }
    }

    override suspend fun login(user: User): Result<String> {
        return try {
            if(!isNetworkConnected(sessionManager.context)) {
                Result.Error<String>("No internet connection!")
            }

            val result = planApi.login(user)
            if(result.success) {
                sessionManager.updateSession(result.message, user.name ?:"", user.email)
                getAllPlansFromServer()
                Result.Success("Login successfully!")
            } else {
                Result.Error<String>(result.message)
            }
        } catch (e:Exception) {
            e.printStackTrace()
            Result.Error<String>( e.message ?: "Some problem occurred!")
        }
    }

    override suspend fun getUser(): Result<User> {
        return try {
            val name = sessionManager.getCurrentUserName()
            val email = sessionManager.getCurrentUserEmail()
            if(name == null || email == null){
                Result.Error<User>("User not logged in!")
            }
            Result.Success(User(email!!, "", name))
        }catch (e:Exception){
            e.printStackTrace()
            Result.Error(e.message ?: "Some problem occurred!")
        }
    }

    override suspend fun logout(): Result<String> {
        return try {
            sessionManager.logout()
            Result.Success("Logged out successfully!" )
        }catch (e:Exception){
            e.printStackTrace()
            Result.Error(e.message ?: "Some problem occurred!")
        }
    }

    override suspend fun createPlan(plan: LocalPlan): Result<String> {
        try {
            planDao.insertPlan(plan)
            val token = sessionManager.getJwtToken()
                ?: return Result.Success("Plan is Saved in local database!")

            if (!isNetworkConnected(sessionManager.context)) {
                return Result.Error("No internet connection!")
            }

            val result = planApi.createPlan(
                "Bearer $token",
                RemotePlan(
                    ID = plan.ID!!,
                    date = plan.date,
                    distance = plan.distance,
                    startPointLat = plan.startPointLat,
                    startPointLong = plan.startPointLong,
                    endPointLat = plan.endPointLat,
                    endPointLong = plan.endPointLong
                )
            )

            return if (result.success) {
                planDao.insertPlan(plan.also { it.connected = true })
                Result.Success("Plan saved successfully!")
            } else {
                Result.Error(result.message)
            }
        } catch (e:Exception){
            e.printStackTrace()
            return Result.Error(e.message ?: "Some problem occurred!")
        }
    }

    override suspend fun updatePlan(plan: LocalPlan): Result<String> {
        try {
            planDao.insertPlan(plan)
            val token = sessionManager.getJwtToken()
                ?: return Result.Success("Plan is Updated in local database!")

            if (!isNetworkConnected(sessionManager.context)) {
                return Result.Error("No internet connection!")
            }

            val result = planApi.updatePlan(
                "Bearer $token",
                RemotePlan(
                    ID = plan.ID!!,
                    date = plan.date,
                    distance = plan.distance,
                    startPointLat = plan.startPointLat,
                    startPointLong = plan.startPointLong,
                    endPointLat = plan.endPointLat,
                    endPointLong = plan.endPointLong
                )
            )

            return if (result.success) {
                planDao.insertPlan(plan.also { it.connected = true })
                Result.Success("Plan updated successfully!")
            } else {
                Result.Error(result.message)
            }
        } catch (e:Exception){
            e.printStackTrace()
            return Result.Error(e.message ?: "Some problem occurred!")
        }
    }

    override suspend fun deletePlan(planId: Int) {
        try {
            planDao.deletePlanLocally(planId)
            val token = sessionManager.getJwtToken() ?: kotlin.run {
                planDao.deletePlan(planId)
                return
            }
            if (!isNetworkConnected(sessionManager.context)) {
                return
            }
            val response = planApi.deletePlan("Bearer $token", planId )
            if (response.success) {
                planDao
            }
        } catch (e:Exception) {
            e.printStackTrace()
        }
    }

    override fun getAllPlans(): Flow<List<LocalPlan>> = planDao.getAllPlansOrderedByDate()

    override suspend fun getAllPlansFromServer() {
        try {
            val token = sessionManager.getJwtToken() ?: return
            if (!isNetworkConnected(sessionManager.context)) {
                return
            }
            val result = planApi.getAllplans("Bearer $token")
            result.forEach { remotePlan ->
                planDao.insertPlan(
                    LocalPlan(
                        ID = remotePlan.ID,
                        date = remotePlan.date,
                        distance = remotePlan.distance,
                        startPointLat = remotePlan.startPointLat,
                        startPointLong = remotePlan.startPointLong,
                        endPointLat = remotePlan.endPointLat,
                        endPointLong = remotePlan.endPointLong,
                        connected = true
                    )
                )
            }
        } catch (e:Exception) {
            e.printStackTrace()
        }
    }

    override suspend fun synchPlans() {
        try {
            println("3.")
            sessionManager.getJwtToken() ?: return
            if (!isNetworkConnected(sessionManager.context)) {
                return
            }
            val locallyDeletedPlans = planDao.getAllLocallyDeletedPlans()
            locallyDeletedPlans.forEach {
                deletePlan(it.ID!!)
            }
            val notConnectedPlans = planDao.getAllLocalPlans()
            notConnectedPlans.forEach {
                createPlan(it)
            }
            val notUpdatedPlans = planDao.getAllLocalPlans()
            notUpdatedPlans.forEach {
                updatePlan(it)
            }
        } catch (e:Exception) {
            e.printStackTrace()
        }
    }
}