/**
 * service.js
 *
 * Computer Science 50
 * Problem Set 8
 *
 * Implements a shuttle service.
 */

//Stations on campus

var STATIONS = 
{
    "1": {lat: 42.40, lng: -71.20},
    "2": {lat: 42, lng: -71},
    "3": {lat: 42.381204, lng: -71.125273},
    "4": {lat: 42.367648, lng: -71.127929},
    "5": {lat: 42.378355, lng: -71.128041},
    "6": {lat: 42.379699, lng: -71.129859},
    "7": {lat: 42.38993, lng: -71.118238},
    "8": {lat: 42.371572, lng: -71.111343},
    "9": {lat: 42.362196, lng: -71.112376},
    "10": {lat: 42.34229, lng: -71.124839},
    "11": {lat: 42.371741, lng: -71.11698},
    "12": {lat: 42.372188, lng: -71.11255}
};

// default height
var HEIGHT = 0.8;

// default latitude
var LATITUDE = 42.3745615030193;

// default longitude
var LONGITUDE = -71.11803936751632;

// default heading
var HEADING = 1.757197490907891;

// default number of seats
var SEATS = 10;

// default velocity
var VELOCITY = 50;

// global reference to shuttle's marker on 2D map
var bus = null;

// global reference to 3D Earth
var earth = null;

// global reference to 2D map
var map = null;

// global reference to shuttle
var shuttle = null;

var markers = [];
var placemarks = [];
var pos = [];
var points = 0;

// load version 1 of the Google Earth API
google.load("earth", "1");

// load version 3 of the Google Maps API
google.load("maps", "3", {other_params: "sensor=false"});

// once the window has loaded
$(window).load(function() {

    // listen for keydown anywhere in body
    $(document.body).keydown(function(event) {
        return keystroke(event, true);
    });

    // listen for keyup anywhere in body
    $(document.body).keyup(function(event) {
        return keystroke(event, false);
    });

    // listen for click on Drop Off button
    $("#dropoff").click(function(event) {
        dropoff();
    });

    // listen for click on Pick Up button
    $("#pickup").click(function(event) {
        pickup();
    });
    
    // listen for click on Super velocity button
    $("#teleport").click(function(event) {
        teleport();
    });

    $("#refuel").click(function(event) {
        refuel();
    });
    
    // load application
    load();
});

// Fill the bus

function refuel()
{
    var refueled = false;
    for (var station in STATIONS)
    {
        if (shuttle.distance(STATIONS[station].lat, STATIONS[station].lng) < 15)
        {
            shuttle.distanceTraveled = 0;
            $("#announcements").html("The bus is now refueled!");
            refueled = true;
        }
        else if((station == "12") && (refueled == false))
        {
            $("#announcements").html("No gas station nearby!");
        }
    }
}

// Teleport to a selected house

function teleport()
{
    var selected = document.getElementById('opciones');
    var selectedOption = selected.options[selected.selectedIndex].value;
    for (var house in HOUSES)
    {
        if(house == selectedOption)
        {
            shuttle.position.latitude = HOUSES[house].lat;
            shuttle.position.longitude = HOUSES[house].lng;
            shuttle.localAnchorCartesian = V3.latLonAltToCartesian([shuttle.position.latitude, shuttle.position.longitude, 0]);
        }
    }
}

// unload application
$(window).unload(function() {
    unload();
});

/**
 * Renders seating chart.
 */
function chart()
{
    var html = "<ol start='0'>";
    for (var i = 0; i < shuttle.seats.length; i++)
    {
        if (shuttle.seats[i] == null)
        {
            html += "<li>Empty Seat</li>";
        }
        else
        {
            html += "<li>" + shuttle.seats[i].name + " goes to " + shuttle.seats[i].house + "</li>";
        }
    }
    html += "</ol>";
    $("#chart").html(html);
}

/**
 * Drops up passengers if their stop is nearby.
 */
