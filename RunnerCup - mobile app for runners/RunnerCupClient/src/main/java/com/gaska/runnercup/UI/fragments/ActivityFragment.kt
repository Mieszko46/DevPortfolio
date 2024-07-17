package com.gaska.runnercup.UI.fragments

import android.os.Bundle
import android.view.*
import androidx.core.os.bundleOf
import androidx.fragment.app.Fragment
import androidx.fragment.app.setFragmentResult
import com.gaska.runnercup.R
import com.gaska.runnercup.databinding.FragmentActivityBinding

class ActivityFragment : Fragment(){

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        val map = MapFragment()
        val stoper = StoperFragment()
        val transaction = childFragmentManager.beginTransaction()
        transaction.add(R.id.ActivityStoperContainer, stoper)
        transaction.add(R.id.ActivityMapFragmentContainer, map)
        transaction.commit()
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val rootView = inflater.inflate(R.layout.fragment_activity, container, false)
        setFragmentResult("MapClickable", bundleOf("mapClickable" to true))
        return rootView
    }
}