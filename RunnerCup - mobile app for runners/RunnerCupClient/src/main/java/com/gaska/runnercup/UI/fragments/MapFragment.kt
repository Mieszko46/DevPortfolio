package com.gaska.runnercup.UI.fragments

import android.annotation.SuppressLint
import android.app.Activity
import android.content.Context
import android.content.pm.PackageManager
import android.graphics.Color
import android.location.Location
import android.os.Bundle
import android.view.*
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.fragment.app.Fragment
import androidx.fragment.app.setFragmentResultListener
import com.beust.klaxon.JsonArray
import com.beust.klaxon.JsonObject
import com.beust.klaxon.Parser
import com.gaska.runnercup.BuildConfig.MY_API_KEY
import com.gaska.runnercup.R
import com.gaska.runnercup.data.local.models.LocalPlan
import com.google.android.gms.location.FusedLocationProviderClient
import com.google.android.gms.location.LocationServices
import com.google.android.gms.maps.CameraUpdateFactory
import com.google.android.gms.maps.GoogleMap
import com.google.android.gms.maps.OnMapReadyCallback
import com.google.android.gms.maps.SupportMapFragment
import com.google.android.gms.maps.model.*
import java.net.URL
import kotlin.math.roundToInt


class MapFragment : Fragment(), GoogleMap.OnMarkerClickListener, OnMapReadyCallback, GoogleMap.OnMapClickListener {

    private lateinit var mMap: GoogleMap
    var markerPoints: ArrayList<LatLng> = ArrayList()
    var distance: Double = 0.0
    private lateinit var lastLocation: Location
    private lateinit var fusedLocationClient: FusedLocationProviderClient
    private lateinit var FragmentContext: Context
    var distanceLabel: TextView? = null


    companion object {
        private const val LOCATION_REQUEST_CODE = 1
    }

    override fun onAttach(context: Context) {
        super.onAttach(context)
        FragmentContext = context
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setHasOptionsMenu(true)
        (activity as AppCompatActivity).supportActionBar?.title = "Map settings"

    }

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val rootView = inflater.inflate(R.layout.fragment_map, container, false)
        distanceLabel = rootView.findViewById(R.id.MapDistance)

        val mapFragment = childFragmentManager.findFragmentById(R.id.MapContainer) as SupportMapFragment?
        mapFragment?.getMapAsync(this)

        fusedLocationClient = LocationServices.getFusedLocationProviderClient(FragmentContext as Activity)