function dropoff()
{
    if(points == PASSENGERS.length)
    {
        $("#announcements").html("You have collected all the passengers!");
        return;
    }
    else
    {   
        for(var i = 0; i < shuttle.seats.length; i++)
        {   
            if(shuttle.seats[i] != null)
            {
                if(shuttle.distance(HOUSES[shuttle.seats[i].house].lat, HOUSES[shuttle.seats[i].house].lng) < 30)
                {
                    shuttle.seats[i] = null;
                    points = points + 1;
                    $("#announcements").html("Score: " + points);
                    break;
                }
                else
                {
                    $("#announcements").html("Nobody on the bus for this house!");
                }
            }
            else
            {
                $("#announcements").html("The bus is empty! Go and pick up more persons!");
            }
        }
    }
    chart();
}

/**
 * Called if Google Earth fails to load.
 */
function failureCB(errorCode) 
{
    // report error unless plugin simply isn't installed
    if (errorCode != ERR_CREATE_PLUGIN)
    {
        alert(errorCode);
    }
}

/**
 * Handler for Earth's frameend event.
 */
function frameend() 
{
    shuttle.update();
}

/**
 * Called once Google Earth has loaded.
 */
function initCB(instance) 
{
    // retain reference to GEPlugin instance
    earth = instance;

    // specify the speed at which the camera moves
    earth.getOptions().setFlyToSpeed(100);

    // show buildings
    earth.getLayerRoot().enableLayerById(earth.LAYER_BUILDINGS, true);

    // disable terrain (so that Earth is flat)
    earth.getLayerRoot().enableLayerById(earth.LAYER_TERRAIN, false);

    // prevent mouse navigation in the plugin
    earth.getOptions().setMouseNavigationEnabled(false);

    // instantiate shuttle
    shuttle = new Shuttle({
        heading: HEADING,
        height: HEIGHT,
        latitude: LATITUDE,
        longitude: LONGITUDE,
        planet: earth,
        seats: SEATS,
        velocity: VELOCITY
    });

    // synchronize camera with Earth
    google.earth.addEventListener(earth, "frameend", frameend);

    // synchronize map with Earth
    google.earth.addEventListener(earth.getView(), "viewchange", viewchange);

    // update shuttle's camera
    shuttle.updateCamera();

    // show Earth
    earth.getWindow().setVisibility(true);

    // render seating chart
    chart();

    // populate Earth with passengers and houses
    populate();
}

/**
 * Handles keystrokes.
 */
function keystroke(event, state)
{
    // ensure we have event
    if (!event)
    {
        event = window.event;
    }

    // left arrow
    if (event.keyCode == 37)
    {
        shuttle.states.turningLeftward = state;
        return false;
    }

    // up arrow
    else if (event.keyCode == 38)
    {
        shuttle.states.tiltingUpward = state;
        return false;
    }

    // right arrow
    else if (event.keyCode == 39)
    {
        shuttle.states.turningRightward = state;
        return false;
    }

    // down arrow
    else if (event.keyCode == 40)
    {
        shuttle.states.tiltingDownward = state;
        return false;
    }

    // A, a
    else if (event.keyCode == 65 || event.keyCode == 97)
    {
        shuttle.states.slidingLeftward = state;
        return false;
    }

    // D, d
    else if (event.keyCode == 68 || event.keyCode == 100)
    {
        shuttle.states.slidingRightward = state;
        return false;
    }
  
    // S, s
    else if (event.keyCode == 83 || event.keyCode == 115)
    {
        shuttle.states.movingBackward = state;     
        return false;
    }

    // W, w
    else if (event.keyCode == 87 || event.keyCode == 119)
    {
        if(shuttle.distanceTraveled <= 3000)
        {
            shuttle.states.movingForward = state;
            $("#announcements").html("Out of fuel in: " + (3000 - shuttle.distanceTraveled) +" meters");
        }
        else
        {
            $("#announcements").html("You are out of fuel!");
        }
        return false;
    }
    return true;
}

/**
 * Loads application.
 */
 
function load()
{
    // embed 2D map in DOM
    var latlng = new google.maps.LatLng(LATITUDE, LONGITUDE);
    map = new google.maps.Map($("#map").get(0), {
        center: latlng,
        disableDefaultUI: true,
        mapTypeId: google.maps.MapTypeId.ROADMAP,
        scrollwheel: false,
        zoom: 17,
        zoomControl: true
    });

    // prepare shuttle's icon for map
    bus = new google.maps.Marker({
        icon:"http://maps.gstatic.com/intl/en_us/mapfiles/ms/micons/bus.png",
        map: map,
        title: "you are here"    
    });
    
    // embed 3D Earth in DOM
    google.earth.createInstance("earth", initCB, failureCB);
}

