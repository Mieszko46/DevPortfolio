package com.gaska.runnercup.UI.fragments

import android.content.Context
import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.EditText
import android.widget.TextView
import android.widget.Toast
import androidx.core.os.bundleOf
import androidx.fragment.app.activityViewModels
import androidx.fragment.app.setFragmentResult
import androidx.fragment.app.setFragmentResultListener
import androidx.lifecycle.lifecycleScope
import androidx.recyclerview.widget.StaggeredGridLayoutManager
import com.gaska.runnercup.Communicator
import com.gaska.runnercup.R
import com.gaska.runnercup.UI.PlanViewModel
import com.gaska.runnercup.UI.adapters.PlanAdapter
import com.gaska.runnercup.data.local.models.LocalPlan
import com.gaska.runnercup.databinding.FragmentPlansListBinding
import com.gaska.runnercup.databinding.FragmentPlansShowDetailBinding
import com.google.android.gms.maps.model.LatLng
import com.google.android.material.floatingactionbutton.FloatingActionButton
import kotlinx.coroutines.launch
import okhttp3.internal.wait

class PlansShowDetailFragment : Fragment() {

    private val map = MapFragment()
    private lateinit var plan: LocalPlan
    private lateinit var communicator: Communicator

    override fun onAttach(context: Context) {
        super.onAttach(context)
        communicator = context as Communicator
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        val stoper = StoperFragment()
        val transaction = childFragmentManager.beginTransaction()
        transaction.add(R.id.PlansDetailMapContainer, map)
        transaction.add(R.id.PlansDetailStoperContainer, stoper)
        transaction.addToBackStack(null)
        transaction.commit()
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val rootView = inflater.inflate(R.layout.fragment_plans_show_detail, container, false)

        val date = rootView.findViewById<TextView>(R.id.PlansDetailDate)
        loadPlan(date)

        val updateBtn = rootView.findViewById<Button>(R.id.PlansDetailUpdateBtn)
        updateBtn.setOnClickListener {
            Toast.makeText(context, "Modifying plan ${plan.date}", Toast.LENGTH_SHORT).show()
            communicator.PassPlanToFragment(plan, PlansAddFragment())
        }

        return rootView
    }

    private fun loadPlan(dateLabel: TextView) {
        plan = arguments?.getSerializable("BUNDLE_PLAN") as LocalPlan
        dateLabel.text = plan.date
        setFragmentResult("PlanToMap", bundleOf("plan" to plan))
        setFragmentResult("MapClickable", bundleOf("mapClickable" to false))
    }
}