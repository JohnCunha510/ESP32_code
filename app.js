  
    // global variable visible to all java functions
    var xmlHttp=createXmlHttpObject();
    var button = document.getElementsByClassName("btn");


    // function to create XML object
    function createXmlHttpObject(){
      if(window.XMLHttpRequest){
        xmlHttp=new XMLHttpRequest();
      }
      else{
        xmlHttp=new ActiveXObject("Microsoft.XMLHTTP");
      }
      return xmlHttp;
    }
    for (var i = 0; i < button.length; i++) {
      button[i].addEventListener("mousedown", function() { //button is pressed
        button.classList.add("active");
      });
    }
    for (var i = 0; i < button.length; i++) {
      button[i].addEventListener("mouseup", function() { //button is released
        button.classList.remove("active");
      });
    }
    // function to handle button press from HTML code above
    // and send a processing string back to server
    // this processing string is use in the .on method
    function ButtonPressForward() {
      var xhttp = new XMLHttpRequest(); 
      xhttp.open("PUT", "BUTTON_Forward?VALUE="+document.getElementById("target_distance").innerHTML, true);
      xhttp.send(); 
    }
    function ButtonPressBackward() {
      var xhttp = new XMLHttpRequest(); 
      xhttp.open("PUT", "BUTTON_Backward?VALUE="+document.getElementById("target_distance").innerHTML, true);
      xhttp.send(); 
    }
    function ButtonPressLeft() {
      var xhttp = new XMLHttpRequest(); 
      xhttp.open("PUT", "BUTTON_Left?VALUE="+document.getElementById("target_angle").innerHTML, true);
      xhttp.send(); 
    }
    function ButtonPressRight() {
      var xhttp = new XMLHttpRequest(); 
      var value = document.getElementById("target_angle").innerHTML;
      xhttp.open("PUT", "BUTTON_Right?VALUE="+value, true);
      xhttp.send(); 
    }
    function ButtonPressStart() {
      var xhttp = new XMLHttpRequest(); 
      xhttp.open("PUT", "XTARGET?VALUE="+document.getElementById("xtarget").innerHTML, true);
      xhttp.send(); 
      var xhttp = new XMLHttpRequest(); 
      xhttp.open("PUT", "BUTTON_START?VALUE="+document.getElementById("ytarget").innerHTML, true);
      xhttp.send(); 
    }



    function UpdateSlider(value) {
      var xhttp = new XMLHttpRequest();
      // this time i want immediate feedback to the fan speed
      // yea yea yea i realize i'm computing fan speed but the point
      // is how to give immediate feedback
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          // update the web based on reply from  ESP
          document.getElementById("pwmreceived").innerHTML=this.responseText;
        }
      }
      // this syntax is really weird the ? is a delimiter
      // first arg UPDATE_SLIDER is use in the .on method
      // server.on("/UPDATE_SLIDER", UpdateSlider);
      // then the second arg VALUE is use in the processing function
      // String t_state = server.arg("VALUE");
      // then + the controls value property
      xhttp.open("PUT", "PWM_update?VALUE="+value, true);
      xhttp.send();
    }
    function UpdateSpeed(value) {
      var xhttp = new XMLHttpRequest();
      // this time i want immediate feedback to the fan speed
      // yea yea yea i realize i'm computing fan speed but the point
      // is how to give immediate feedback
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          // update the web based on reply from  ESP
          document.getElementById("ReceiveSpeed").innerHTML=this.responseText;
        }
      }
      xhttp.open("PUT", "Speed?VALUE="+value, true);
      xhttp.send();
    }

    // function to handle the response from the ESP
    function response(){
      var message;
      var currentsensor;
      var xmlResponse;
      var xmldoc;
      var dt = new Date();
     
      // get the xml stream
      xmlResponse=xmlHttp.responseXML;
  
      // get host date and time
      document.getElementById("time").innerHTML = dt.toLocaleTimeString();
      document.getElementById("date").innerHTML = dt.toLocaleDateString();
  
      // Encoder value
      xmldoc = xmlResponse.getElementsByTagName("TOTALDIST"); //E0 value
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("totaldist").innerHTML=message;

      xmldoc = xmlResponse.getElementsByTagName("INSTVELO"); //E0 value
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("instvelo").innerHTML=message;

      xmldoc = xmlResponse.getElementsByTagName("INSTCOORX"); //E0 value
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("instcoorx").innerHTML=message;

      xmldoc = xmlResponse.getElementsByTagName("INSTCOORY"); //E0 value
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("instcoory").innerHTML=message;
      
      xmldoc = xmlResponse.getElementsByTagName("INSTROT"); //E0 value
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("instrot").innerHTML=message;

      xmldoc = xmlResponse.getElementsByTagName("VICSTATUS"); //E0 value
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("vicstatus").innerHTML=message;

     }

  
    // general processing code for the web page to ask for an XML steam
    // this is actually why you need to keep sending data to the page to 
    // force this call with stuff like this
    // server.on("/xml", SendXML);
    // otherwise the page will not request XML from the ESP, and updates will not happen
    function process(){
     
     if(xmlHttp.readyState==0 || xmlHttp.readyState==4) {
        xmlHttp.open("PUT","xml",true);
        xmlHttp.onreadystatechange=response;
        xmlHttp.send(null);
      }       
        // you may have to play with this value, big pages need more porcessing time, and hence
        // a longer timeout
        setTimeout("process()",100);
    }