        return rootView
    }

    override fun onMapReady(googleMap: GoogleMap) {
        mMap = googleMap
        mMap.uiSettings.isZoomControlsEnabled = true
        mMap.setOnMarkerClickListener(this)
        setUpMap()
        chooseIfMapIsClickable()
        uploadPlan()
    }

    override fun onMapClick(position: LatLng) {
        if (markerPoints.size > 1) {
            markerPoints.clear()
            mMap.clear()
        }
        markerPoints.add(position)
        val options = MarkerOptions()
        options.position(position)
        options.draggable(true)

        if (markerPoints.size == 1) {
            options.icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_RED))
        } else if (markerPoints.size == 2) {
            options.icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_RED))
        }

        mMap.addMarker(options)
        if (markerPoints.size >= 2) {
            val origin = markerPoints[0]
            val dest = markerPoints[1]

            drawRoute(origin, dest)
            distance = CalculateDistance(origin, dest)
            distanceLabel?.text = "$distance km"
        }
    }

    override fun onMarkerClick(p0: Marker) = false

    private fun drawRoute(startPoint: LatLng, endPoint: LatLng) {
        val LatLongB = LatLngBounds.Builder()
        val options = PolylineOptions()
        options.color(Color.BLUE)
        options.width(10f)
        val url = getDirectionsUrl(startPoint, endPoint)
        var result: String
        val thread = Thread(){
            run{
                result = URL(url).readText()
            }
            activity?.runOnUiThread() {
                val parser: Parser = Parser()
                val stringBuilder: StringBuilder = StringBuilder(result)
                val json: JsonObject = parser.parse(stringBuilder) as JsonObject
                val routes = json.array<JsonObject>("routes")
                val points = routes!!["legs"]["steps"][0] as JsonArray<JsonObject>
                val polypts = points.flatMap { decodePoly(it.obj("polyline")?.string("points")!!)  }
                options.add(startPoint)
                LatLongB.include(startPoint)
                for (point in polypts)  {
                    options.add(point)
                    LatLongB.include(point)
                }
                options.add(endPoint)
                LatLongB.include(endPoint)
                val bounds = LatLongB.build()
                mMap.addPolyline(options)
                mMap.moveCamera(CameraUpdateFactory.newLatLngBounds(bounds, 100))
            }
        }
        thread.start()
    }

    private fun getDirectionsUrl(origin: LatLng, dest: LatLng): String {

        val str_origin = "origin=" + origin.latitude + "," + origin.longitude
        val str_dest = "destination=" + dest.latitude + "," + dest.longitude
        val sensor = "sensor=false"
        val mode = "mode=walking"
        val key = "key=$MY_API_KEY"
        val parameters = "$str_origin&$str_dest&$sensor&$mode&$key"
        val output = "json"
        println("https://maps.googleapis.com/maps/api/directions/$output?$parameters")
        return "https://maps.googleapis.com/maps/api/directions/$output?$parameters"
    }

    @SuppressLint("MissingPermission")
    fun setUpMap() {
        if (ActivityCompat.checkSelfPermission(
                FragmentContext,
                android.Manifest.permission.ACCESS_FINE_LOCATION
            ) != PackageManager.PERMISSION_GRANTED
        ) {
            ActivityCompat.requestPermissions(
                FragmentContext as Activity,
                arrayOf(android.Manifest.permission.ACCESS_FINE_LOCATION),
                LOCATION_REQUEST_CODE)
            return
        }
        mMap.isMyLocationEnabled = true
        fusedLocationClient.lastLocation.addOnSuccessListener(FragmentContext as Activity) { location ->
            if (location != null) {
                lastLocation = location
                val currentLatLong = LatLng(location.latitude, location.longitude)
                mMap.animateCamera(CameraUpdateFactory.newLatLngZoom(currentLatLong, 12f))
            }
        }
    }

    private fun chooseIfMapIsClickable() {
        requireParentFragment().setFragmentResultListener("MapClickable") { _, bundle ->
            val setMapClickable = bundle.getBoolean("mapClickable")
            if (setMapClickable)
                mMap.setOnMapClickListener(this)
            else
                mMap.setOnMapClickListener(null)
        }
    }

    private fun uploadPlan() {
        requireParentFragment().setFragmentResultListener("PlanToMap") { _, bundle ->
            val plan = bundle.getSerializable("plan") as? LocalPlan
            if (plan != null) {
//                println("uploaded plan: $plan")
                val startPosition = LatLng(plan.startPointLat!!, plan.startPointLong!!)
                val endPosition = LatLng(plan.endPointLat!!, plan.endPointLong!!)
                distance = plan.distance!!
                onMapClick(startPosition)
                onMapClick(endPosition)
            }
        }
    }

    override fun onCreateOptionsMenu(menu: Menu, inflater: MenuInflater) {
        super.onCreateOptionsMenu(menu, inflater)
        inflater.inflate(R.menu.map_options, menu)
    }

    override fun onOptionsItemSelected(item: MenuItem) = when (item.itemId) {
        R.id.normal_map -> {
            mMap.mapType = GoogleMap.MAP_TYPE_NORMAL
            true
        }
        R.id.hybrid_map -> {
            mMap.mapType = GoogleMap.MAP_TYPE_HYBRID
            true
        }
        R.id.satellite_map -> {
            mMap.mapType = GoogleMap.MAP_TYPE_SATELLITE
            true
        }
        R.id.terrain_map -> {
            mMap.mapType = GoogleMap.MAP_TYPE_TERRAIN
            true
        }
        else -> super.onOptionsItemSelected(item)
    }

    private fun decodePoly(encoded: String): List<LatLng> {
        val poly = ArrayList<LatLng>()
        var index = 0
        val len = encoded.length
        var lat = 0
        var lng = 0

        while (index < len) {
            var b: Int
            var shift = 0
            var result = 0
            do {
                b = encoded[index++].toInt() - 63
                result = result or (b and 0x1f shl shift)
                shift += 5
            } while (b >= 0x20)
            val dlat = if (result and 1 != 0) (result shr 1).inv() else result shr 1
            lat += dlat

            shift = 0
            result = 0
            do {
                b = encoded[index++].toInt() - 63
                result = result or (b and 0x1f shl shift)
                shift += 5
            } while (b >= 0x20)
            val dlng = if (result and 1 != 0) (result shr 1).inv() else result shr 1
            lng += dlng

            val p = LatLng(
                lat.toDouble() / 1E5,
                lng.toDouble() / 1E5
            )
            poly.add(p)
        }
        return poly
    }

    fun CalculateDistance(StartP: LatLng, EndP: LatLng): Double {
        val Radius = 6371 // radius of earth in Km
        val lat1 = StartP.latitude
        val lat2 = EndP.latitude
        val lon1 = StartP.longitude
        val lon2 = EndP.longitude
        val dLat = Math.toRadians(lat2 - lat1)
        val dLon = Math.toRadians(lon2 - lon1)
        val a = (Math.sin(dLat / 2) * Math.sin(dLat / 2)
                + (Math.cos(Math.toRadians(lat1))
                * Math.cos(Math.toRadians(lat2)) * Math.sin(dLon / 2)
                * Math.sin(dLon / 2)))
        val c = 2 * Math.asin(Math.sqrt(a))
        val valueResult = Radius * c
        val km = valueResult / 1
        val kmRound = (km * 1000.0).roundToInt() / 1000.0

        return kmRound
    }
}