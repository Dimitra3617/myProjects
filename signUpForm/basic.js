var lat1;
var lng1;
var city;
var street;
var country;
var nameofuser;
var lat;
var lon;


function checkPasswordMatch() {
    var password = document.getElementById('psw');
    var confirm_psw = document.getElementById('confirm_psw');
   
    var message = document.getElementById('confirmMessage');
    
    var correct = "#66cc66";
    var wrong = "red";

    if(password.value == confirm_psw.value){
        confirm_psw.style.backgroundColor = correct;
        message.style.color = correct;
        message.innerHTML = "Passwords Match!"
    }
    else{
        confirm_psw.style.backgroundColor = wrong;
        message.style.color = wrong;
        message.innerHTML = "Passwords Do Not Match!";
    }
}

function validateLocation() {
    var Location=document.getElementById('country').value + '+' + document.getElementById('city').value + '+' + document.getElementById('address').value;
    var url = 'https://nominatim.openstreetmap.org/search?format=json&addressdetails=1&polygon=1&q=' + Location;
    
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) { 
            var response = JSON.parse(this.responseText);
        
            if(response.length != 0) {
                document.getElementById('demo').innerHTML = "Correct address!";
                document.getElementById('demo').style.color = 'green';
                lat = response["0"].lat;
                lon = response["0"].lon;

            }
            else {
                document.getElementById('demo').innerHTML = "Wrong address!";
                document.getElementById('demo').style.color = 'red';
            }
        }
    };
    xhttp.open('GET', url, true);
    xhttp.send();
}

function showMap() {
    document.getElementById("mapdiv").innerHTML="";
    document.getElementById("mapdiv").style.height = "400px";
    map = new OpenLayers.Map("mapdiv");
    map.addLayer(new OpenLayers.Layer.OSM());

    var lonLat = new OpenLayers.LonLat( lon , lat)
          .transform(
            new OpenLayers.Projection("EPSG:4326"), 
            map.getProjectionObject() 
          );
          
    var zoom=16;

    var markers = new OpenLayers.Layer.Markers( "Markers" );
    map.addLayer(markers);
    
    markers.addMarker(new OpenLayers.Marker(lonLat));

    map.setCenter (lonLat, zoom);

}

function findLocation() {
    var x = document.getElementById("getLocation");
    if (navigator.geolocation) {
        navigator.geolocation.getCurrentPosition(showPosition);
    }
    else 
        x.innerHTML = "Geolocation is not supported by this browser.";

    function showPosition(position) {
        x.innerHTML = "Latitude: " + position.coords.latitude + 
        "<br>Longitude: " + position.coords.longitude;
        lat = position.coords.latitude;
        lon = position.coords.longitude;
        showMap();
        var url = 'https://nominatim.openstreetmap.org/reverse?format=json&zoom=18&addressdetails=1&lat=' + lat + '&lon=' + lon;
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) { 
            var response = JSON.parse(this.responseText);
            console.log(response);
            if(response.length != 0) {
                country = response.address.country_code.toUpperCase(); 
                city = response.address.city;
                street = response.address.road;
                document.getElementById('country').value = country;
                document.getElementById('city').value = city;
                document.getElementById('address').value = street;
            }
            else {
                document.getElementById('demo').innerHTML = "Wrong address!";
                document.getElementById('demo').style.color = 'red';
            }
        }
        };
        xhttp.open('GET', url, true);
        xhttp.send();
    }
}

function Username_ajaxPOST() {
    
    var xhr = new XMLHttpRequest();
    xhr.open("POST", 'ServletUname?Username=' + document.getElementById('user').value);

    xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    
    xhr.onload = function() {
    if (this.readyState == 4 && this.status == 200) { 
        
        document.getElementById("validate_uname").style.display = "block";
        document.getElementById("validate_uname").style.display = "none";
       
        if(xhr.responseText != 'true') {
            document.getElementById('validate_uname').style.display = "block";
            document.getElementById('validate_uname').style.color = 'red';
        }
        if(xhr.responseText == 'false1') 
            document.getElementById('validate_uname').innerHTML = "This field is required.";
        else if (xhr.responseText == 'false2') 
            document.getElementById('validate_uname').innerHTML = "Wrong regex for Username.";
        else if (xhr.responseText == 'false3')
            document.getElementById('validate_uname').innerHTML = "This username is used.";
    }
    };
    
    xhr.send();
}

