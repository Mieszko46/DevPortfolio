package com.gaska.runnercup.UI.fragments

import android.content.Context
import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.fragment.app.activityViewModels
import androidx.lifecycle.lifecycleScope
import com.gaska.runnercup.Communicator
import com.gaska.runnercup.R
import com.gaska.runnercup.UI.UserViewModel
import com.gaska.runnercup.utils.Result
import dagger.hilt.android.AndroidEntryPoint
import kotlinx.coroutines.launch

@AndroidEntryPoint
class ProfileFragment : Fragment() {

    private lateinit var communicator: Communicator
    private val userViewModel:UserViewModel by activityViewModels()

    override fun onAttach(context: Context) {
        super.onAttach(context)
        communicator = context as Communicator
    }

    override fun onStart() {
        super.onStart()
        userViewModel.getCurrentUser()
    }

    override fun onResume() {
        super.onResume()
        (activity as AppCompatActivity).supportActionBar?.title = "Profile"
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val rootView = inflater.inflate(R.layout.fragment_profile, container, false)

        subscribeToCurrentUserEvents(rootView)

        val logoutBtn = rootView.findViewById<Button>(R.id.ProfileLogoutBtn)
        logoutBtn.setOnClickListener{
            communicator.Logout()
        }
        return rootView
    }

    private fun subscribeToCurrentUserEvents(rootView: View) = lifecycleScope.launch {
        val email = rootView.findViewById<TextView>(R.id.ProfileEmailText)
        val name = rootView.findViewById<TextView>(R.id.ProfileUsernameText)
        userViewModel.currentUserState.collect { result ->
            when (result) {
                is Result.Success -> {
                    email.text = result.data?.name ?: "No data"
                    name.text = result.data?.email ?: "No data"
                }
                is Result.Error -> {
                    Toast.makeText(requireContext(), result.errorMessage, Toast.LENGTH_SHORT).show()
                }
            }
        }
    }
}
