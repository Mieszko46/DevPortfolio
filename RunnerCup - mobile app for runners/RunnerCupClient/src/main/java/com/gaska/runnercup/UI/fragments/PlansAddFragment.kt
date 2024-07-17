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
import com.gaska.runnercup.Communicator
import com.gaska.runnercup.R
import com.gaska.runnercup.UI.PlanViewModel
import com.gaska.runnercup.UI.UserViewModel
import com.gaska.runnercup.data.local.models.LocalPlan
import com.gaska.runnercup.databinding.FragmentPlansAddBinding
import dagger.hilt.android.AndroidEntryPoint

@AndroidEntryPoint
class PlansAddFragment : Fragment() {

    private var plan: LocalPlan? = null
    val planViewModel: PlanViewModel by activityViewModels()
    private lateinit var rootView: View
    private lateinit var communicator: Communicator
    private val map = MapFragment()

    override fun onAttach(context: Context) {
        super.onAttach(context)
        communicator = context as Communicator
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {
        rootView = inflater.inflate(R.layout.fragment_plans_add, container, false)

        val transaction = childFragmentManager.beginTransaction()
        transaction.add(R.id.PlansAddMapFragmentContainer, map)
        transaction.commit()

        val saveBtn = rootView.findViewById<Button>(R.id.PlansAddSaveBtn)
        saveBtn.setOnClickListener {
            activity?.recreate()
        }

        return rootView
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        val dateInput = rootView.findViewById<EditText>(R.id.PlansAddDateInput)

        plan = arguments?.getSerializable("BUNDLE_PLAN") as? LocalPlan
        setFragmentResult("PlanToMap", bundleOf("plan" to plan))
        setFragmentResult("MapClickable", bundleOf("mapClickable" to true))

        planViewModel.oldPlan = plan
        planViewModel.oldPlan?.date.let {
            dateInput.setText(it)
        }
    }

    override fun onPause() {
        super.onPause()
        if (planViewModel.oldPlan == null) {
            createPlan()
        } else {
            updatePlan()
        }
    }

    private fun createPlan() {
        val planDateInput = rootView.findViewById<EditText>(R.id.PlansAddDateInput)
        val planDate = planDateInput.text.toString()

        if (planDate.isEmpty()) {
            Toast.makeText(requireContext(), "Date field is empty", Toast.LENGTH_SHORT).show()
            return
        }

        val distance  = map.distance
        val markers = map.markerPoints

        planViewModel.createPlan(
            planDate,
            distance,
            markers[0].latitude,
            markers[0].longitude,
            markers[1].latitude,
            markers[1].longitude
        )
    }

    private fun updatePlan() {
        val planDateInput = rootView.findViewById<EditText>(R.id.PlansAddDateInput)
        val planDate = planDateInput.text.toString().trim()

//        val bundle = Bundle()
//        plan?.date = planDate
//        bundle.putSerializable("BUNDLE_PLAN", plan)
//        PlansShowDetailFragment().arguments = bundle
//        println(plan)

        if (planDate.isNullOrEmpty()) {
            planViewModel.deletePlan(planViewModel.oldPlan!!.ID!!)
            return
        }

        val distance  = map.distance
        val markers = map.markerPoints

        planViewModel.updatePlan(
            planDate,
            distance,
            markers[0].latitude,
            markers[0].longitude,
            markers[1].latitude,
            markers[1].longitude
        )
    }

}