function Email_ajaxPOST() {
    var xhr = new XMLHttpRequest();
    xhr.open("POST", 'ServletEmail?Email=' + document.getElementById('Email').value);

    xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    
    xhr.onload = function() {
    if (this.readyState == 4 && this.status == 200) { 
        
        document.getElementById("validate_email").style.display = "block";
        document.getElementById("validate_email").style.display = "none";
       
        if(xhr.responseText != 'true') {
            document.getElementById('validate_email').style.display = "block";
            document.getElementById('validate_email').style.color = 'red';
        }
        if(xhr.responseText == 'false1') 
            document.getElementById('validate_email').innerHTML = "This field is required.";
        else if (xhr.responseText == 'false2') 
            document.getElementById('validate_email').innerHTML = "Wrong regex for Email.";
        else if (xhr.responseText == 'false3')
            document.getElementById('validate_email').innerHTML = "This email is used.";
    }
    };
    
    xhr.send();
}

function Password_ajaxPOST() {
    var xhr = new XMLHttpRequest();
    xhr.open("POST", 'ServletPassword?Password=' + document.getElementById('psw').value);

    xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    
    xhr.onload = function() {
    if (this.readyState == 4 && this.status == 200) { 
        
        document.getElementById("validate_password").style.display = "block";
        document.getElementById("validate_password").style.display = "none";
       
        if(xhr.responseText != 'true') {
            document.getElementById('validate_password').style.display = "block";
            document.getElementById('validate_password').style.color = 'red';
        }
        if(xhr.responseText == 'false1') 
            document.getElementById('validate_password').innerHTML = "This field is required.";
        else if (xhr.responseText == 'false2') 
            document.getElementById('validate_password').innerHTML = "Wrong regex for Password.";
    }
    };
    
    xhr.send();
}

function Confirmpsw_ajaxPOST() {
    var xhr = new XMLHttpRequest();
    xhr.open("POST", 'ServletConfirmPassword?Confirm_Password=' + document.getElementById('confirm_psw').value 
    + '&Password=' + document.getElementById('psw').value);
    xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    
    xhr.onload = function() {
    if (this.readyState == 4 && this.status == 200) { 
        
        document.getElementById("validate_confpassword").style.display = "block";
        document.getElementById("validate_confpassword").style.display = "none";
  
        if(xhr.responseText != 'true') {
            document.getElementById('validate_confpassword').style.display = "block";
            document.getElementById('validate_confpassword').style.color = 'red';
        }
        if(xhr.responseText == 'false1') 
            document.getElementById('validate_confpassword').innerHTML = "This field is required.";
        else if (xhr.responseText == 'false2') 
            document.getElementById('validate_confpassword').innerHTML = "Please type again...";
    }
    };
    
    xhr.send();
}

function firstname_ajaxPOST() {
    var xhr = new XMLHttpRequest();
    xhr.open("POST", 'ServletFirstname?First_name=' + document.getElementById("first_name").value);
    xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    
    xhr.onload = function() {
    if (this.readyState == 4 && this.status == 200) { 
        
        document.getElementById("validate_firstname").style.display = "block";
        document.getElementById("validate_firstname").style.display = "none";
  
        if(xhr.responseText != 'true') {
            document.getElementById('validate_firstname').style.display = "block";
            document.getElementById('validate_firstname').style.color = 'red';
        }
        if(xhr.responseText == 'false1') 
            document.getElementById('validate_firstname').innerHTML = "This field is required.";
        else if (xhr.responseText == 'false2') 
            document.getElementById('validate_firstname').innerHTML = "Wrong regex for First name.";
    }
    };
    
    xhr.send();
}