/**
 * Picks up nearby passengers.
 */
function pickup()
{
    var seat = emptySeat();
    
    if (seat == -1)
    {
        $("#announcements").html("The bus is full!");
        return;
    }
    else
    {
        for (var i = 0; i < PASSENGERS.length; i++)
        {
            if (seat != -1)
            {
                if (shuttle.distance(pos[i].lat, pos[i].long) < 15)
                {
                    earth.getFeatures().removeChild(placemarks[i]);
                    placemarks[i] = null;
                    markers[i].setMap(null);
                    shuttle.seats[seat] = PASSENGERS[i];
                    $("#announcements").html("");
                }
                else
                {
                    $("#announcements").html("The person is out of range or nobody is nearby!");
                }
            }
            seat = emptySeat();
        }
     }
     $("#announcements").html("");
    chart();
}

function emptySeat()
{
    for (var i = 0; i < shuttle.seats.length; i++){
        if (shuttle.seats[i] == null)
            return i;
    }
    return -1;
}
/**
 * Populates Earth with passengers and houses.
 */
function populate()
{
    // mark houses
    for (var house in HOUSES)
    {
        // plant house on map
        new google.maps.Marker({
            icon: "https://google-maps-icons.googlecode.com/files/home.png",
            map: map,
            position: new google.maps.LatLng(HOUSES[house].lat, HOUSES[house].lng),
            title: house
        });
    }

    // get current URL, sans any filename
    var url = window.location.href.substring(0, (window.location.href.lastIndexOf("/")) + 1);

    // scatter passengers
    for (var i = 0; i < PASSENGERS.length; i++)
    {
        // pick a random building
        var building = BUILDINGS[Math.floor(Math.random() * BUILDINGS.length)];

        // prepare placemark
        var placemark = earth.createPlacemark("");
        placemark.setName(PASSENGERS[i].name + " to " + PASSENGERS[i].house);

        // prepare icon
        var icon = earth.createIcon("");
        icon.setHref(url + "/img/" + PASSENGERS[i].username + ".jpg");

        // prepare style
        var style = earth.createStyle("");
        style.getIconStyle().setIcon(icon);
        style.getIconStyle().setScale(4.0);

        // prepare stylemap
        var styleMap = earth.createStyleMap("");
        styleMap.setNormalStyle(style);
        styleMap.setHighlightStyle(style);

        // associate stylemap with placemark
        placemark.setStyleSelector(styleMap);

        // prepare point
        var point = earth.createPoint("");
        point.setAltitudeMode(earth.ALTITUDE_RELATIVE_TO_GROUND);
        point.setLatitude(building.lat);
        point.setLongitude(building.lng);
        point.setAltitude(0.0);

        // associate placemark with point
        placemark.setGeometry(point);

        // add placemark to Earth
        earth.getFeatures().appendChild(placemark);

        // add marker to map
        var marker = new google.maps.Marker({
            icon: "https://maps.gstatic.com/intl/en_us/mapfiles/ms/micons/man.png",
            map: map,
            position: new google.maps.LatLng(building.lat, building.lng),
            title: PASSENGERS[i].name + " at " + building.name
        });
        
        markers.push(marker);       
        placemarks.push(placemark);
        pos.push({long: building.lng, lat: building.lat})
    }
    
    for (var station in STATIONS)
    {
        var station = new google.maps.Marker({
            icon: "http://google-maps-icons.googlecode.com/files/gazstation.png",
            map: map,
            position: new google.maps.LatLng(STATIONS[station].lat, STATIONS[station].lng),
        });   
    }
}

/**
 * Handler for Earth's viewchange event.
 */
function viewchange() 
{
    // keep map centered on shuttle's marker
    var latlng = new google.maps.LatLng(shuttle.position.latitude, shuttle.position.longitude);
    map.setCenter(latlng);
    bus.setPosition(latlng);
}

/**
 * Unloads Earth.
 */
function unload()
{
    google.earth.removeEventListener(earth.getView(), "viewchange", viewchange);
    google.earth.removeEventListener(earth, "frameend", frameend);
}
