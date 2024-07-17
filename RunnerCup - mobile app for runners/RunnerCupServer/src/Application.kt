package com.gaska

import com.gaska.authentication.JWTService
import com.gaska.authentication.hash
import com.gaska.data.model.User
import com.gaska.repository.DatabaseFactory
import com.gaska.repository.Repo
import com.gaska.routes.PlansRoutes
import com.gaska.routes.UserRoutes
import io.ktor.application.*
import io.ktor.response.*
import io.ktor.routing.*
import io.ktor.http.*
import io.ktor.sessions.*
import io.ktor.auth.*
import io.ktor.auth.jwt.*
import io.ktor.gson.*
import io.ktor.features.*
import io.ktor.locations.*

fun main(args: Array<String>): Unit = io.ktor.server.netty.EngineMain.main(args)

@Suppress("unused") // Referenced in application.conf
@kotlin.jvm.JvmOverloads
fun Application.module(testing: Boolean = false) {

    DatabaseFactory.init()
    val db = Repo()
    val jwtService = JWTService()
    val hashFunction = { s:String -> hash(s) }

    install(Sessions) {
        cookie<MySession>("MY_SESSION") {
            cookie.extensions["SameSite"] = "lax"
        }
    }

    install(Authentication) {
        jwt("jwt") {
            verifier(jwtService.varifier)
            realm = "RunnerCup Server"
            validate {
                val payload = it.payload
                val email = payload.getClaim("email").asString()
                val user = db.findUserByEmail(email)
                user
            }
        }
    }

    install(Locations)

    install(ContentNegotiation) {
        gson {
        }
    }

    routing {
        get("/") {
            call.respondText("HELLO WORLD!", contentType = ContentType.Text.Plain)
        }

        UserRoutes(db, jwtService, hashFunction)
        PlansRoutes(db, hashFunction)

        get("/token") {
            val email = call.request.queryParameters["email"]!!
            val password = call.request.queryParameters["password"]!!
            val username = call.request.queryParameters["username"]!!

            val user = User(email, hashFunction(password), username)
            call.respond(jwtService.generateToken(user))

        }

    }
}

data class MySession(val count: Int = 0)

