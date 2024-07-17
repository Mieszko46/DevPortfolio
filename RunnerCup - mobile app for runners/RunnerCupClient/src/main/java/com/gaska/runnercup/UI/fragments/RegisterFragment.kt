package com.gaska.runnercup.UI.fragments

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.*
import androidx.core.view.isVisible
import androidx.fragment.app.activityViewModels
import androidx.lifecycle.lifecycleScope
import com.gaska.runnercup.R
import com.gaska.runnercup.UI.UserViewModel
import com.gaska.runnercup.utils.Result
import dagger.hilt.android.AndroidEntryPoint
import kotlinx.coroutines.launch

@AndroidEntryPoint
class RegisterFragment : Fragment() {

    private val userViewModel: UserViewModel by activityViewModels()

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val rootView = inflater.inflate(R.layout.fragment_register, container, false)

        val emailInput = rootView.findViewById<EditText>(R.id.RegisterEmail)
        val nameInput = rootView.findViewById<EditText>(R.id.RegisterUsername)
        val passwordInput = rootView.findViewById<EditText>(R.id.RegisterPassword)
        val confirmPasswordInput = rootView.findViewById<EditText>(R.id.RegisterConfirmPassword)
        val registerBtn = rootView.findViewById<Button>(R.id.RegisterBtn)


        subscribeToRegisterEvents(rootView)

        registerBtn.setOnClickListener {
            val email = emailInput.text.toString()
            val name = nameInput.text.toString()
            val password = passwordInput.text.toString()
            val confirmPassword = confirmPasswordInput.text.toString()

            userViewModel.createUser(
                name.trim(),
                email.trim(),
                password.trim(),
                confirmPassword.trim()
            )
        }

        return rootView
    }

    private fun subscribeToRegisterEvents(rootView: View) = lifecycleScope.launch {
        userViewModel.registerState.collect { result ->
            when (result) {
                is Result.Success -> {
                    hideProgressBar(rootView)
                    Toast.makeText(
                        requireContext(),
                        "Account Successfully Created!",
                        Toast.LENGTH_SHORT
                    ).show()
                    requireActivity().supportFragmentManager.popBackStack()
                }
                is Result.Error -> {
                    hideProgressBar(rootView)
                    Toast.makeText(requireContext(), result.errorMessage, Toast.LENGTH_SHORT).show()
                }
                is Result.Loading -> {
                    showProgressBar(rootView)
                }
            }
        }
    }

    private fun showProgressBar(rootView: View){
        val progressBar = rootView.findViewById<ProgressBar>(R.id.RegisterProgressBar)
        progressBar.isVisible = true
    }


    private fun hideProgressBar(rootView: View){
        val progressBar = rootView.findViewById<ProgressBar>(R.id.RegisterProgressBar)
        progressBar.isVisible = false
    }
}