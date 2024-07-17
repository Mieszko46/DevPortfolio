package com.gaska.data.table

import org.jetbrains.exposed.sql.Table

object PlansTable: Table() {

    val ID = integer("ID").autoIncrement()
    val userEmail = varchar("userEmail", 512).references(UserTable.email)
    val date =  text("date")
    val distance =  double("distance")
    val startPointLat = double("startPointLat")
    val startPointLong = double("startPointLong")
    val endPointLat = double("endPointLat")
    val endPointLong = double("endPointLong")

    override val primaryKey: PrimaryKey = PrimaryKey(ID)
}