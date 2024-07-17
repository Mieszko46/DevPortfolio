package com.gaska.runnercup.data.remote

import com.gaska.runnercup.data.remote.models.RemotePlan
import com.gaska.runnercup.data.remote.models.SimpleResponse
import com.gaska.runnercup.data.remote.models.User
import com.gaska.runnercup.utils.Constants.API_VERSION
import retrofit2.http.*

interface PlanApi {

    @Headers("Content-Type: application/json")
    @POST("$API_VERSION/users/register")
    suspend fun createAccount(
        @Body user: User
    ): SimpleResponse

    @Headers("Content-Type: application/json")
    @POST("$API_VERSION/users/login")
    suspend fun login(
        @Body user: User
    ): SimpleResponse


    // =========== PLANS ===========

    @Headers("Content-Type: application/json")
    @POST("$API_VERSION/plans/create")
    suspend fun createPlan(
        @Header("Authorization") token:String,
        @Body plan: RemotePlan
    ): SimpleResponse

    @Headers("Content-Type: application/json")
    @GET("$API_VERSION/plans")
    suspend fun getAllplans(
        @Header("Authorization") token:String
    ): List<RemotePlan>

    @Headers("Content-Type: application/json")
    @POST("$API_VERSION/plans/update")
    suspend fun updatePlan(
        @Header("Authorization") token:String,
        @Body plan: RemotePlan
    ): SimpleResponse

    @Headers("Content-Type: application/json")
    @DELETE("$API_VERSION/plans/delete")
    suspend fun deletePlan(
        @Header("Authorization") token:String,
        @Query("id") planId:Int
    ): SimpleResponse
}