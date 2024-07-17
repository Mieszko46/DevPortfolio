package com.gaska.runnercup.data.local

import androidx.room.Database
import androidx.room.RoomDatabase
import com.gaska.runnercup.data.local.dao.PlanDao
import com.gaska.runnercup.data.local.models.LocalPlan

@Database(
    entities = [LocalPlan::class],
    version = 1,
    exportSchema = false
)
abstract class PlanDB: RoomDatabase() {

    abstract fun getPlanDao(): PlanDao
}