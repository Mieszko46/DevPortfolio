package com.gaska.runnercup.UI.fragments

import android.content.Context
import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.fragment.app.activityViewModels
import androidx.lifecycle.lifecycleScope
import androidx.recyclerview.widget.ItemTouchHelper
import androidx.recyclerview.widget.RecyclerView
import androidx.recyclerview.widget.StaggeredGridLayoutManager
import com.gaska.runnercup.Communicator
import com.gaska.runnercup.R
import com.gaska.runnercup.UI.PlanViewModel
import com.gaska.runnercup.UI.adapters.PlanAdapter
import com.gaska.runnercup.databinding.FragmentPlansListBinding
import com.google.android.material.floatingactionbutton.FloatingActionButton
import com.google.android.material.snackbar.Snackbar
import dagger.hilt.android.AndroidEntryPoint
import kotlinx.coroutines.launch

@AndroidEntryPoint
class PlansListFragment : Fragment() {

    private var _binding: FragmentPlansListBinding? = null
    val binding: FragmentPlansListBinding?
        get() = _binding

    private lateinit var communicator: Communicator
    private lateinit var planAdapter: PlanAdapter
    private val planViewModel: PlanViewModel by activityViewModels()

    val itemTouchHelperCallback = object: ItemTouchHelper.SimpleCallback(
        0,
        ItemTouchHelper.LEFT or ItemTouchHelper.RIGHT
    ) {
        override fun onMove(
            recyclerView: RecyclerView,
            viewHolder: RecyclerView.ViewHolder,
            target: RecyclerView.ViewHolder
        ): Boolean {
            return true
        }

        override fun onSwiped(viewHolder: RecyclerView.ViewHolder, direction: Int) {
            val position = viewHolder.layoutPosition
            val plan = planAdapter.plans[position]
            planViewModel.deletePlan(plan.ID!!)
            Snackbar.make(
                requireView(),
                "Plan deleted successfully!",
                Snackbar.LENGTH_LONG
            ).apply {
                setAction(
                    "Undo "
                ) {
                    planViewModel.undoDelete(plan)
                }
                show()
            }
        }
    }

    override fun onAttach(context: Context) {
        super.onAttach(context)
        communicator = context as Communicator
    }

    override fun onResume() {
        super.onResume()
        (activity as AppCompatActivity).supportActionBar?.title = "Plans"
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val rootView = inflater.inflate(R.layout.fragment_plans_list, container, false)
        val newPlansFAB = rootView.findViewById<FloatingActionButton>(R.id.PlansListNewFAB)

        newPlansFAB.setOnClickListener{
            val transaction = requireActivity().supportFragmentManager.beginTransaction()
            transaction.replace(R.id.MainFragmentContainer, PlansAddFragment())
            transaction.addToBackStack(null)
            transaction.commit()
        }

        return rootView
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        _binding = FragmentPlansListBinding.bind(view)
        setupRecyclerView()
        subscribeToNotes()
        setupSwipeLayout()
        planViewModel.synchPlans()
    }

    private fun setupRecyclerView() {
        planAdapter = PlanAdapter()
        planAdapter.setOnItemClickListener {
            Toast.makeText(context, "Showing ${it.date}", Toast.LENGTH_SHORT).show()
            communicator.PassPlanToFragment(it, PlansShowDetailFragment())
        }
        binding?.PlansListRecyclerView?.apply {
            adapter = planAdapter
            layoutManager = StaggeredGridLayoutManager(1, StaggeredGridLayoutManager.VERTICAL)

            ItemTouchHelper(itemTouchHelperCallback)
                .attachToRecyclerView(this)
        }
    }

    private fun setupSwipeLayout() {
        binding?.PlansListSwipeRefresh?.setOnRefreshListener {
            println("1.")
            planViewModel.synchPlans {
                println("2.")
                binding?.PlansListSwipeRefresh?.isRefreshing = false
            }
        }
    }

    private fun subscribeToNotes() = lifecycleScope.launch {
        planViewModel.plans.collect {
            planAdapter.plans = it
        }
    }
}
