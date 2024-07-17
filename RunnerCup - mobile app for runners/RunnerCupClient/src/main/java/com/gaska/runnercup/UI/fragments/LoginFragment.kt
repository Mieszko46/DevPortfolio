package com.gaska.runnercup.UI.fragments

import android.content.Context
import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.*
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.isVisible
import androidx.fragment.app.activityViewModels
import androidx.lifecycle.lifecycleScope
import com.gaska.runnercup.Communicator
import com.gaska.runnercup.R
import com.gaska.runnercup.UI.UserViewModel
import com.gaska.runnercup.utils.Result
import dagger.hilt.android.AndroidEntryPoint
import kotlinx.coroutines.launch

@AndroidEntryPoint
class LoginFragment : Fragment() {

    private lateinit var communicator: Communicator
    private val userViewModel: UserViewModel by activityViewModels()

    override fun onAttach(context: Context) {
        super.onAttach(context)
        communicator = context as Communicator
    }

    override fun onStop() {
        super.onStop()
        (activity as AppCompatActivity).supportActionBar?.show()
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val rootView = inflater.inflate(R.layout.fragment_login, container, false)

        val emailInput = rootView.findViewById<EditText>(R.id.LoginEmail)
        val nameInput = rootView.findViewById<EditText>(R.id.LoginName)
        val passwordInput = rootView.findViewById<EditText>(R.id.LoginPassword)
        val loginBtn = rootView.findViewById<Button>(R.id.LoginBtn)
        val registerBtn = rootView.findViewById<Button>(R.id.LoginRegisterBtn)

        subscribeToLoginEvents(rootView)

        loginBtn.setOnClickListener{
            val email = emailInput.text.toString()
            val name = nameInput.text.toString()
            val password = passwordInput.text.toString()

            userViewModel.loginUser(
                name.trim(),
                email.trim(),
                password.trim()
            )
        }

        registerBtn.setOnClickListener{
            val transaction = requireActivity().supportFragmentManager.beginTransaction()
            transaction.replace(R.id.LoginFragmentContainer, RegisterFragment())
            transaction.addToBackStack(null)
            transaction.commit()
        }

        return rootView
    }

    private fun subscribeToLoginEvents(rootView: View) = lifecycleScope.launch {
        userViewModel.loginState.collect { result ->
            when (result) {
                is Result.Success -> {
                    hideProgressBar(rootView)
                    Toast.makeText(
                        requireContext(),
                        "User successfully logged!",
                        Toast.LENGTH_SHORT
                    ).show()
                    communicator.PassLoginData("TEMP")
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
        val progressBar = rootView.findViewById<ProgressBar>(R.id.LoginProgressBar)
        progressBar.isVisible = true
    }


    private fun hideProgressBar(rootView: View){
        val progressBar = rootView.findViewById<ProgressBar>(R.id.LoginProgressBar)
        progressBar.isVisible = false
    }

}
