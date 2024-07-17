package com.gaska.repository

import com.gaska.data.model.Plan
import com.gaska.data.model.User
import com.gaska.data.table.PlansTable
import com.gaska.data.table.UserTable
import com.gaska.repository.DatabaseFactory.dbQuery
import org.jetbrains.exposed.sql.*
import kotlin.math.roundToInt

class Repo {

//  =================== USERS ===================

    suspend fun addUser(user: User){
        dbQuery {
            UserTable.insert { ut->
                ut[UserTable.email] = user.email
                ut[UserTable.password] = user.hashPassword
                ut[UserTable.name] = user.userName
            }
        }
    }

    suspend fun findUserByEmail(email:String) = dbQuery {
        UserTable.select { UserTable.email.eq(email) }
            .map { rowToUser(it) }
            .singleOrNull()
    }

    private fun rowToUser(row:ResultRow?):User? {
        if (row == null) {
            return null
        }
        return User(
            email = row[UserTable.email],
            hashPassword =  row[UserTable.password],
            userName =  row[UserTable.name]
        )
    }

//  =================== PLANS ===================

    suspend fun addPlan(plan: Plan, email: String) {
        dbQuery {
            PlansTable.insert { pt ->
                pt[PlansTable.userEmail] = email
                pt[PlansTable.date] = plan.date
                pt[PlansTable.distance] = plan.distance
                pt[PlansTable.startPointLat] = plan.startPointLat
                pt[PlansTable.startPointLong] = plan.startPointLong
                pt[PlansTable.endPointLat] = plan.endPointLat
                pt[PlansTable.endPointLong] = plan.endPointLong
            }
        }
    }

    suspend fun getAllPlans(email: String):List<Plan> = dbQuery {
        PlansTable.select{
            PlansTable.userEmail.eq(email)
        }.mapNotNull { rowToPlans(it) }
    }

    suspend fun updatePlan(plan: Plan, email: String){
        dbQuery {
            PlansTable.update(
                where = {
                    PlansTable.userEmail.eq(email) and PlansTable.ID.eq(plan.ID)
                }
            ){ pt ->
                pt[PlansTable.date] = plan.date
                pt[PlansTable.distance] = plan.distance
                pt[PlansTable.startPointLat] = plan.startPointLat
                pt[PlansTable.startPointLong] = plan.startPointLong
                pt[PlansTable.endPointLat] = plan.endPointLat
                pt[PlansTable.endPointLong] = plan.endPointLong
            }
        }
    }

    suspend fun deletePlan(id: Int, email: String){
        dbQuery {
            PlansTable.deleteWhere { PlansTable.userEmail.eq(email) and (PlansTable.ID eq id) }
        }
    }

    private fun rowToPlans(row:ResultRow?): Plan? {
        if (row == null){
            return null
        }

        return Plan(
            ID = row[PlansTable.ID],
            date = row[PlansTable.date],
            distance = row[PlansTable.distance],
            startPointLat = row[PlansTable.startPointLat],
            startPointLong = row[PlansTable.startPointLong],
            endPointLat = row[PlansTable.endPointLat],
            endPointLong = row[PlansTable.endPointLong]
        )
    }
}