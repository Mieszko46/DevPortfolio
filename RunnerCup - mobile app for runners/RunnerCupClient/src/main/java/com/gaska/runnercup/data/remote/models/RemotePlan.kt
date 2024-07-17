package com.gaska.runnercup.data.remote.models

data class RemotePlan(
    val ID: Int,
    val date: String?,
    val distance: Double?,
    val startPointLat: Double?,
    val startPointLong: Double?,
    val endPointLat: Double?,
    val endPointLong: Double?,
)
