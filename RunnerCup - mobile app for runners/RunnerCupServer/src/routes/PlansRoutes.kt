package com.gaska.routes

import com.gaska.data.model.Plan
import com.gaska.data.model.SimpleResponse
import com.gaska.data.model.User
import com.gaska.repository.Repo
import io.ktor.application.*
import io.ktor.auth.*
import io.ktor.http.*
import io.ktor.locations.*
import io.ktor.request.*
import io.ktor.response.*
import io.ktor.routing.*
import java.lang.Exception

const val PLANS = "$API_VERSION/plans"
const val CREATE_PLANS = "$PLANS/create"
const val UPDATE_PLANS = "$PLANS/update"
const val DELETE_PLANS = "$PLANS/delete"

@Location(CREATE_PLANS)
class PlanCreateRoute

@Location(PLANS)
class PlanGetRoute

@Location(UPDATE_PLANS)
class PlanUpdateRoute

@Location(DELETE_PLANS)
class PlanDeleteRoute

fun Route.PlansRoutes(
    db:Repo,
    hashFunction: (String)->String
) {
    authenticate("jwt") {
        post<PlanCreateRoute> {
            val plan = try {
                call.receive<Plan>()
            } catch (e:Exception) {
                call.respond(HttpStatusCode.BadRequest, SimpleResponse(false, "Missing fields"))
                return@post
            }

            try {
                val email = call.principal<User>()!!.email
                db.addPlan(plan, email)
                call.respond(HttpStatusCode.OK, SimpleResponse(true, "Plan added successfully!"))
            } catch (e:Exception) {
                call.respond(HttpStatusCode.Conflict, SimpleResponse(false, e.message ?: "Some problem occurred"))
            }
        }

        get<PlanGetRoute> {
            try {
                val email = call.principal<User>()!!.email
                val plans = db.getAllPlans(email)
                call.respond(HttpStatusCode.OK,  plans)
            } catch (e:Exception) {
                call.respond(HttpStatusCode.Conflict, e.message ?: "Some problem occurred")
            }
        }

        post<PlanUpdateRoute> {
            val plan = try {
                call.receive<Plan>()
            } catch (e:Exception) {
                call.respond(HttpStatusCode.BadRequest, SimpleResponse(false, "Missing Fields"))
                return@post
            }

            try {
                val email = call.principal<User>()!!.email
                db.updatePlan(plan, email)
                call.respond(HttpStatusCode.OK, SimpleResponse(true, "Plan updated successfully!"))
            } catch (e:Exception) {
                call.respond(HttpStatusCode.Conflict, SimpleResponse(false, e.message ?: "Some problem occurred"))
            }
        }

        delete<PlanDeleteRoute> {
            val planID = try {
                call.request.queryParameters["id"]!!
            } catch (e:Exception) {
                call.respond(HttpStatusCode.BadRequest, SimpleResponse(false, "QueryParameter:id is not present"))
                return@delete
            }

            try {
                val email = call.principal<User>()!!.email
                db.deletePlan(planID.toInt(), email)
                call.respond(HttpStatusCode.OK, SimpleResponse(true, "Note deleted successfully!"))
            } catch (e:Exception) {
                call.respond(HttpStatusCode.Conflict, SimpleResponse(false, e.message ?: "Some problem occurred"))
            }
        }
    }
}