function lastname_ajaxPOST() {
    var xhr = new XMLHttpRequest();
    xhr.open("POST", 'ServletLastname?Lastname=' + document.getElementById("last_name").value);
    xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    
    xhr.onload = function() {
    if (this.readyState == 4 && this.status == 200) { 
        
        document.getElementById("validate_lastname").style.display = "block";
        document.getElementById("validate_lastname").style.display = "none";
  
        if(xhr.responseText != 'true') {
            document.getElementById('validate_lastname').style.display = "block";
            document.getElementById('validate_lastname').style.color = 'red';
        }
        if(xhr.responseText == 'false1') 
            document.getElementById('validate_lastname').innerHTML = "This field is required.";
        else if (xhr.responseText == 'false2') 
            document.getElementById('validate_lastname').innerHTML = "Wrong regex for Last name.";
    }
    };
    
    xhr.send();
}

function city_ajaxPOST() {
    var xhr = new XMLHttpRequest();
    xhr.open("POST", 'ServletCity?City=' + document.getElementById("city").value);
    xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    
    xhr.onload = function() {
    if (this.readyState == 4 && this.status == 200) { 
        
        document.getElementById("validate_city").style.display = "block";
        document.getElementById("validate_city").style.display = "none";
  
        if(xhr.responseText != 'true') {
            document.getElementById('validate_city').style.display = "block";
            document.getElementById('validate_city').style.color = 'red';
        }
        if(xhr.responseText == 'false1') 
            document.getElementById('validate_city').innerHTML = "This field is required.";
        else if (xhr.responseText == 'false2') 
            document.getElementById('validate_city').innerHTML = "Wrong regex for City.";
    }
    };
    
    xhr.send();
}

function function_calls() {
    validateLocation(); 
    city_ajaxPOST();
}

function job_ajaxPOST() {
    var xhr = new XMLHttpRequest();
    xhr.open("POST", 'ServletJob?Job=' + document.getElementById("job").value);
    xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    
    xhr.onload = function() {
    if (this.readyState == 4 && this.status == 200) { 
        
        document.getElementById("validate_job").style.display = "block";
        document.getElementById("validate_job").style.display = "none";
  
        if(xhr.responseText != 'true') {
            document.getElementById('validate_job').style.display = "block";
            document.getElementById('validate_job').style.color = 'red';
        }
        if(xhr.responseText == 'false1') 
            document.getElementById('validate_job').innerHTML = "This field is required.";
        else if (xhr.responseText == 'false2') 
            document.getElementById('validate_job').innerHTML = "Wrong regex for Job.";
    }
    };
    
    xhr.send();
}

function interests_ajaxPOST() {
    var xhr = new XMLHttpRequest();
    xhr.open("POST", 'ServletInterests?Interests=' + document.getElementById("interests").value);
    xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    
    xhr.onload = function() {
    if (this.readyState == 4 && this.status == 200) { 
        
        document.getElementById("validate_interests").style.display = "block";
        document.getElementById("validate_interests").style.display = "none";
  
        if (xhr.responseText == 'false1') {
            document.getElementById('validate_interests').style.display = "block";
            document.getElementById('validate_interests').style.color = 'red';
            document.getElementById('validate_interests').innerHTML = "Too large description..";
        }
    }
    };
    
    xhr.send();
}

function other_ajaxPOST() {
    var xhr = new XMLHttpRequest();
    xhr.open("POST", 'ServletOther?Other=' + document.getElementById("other").value);
    xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    
    xhr.onload = function() {
    if (this.readyState == 4 && this.status == 200) { 
        
        document.getElementById("validate_other").style.display = "block";
        document.getElementById("validate_other").style.display = "none";
  
        if (xhr.responseText == 'false1') {
            document.getElementById('validate_other').style.display = "block";
            document.getElementById('validate_other').style.color = 'red';
            document.getElementById('validate_other').innerHTML = "Too large description..";
        }
    }
    };
    
    xhr.send();
}

