package com.gaska.runnercup.data.remote.models

data class User(
    val email:String,
    val password:String,
    val name: String? = null
)
