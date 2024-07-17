package com.gaska.data.model

data class Plan(
    val ID: Int,
    val date: String,
    val distance: Double,
    val startPointLat: Double,
    val startPointLong: Double,
    val endPointLat: Double,
    val endPointLong: Double
)
