package com.gaska.runnercup.data.local.dao

import androidx.room.Dao
import androidx.room.Insert
import androidx.room.OnConflictStrategy
import androidx.room.Query
import com.gaska.runnercup.data.local.models.LocalPlan
import kotlinx.coroutines.flow.Flow

@Dao
interface PlanDao {

    @Insert(onConflict = OnConflictStrategy.REPLACE)
    suspend fun insertPlan(plan: LocalPlan)

    @Query("SELECT * FROM LocalPlan WHERE locallyDeleted = 0 ORDER BY date DESC")
    fun getAllPlansOrderedByDate(): Flow<List<LocalPlan>>

    @Query("DELETE FROM LocalPlan WHERE ID=:planId")
    suspend fun deletePlan(planId: Int)

    @Query("UPDATE LocalPlan SET locallyDeleted = 1 WHERE ID=:planId")
    suspend fun deletePlanLocally(planId: Int)

    @Query("SELECT * FROM LocalPlan WHERE connected = 0")
    suspend fun getAllLocalPlans(): List<LocalPlan>

    @Query("SELECT * FROM LocalPlan WHERE locallyDeleted = 1")
    suspend fun getAllLocallyDeletedPlans(): List<LocalPlan>
}