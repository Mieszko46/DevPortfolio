package com.gaska.runnercup.DI

import android.content.Context
import androidx.room.Room
import com.gaska.runnercup.data.local.PlanDB
import com.gaska.runnercup.data.local.dao.PlanDao
import com.gaska.runnercup.data.remote.PlanApi
import com.gaska.runnercup.repository.PlanRepo
import com.gaska.runnercup.repository.PlanRepolmpl
import com.gaska.runnercup.utils.Constants.BASE_URL
import com.gaska.runnercup.utils.SessionManager
import com.google.gson.Gson
import dagger.Module
import dagger.Provides
import dagger.hilt.InstallIn
import dagger.hilt.android.qualifiers.ApplicationContext
import dagger.hilt.components.SingletonComponent
import okhttp3.OkHttpClient
import okhttp3.logging.HttpLoggingInterceptor
import retrofit2.Retrofit
import retrofit2.converter.gson.GsonConverterFactory
import javax.inject.Singleton

@Module
@InstallIn(SingletonComponent::class)
object AppModule {

    @Singleton
    @Provides
    fun provideGson() = Gson()

    @Singleton
    @Provides
    fun provideSessionManager(
        @ApplicationContext context: Context
    ) = SessionManager(context)

    @Singleton
    @Provides
    fun providePlanDatabase(
        @ApplicationContext context: Context
    ): PlanDB = Room.databaseBuilder(
        context,
        PlanDB::class.java,
        "plans_db"
    ).build()

    @Singleton
    @Provides
    fun providePlanDao(
        planDB: PlanDB
    ) = planDB.getPlanDao()

    @Singleton
    @Provides
    fun providePlanApi(): PlanApi {
        val httpLoggingInterceptor = HttpLoggingInterceptor()
            .setLevel(HttpLoggingInterceptor.Level.BODY)

        val client = OkHttpClient.Builder()
            .addInterceptor(httpLoggingInterceptor)
            .build()

        return Retrofit.Builder()
            .baseUrl(BASE_URL)
            .client(client)
            .addConverterFactory(GsonConverterFactory.create())
            .build()
            .create(PlanApi::class.java)
    }

    @Singleton
    @Provides
    fun providePlanRepo(
        planApi: PlanApi,
        planDao: PlanDao,
        sessionManager: SessionManager
    ): PlanRepo {
        return PlanRepolmpl(
            planApi,
            planDao,
            sessionManager
        )
    }
}