function signup_ajaxPOST() {
    var xhr = new XMLHttpRequest();
    xhr.open("POST", 'ServletSignUp?1=' + document.getElementById('user').value + "&2=" 
    + document.getElementById('Email').value + "&3=" + document.getElementById('psw').value 
    + "&4=" + document.getElementById('confirm_psw').value + "&5=" 
    + document.getElementById("first_name").value + "&6=" + document.getElementById("last_name").value
    + 
    "&8=" + document.querySelector('input[name="gender"]:checked').value + "&9=" 
    + document.getElementById('country').value + "&10=" + document.getElementById("city").value
    + "&11=" + document.getElementById("address").value + "&12=" + document.getElementById("job").value 
    + "&13=" + document.getElementById("interests").value + "&14=" + document.getElementById("other").value);
    xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    
    xhr.onload = function() {
    if (this.readyState == 4 && this.status == 200) { 
        if (xhr.responseText == 'A new user signed up.')
            change_page(1);
            document.getElementById("success").style.display = "block";
    }
    };
    
    xhr.send();
}

function login_ajaxPOST() {
    var xhr = new XMLHttpRequest();
    xhr.open("POST", 'ServletLogin', true );
    xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    
    xhr.onload = function() {
    if (this.readyState == 4 && this.status == 200) {    
        
        document.getElementById("valid_user").style.display = "none";
        document.getElementById("valid_pass").style.display = "none";
        
        if(xhr.responseText == "Valid username and password.") {
            change_page(3);
            nameofuser = document.getElementById('existing_user').value;
        }
        else if(xhr.responseText == "Unkown_user") {
            document.getElementById('valid_user').style.display = "block";
            document.getElementById('valid_user').style.color = 'red';
            document.getElementById('valid_user').innerHTML = "This user does not exist.";
        }
        else if(xhr.responseText == "Wrong password") {
            document.getElementById('valid_pass').style.display = "block";
            document.getElementById('valid_pass').style.color = 'red';
            document.getElementById('valid_pass').innerHTML = "This is not your password.";
        }
    }
  
    };
    
    xhr.send('Username=' + document.getElementById("existing_user").value 
    + "&Password=" + document.getElementById("and_his_pass").value);
}

function change_page(x) {
    if(x==1) {
        document.getElementById("html_signup").style.display = "none";
        document.getElementById("htmllog_in").style.display = "block";
        document.getElementById("html_loggedin").style.display = "none";
    }
    if(x==2) {
        document.getElementById("html_signup").style.display = "block";
        document.getElementById("htmllog_in").style.display = "none";
    }
    if(x==3) {
        document.getElementById("html_signup").style.display = "none";
        document.getElementById("htmllog_in").style.display = "none";
        document.getElementById("html_loggedin").style.display = "block";
    }
}

function logout(){
    change_page(1);
    document.getElementById("success").style.display = "none";
}

function data() {
    var xhr = new XMLHttpRequest();
    xhr.open('POST', 'ServletforData', true);
    xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    xhr.onreadystatechange = function() {
    if (this.readyState === 4 && this.status === 200) {
                document.getElementById("returned_data").innerHTML = xhr.responseText;
                document.getElementById("returned_data").style.display = "block";
                document.getElementById("returned_users").style.display = "none";
    }
    };
    xhr.send('Username=' + nameofuser);
}

function Users() {
    var xhr = new XMLHttpRequest();
    xhr.open('POST', 'ServletforData', true);
    xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    xhr.onreadystatechange = function() {
        if (this.readyState === 4 && this.status === 200) {
            document.getElementById("returned_users").innerHTML = xhr.responseText;
            document.getElementById("returned_users").style.display = "block";
            document.getElementById("returned_data").style.display = "none";
            }
    }; 
    xhr.send('Username=' + 'all_users');
}