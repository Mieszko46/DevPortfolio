package com.gaska.runnercup.UI.adapters

import android.view.LayoutInflater
import android.view.ViewGroup
import androidx.core.view.isVisible
import androidx.recyclerview.widget.AsyncListDiffer
import androidx.recyclerview.widget.DiffUtil
import androidx.recyclerview.widget.RecyclerView
import com.gaska.runnercup.R
import com.gaska.runnercup.data.local.models.LocalPlan
import com.gaska.runnercup.databinding.ItemListBinding

class PlanAdapter: RecyclerView.Adapter<PlanAdapter.PlanViewHolder>() {

    class PlanViewHolder(val binding: ItemListBinding):RecyclerView.ViewHolder(binding.root)

    val diffUtil = object: DiffUtil.ItemCallback<LocalPlan>(){
        override fun areItemsTheSame(oldItem: LocalPlan, newItem: LocalPlan): Boolean {
            return oldItem.ID == newItem.ID
        }

        override fun areContentsTheSame(oldItem: LocalPlan, newItem: LocalPlan): Boolean {
            return oldItem == newItem
        }
    }

    val differ = AsyncListDiffer(this, diffUtil)
    var plans: List<LocalPlan>
        get() = differ.currentList
        set(value) = differ.submitList(value)

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): PlanViewHolder {
        return PlanViewHolder(
            ItemListBinding.inflate(
                LayoutInflater.from(parent.context),
                parent,
                false
            )
        )
    }

    override fun onBindViewHolder(holder: PlanViewHolder, position: Int) {
        val plan = plans[position]
        holder.binding.apply {
             itemDate.isVisible = plan.date != null

            plan.date?.let {
                itemDate.text = it
            }

            itemSync.setBackgroundResource(
                if (plan.connected) R.drawable.synced
                else R.drawable.not_sync
            )

            root.setOnClickListener {
                onItemClickListener?.invoke(plan)
            }
        }
    }

    private var onItemClickListener: ((LocalPlan) -> Unit)? = null
    fun setOnItemClickListener(listener: (LocalPlan) -> Unit) {
        onItemClickListener = listener
    }

    override fun getItemCount(): Int {
        return plans.size
    }
}