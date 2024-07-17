package com.gaska.runnercup.UI.fragments

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import com.gaska.runnercup.R
import com.gaska.runnercup.databinding.FragmentAnalysisShowDetailBinding

class AnalysisShowDetailFragment : Fragment() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        val map = MapFragment()
        val transaction = childFragmentManager.beginTransaction()
        transaction.add(R.id.AnalysisDetailMapContainer, map)
        transaction.addToBackStack(null)
        transaction.commit()
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val rootView = inflater.inflate(R.layout.fragment_analysis_show_detail, container, false)
        return rootView
    }
}