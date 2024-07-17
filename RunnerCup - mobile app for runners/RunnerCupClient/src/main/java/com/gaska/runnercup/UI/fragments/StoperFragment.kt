package com.gaska.runnercup.UI.fragments

import android.content.Context
import android.os.Build
import android.os.Bundle
import android.os.Handler
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.TextView
import androidx.annotation.RequiresApi
import com.gaska.runnercup.R

class StoperFragment : Fragment(), View.OnClickListener{

    private lateinit var FragmentContext: Context
    private var seconds: Int = 0
    private var isRunning: Boolean = false
    private var wasRunning: Boolean = false

    override fun onSaveInstanceState(savedInstanceState: Bundle) {
        super.onSaveInstanceState(savedInstanceState)
        savedInstanceState.putInt("seconds", seconds)
        savedInstanceState.putBoolean("isRunning", isRunning)
        savedInstanceState.putBoolean("wasRunning", wasRunning)
    }

    override fun onAttach(context: Context) {
        super.onAttach(context)
        FragmentContext = context
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        if (savedInstanceState != null) {
            seconds = savedInstanceState.getInt("seconds")
            isRunning = savedInstanceState.getBoolean("isRunning")
            wasRunning = savedInstanceState.getBoolean("wasRunning")
        }
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?): View? {
        val rootView = inflater.inflate(R.layout.fragment_stoper, container, false)

        runStoper(rootView)

        val btnStart = rootView.findViewById<Button>(R.id.StoperStartBtn)
        btnStart.setOnClickListener(this)
        val btnStop = rootView.findViewById<Button>(R.id.StoperStopBtn)
        btnStop.setOnClickListener(this)
        val btnReset = rootView.findViewById<Button>(R.id.StoperResetBtn)
        btnReset.setOnClickListener(this)
        val btnSave = rootView.findViewById<Button>(R.id.StoperSaveBtn)
        btnSave.setOnClickListener(this)

        return rootView
    }

    override fun onPause() {
        super.onPause()
        wasRunning = isRunning
        isRunning = false
    }

    override fun onResume() {
        super.onResume()
        if (wasRunning) {
            isRunning = true
        }
    }

    private fun runStoper(view: View) {
        val stoperText = view.findViewById<TextView>(R.id.StoperTextView)
        val handler = Handler()
        handler.post(object : Runnable {
            override fun run() {
                val hours = seconds / 3600
                val minutes = seconds % 3600 / 60
                val secs = seconds % 60
                val time = String.format("%d:%02d:%02d", hours, minutes, secs)
                stoperText.text = time
                if (isRunning) {
                    seconds++
                }
                handler.postDelayed(this, 1000)
            }
        })
    }

    private fun onClickStart() {
        isRunning = true
    }

    private fun onClickStop() {
        isRunning = false
    }

    private fun onClickReset() {
        isRunning = false
        seconds = 0
    }

    @RequiresApi(Build.VERSION_CODES.O)
    private fun onClickSave() {
        isRunning = false
//        sendDataToDB()
//        activity?.recreate()
    }

    @RequiresApi(Build.VERSION_CODES.O)
    override fun onClick(view: View?) {
        if (view != null) {
            when (view.id) {
                R.id.StoperStartBtn -> onClickStart()
                R.id.StoperStopBtn -> onClickStop()
                R.id.StoperResetBtn -> onClickReset()
                R.id.StoperSaveBtn -> onClickSave()
            }
        }
    }

//    @RequiresApi(Build.VERSION_CODES.O)
//    private fun sendDataToDB() {
//        val calories = (seconds * 0.25).toInt()
//        val pulse = (100..200).random()
//        val tempDate = LocalDate.now()
//        val formatter = DateTimeFormatter.ofPattern("dd-MM-yyyy")
//        val currDate = tempDate.format(formatter)
//        val db = DBHelper(FragmentContext)
//        val userName = arguments?.getString("user")!!
//        val trackName = arguments?.getString("track")!!
////        println("Calories: $calories \nPulse: $pulse \nDate: $formattedDate \nUser: $userName\nTrack: $trackName")
//
//        val scoreData = scoreData(
//            userName,
//            trackName,
//            seconds,
//            currDate.toString(),
//            0,
//            "",
//            pulse,
//            calories
//        )
//        db.addNewScore(scoreData)
////        db.printAllScoresData()
//    }
}