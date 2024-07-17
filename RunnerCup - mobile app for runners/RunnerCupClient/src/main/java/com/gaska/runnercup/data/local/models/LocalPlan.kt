package com.gaska.runnercup.data.local.models

import androidx.room.Entity
import androidx.room.PrimaryKey
import java.io.Serializable

@Entity
data class LocalPlan (
    var date: String? = null,
    var distance: Double? = null,
    var startPointLat: Double? = null,
    var startPointLong: Double? = null,
    var endPointLat: Double? = null,
    var endPointLong: Double? = null,
    var connected: Boolean = false,
    var locallyDeleted: Boolean = false,
    @PrimaryKey(autoGenerate = false)
    var ID:Int? = null
): Serializable