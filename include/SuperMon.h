/*


  OK, ya ready for some fun? HTML + CSS styling + javascript all in and undebuggable environment

  one trick I've learned to how to debug HTML and CSS code.

  get all your HTML code (from html to /html) and past it into this test site
  muck with the HTML and CSS code until it's what you want
  https://www.w3schools.com/html/tryit.asp?filename=tryhtml_intro

  No clue how to debug javascrip other that write, compile, upload, refresh, guess, repeat

  I'm using class designators to set styles and id's for data updating
  for example:
  the CSS class .tabledata defines with the cell will look like
  <td><div class="tabledata" id = "switch"></div></td>

  the XML code will update the data where id = "switch"
  java script then uses getElementById
  document.getElementById("switch").innerHTML="Switch is OFF";


  .. now you can have the class define the look AND the class update the content, but you will then need
  a class for every data field that must be updated, here's what that will look like
  <td><div class="switch"></div></td>

  the XML code will update the data where class = "switch"
  java script then uses getElementsByClassName
  document.getElementsByClassName("switch")[0].style.color=text_color;


  the main general sections of a web page are the following and used here

  <html>
    <style>
    // dump CSS style stuff in here
    </style>
    <body>
      <header>
      // put header code for cute banners here
      </header>
      <main>
      // the buld of your web page contents
      </main>
      <footer>
      // put cute footer (c) 2021 xyz inc type thing
      </footer>
    </body>
    <script>
    // you java code between these tags
    </script>
  </html>


*/

// note R"KEYWORD( html page code )KEYWORD"; 
// again I hate strings, so char is it and this method let's us write naturally

const char PAGE_MAIN[] PROGMEM = R"=====(

<!DOCTYPE html>
<html lang="en" class="js-focus-visible">

<title>Web Page Motor Demo</title>

<style>
    table {
    position: relative;
    width:100%;
    border-spacing: 0px;
    border-collapse: collapse;
    width: 45%; /* Adjust width as needed */
    margin-bottom: 20px;
  }
  tr {
    border: 1px solid white;
    font-family: "Verdana", "Arial", sans-serif;
    font-size: 20px;
  }
  th {
    height: 20px;
    padding: 3px 15px;
    background-color: #343a40;
    color: #FFFFFF !important;
    }
  td {
    height: 20px;
     padding: 3px 15px;
  }
  .tabledata {
    font-size: 24px;
    position: relative;
    padding-left: 5px;
    padding-top: 5px;
    height:   25px;
    border-radius: 5px;
    color: #FFFFFF;
    line-height: 20px;
    transition: all 200ms ease-in-out;
    background-color: #00AA00;
  }
  .pwmslider {
    width: 90%;
    height: 55px;
    outline: none;
    height: 25px;
  }
  .bodytext {
    font-family: "Verdana", "Arial", sans-serif;
    font-size: 24px;
    text-align: left;
    font-weight: light;
    border-radius: 5px;
    display:inline;
  }
  .navbar {
    width: 100%;
    height: 50px;
    margin: 0;
    padding: 10px 0px;
    background-color: #FFF;
    color: #000000;
    border-bottom: 5px solid #293578;
  }
  .fixed-top {
    position: fixed;
    top: 0;
    right: 0;
    left: 0;
    z-index: 1030;
  }
  .navtitle {
    float: left;
    height: 50px;
    font-family: "Verdana", "Arial", sans-serif;
    font-size: 50px;
    font-weight: bold;
    line-height: 50px;
    padding-left: 20px;
  }
 .navheading {
   position: fixed;
   left: 60%;
   height: 50px;
   font-family: "Verdana", "Arial", sans-serif;
   font-size: 20px;
   font-weight: bold;
   line-height: 20px;
   padding-right: 20px;
 }
 .navdata {
    justify-content: flex-end;
    position: fixed;
    left: 70%;
    height: 50px;
    font-family: "Verdana", "Arial", sans-serif;
    font-size: 20px;
    font-weight: bold;
    line-height: 20px;
    padding-right: 20px;
 }
  .category {
    font-family: "Verdana", "Arial", sans-serif;
    font-weight: bold;
    font-size: 32px;
    line-height: 50px;
    padding: 20px 10px 0px 10px;
    color: #000000;
  }
  .heading {
    font-family: "Verdana", "Arial", sans-serif;
    font-weight: normal;
    font-size: 28px;
    text-align: left;
  }

  .btn {
    background-color: #444444;
    border: none;
    color: white;
    padding: 10px 60px;
    text-align: center;
    text-decoration: none;
    display: inline-block;
    font-size: 16px;
    margin: 4px 2px;
    cursor: pointer;
  }
  .active {
    background-color: #666666;
    color: black;
  }
  .text-input {
    font-family: "Verdana", "Arial", sans-serif;
    font-size: 20px;
    text-align: left;
    font-weight: light;
    border-radius: 5px;
    display:inline;
    width: 80%;
  }
  .foot {
    font-family: "Verdana", "Arial", sans-serif;
    font-size: 20px;
    position: relative;
    height:   30px;
    text-align: center;   
    color: #AAAAAA;
    line-height: 20px;
  }
  .container {
    max-width: 1800px;
    margin: 0 auto;
  }
  table tr:first-child th:first-child {
    border-top-left-radius: 5px;
  }
  table tr:first-child th:last-child {
    border-top-right-radius: 5px;
  }
  table tr:last-child td:first-child {
    border-bottom-left-radius: 5px;
  }
  table tr:last-child td:last-child {
    border-bottom-right-radius: 5px;
  }
</style>

  <body style="background-color: #efefef" onload="process()">
  
    <header>
      <div class="navbar fixed-top">
          <div class="container">
            <div class="navtitle">Robot Contorl Panel</div>
            <div class="navdata" id = "date">mm/dd/yyyy</div>
            <div class="navheading">DATE</div><br>
            <div class="navdata" id = "time">00:00:00</div>
            <div class="navheading">TIME</div>
            
          </div>
      </div>
    </header>
  
    <main class="container" style="margin-top:70px">
      <div class="category">Send to Robot</div>
      <div style="border-radius: 10px !important; display: flex; justify-content: space-between;">
        <table style="width:25%">
          <colgroup>
            <col span="1" style="background-color:rgb(230,230,230); width: 50%; color:#000000 ;">
            <col span="1" style="background-color:rgb(200,200,200); width: 50%; color:#000000 ;">
          </colgroup>
          <col span="2"style="background-color:rgb(0,0,0); color:#FFFFFF">
          <col span="2"style="background-color:rgb(0,0,0); color:#FFFFFF">
          <tr>
            <th colspan="2"><div class="heading">Symple Command</div></th>
          </tr>
          <tr>
            <td><div class="bodytext">Speed(PWM: <span id="ReceiveSpeed"></span>)</div></td>
            <td><div><input type="range" class="pwmslider" min="10" max="100" value = "0" width = "0%" oninput="UpdateSpeed(this.value)"/></div></td>
          </tr>
          <tr>
            <td class="button-cell"><button class="btn" id="btnForward" onclick="ButtonPressForward()">Forward</button></td>
            <td class="button-cell"><button class="btn" id="btnBackward" onclick="ButtonPressBackward()">Backward</button></td>
          </tr>
          <tr>
            <td class="button-cell"><button class="btn" id="btnLeft" onclick="ButtonPressLeft()">Turn_Left</button></td>
            <td class="button-cell"><button class="btn" id="btnRight" onclick="ButtonPressRight()">Turn_Right</button></td>
          </tr>
        </table>
        <table style="width:25%">
          <colgroup>
            <col span="1" style="background-color:rgb(230,230,230); width: 50%; color:#000000 ;">
            <col span="1" style="background-color:rgb(200,200,200); width: 50%; color:#000000 ;">
          </colgroup>
          <col span="2"style="background-color:rgb(0,0,0); color:#FFFFFF">
          <col span="2"style="background-color:rgb(0,0,0); color:#FFFFFF">
          <tr>
            <th colspan="2"><div class="heading">Go to Coordenates</div></th>
          </tr>
          <tr>
            <td class="input-cell"><div class="bodytext">X&nbsp;</div><input type="number" min="0" max="100" class="text-input" id="xtarget" class="text-input"></td>
            <td class="input-cell"><div class="bodytext">Y&nbsp;</div><input type="number" min="0" max="100" class="text-input" id="ytarget" class="text-input"></td>
          </tr>
          <tr>
            <td class="button-cell"><button class="btn" id="btnStartMove" onclick="ButtonPressStartMove()">Start</button></td>
            <td class="button-cell"><button class="btn" id="btnStopMove" onclick="ButtonPressStop()">Stop</button></td>
          </tr>
          <colgroup>
            <col span="1" style="background-color:rgb(230,230,230); width: 50%; color:#000000 ;">
            <col span="1" style="background-color:rgb(200,200,200); width: 50%; color:#000000 ;">
          </colgroup>
          <col span="2"style="background-color:rgb(0,0,0); color:#FFFFFF">
          <col span="2"style="background-color:rgb(0,0,0); color:#FFFFFF">
          <tr>
            <th colspan="2"><div class="heading">Go to Angle</div></th>
          </tr>
          <tr>
            <td><div class="bodytext">degrees</div></td>
            <td><div class="input-cell"><input type="number" min="0" max="360" class="text-input" id="target_angle"></div></td>
          </tr>
          <tr>
            <td class="button-cell"><button class="btn" id="btnStartAngle" onclick="ButtonPressStartAngle()">Start</button></td>
            <td class="button-cell"><button class="btn" id="btnStopAngle" onclick="ButtonPressStop()">Stop</button></td>
          </tr>
        </table>
        <table style="width:25%">
          <colgroup>
            <col span="1" style="background-color:rgb(230,230,230); width: 50%; color:#000000 ;">
            <col span="1" style="background-color:rgb(200,200,200); width: 50%; color:#000000 ;">
          </colgroup>
          <col span="2"style="background-color:rgb(0,0,0); color:#FFFFFF">
          <col span="2"style="background-color:rgb(0,0,0); color:#FFFFFF">
          <tr>
            <th colspan="2"><div class="heading">Trajectories</div></th>
          </tr>
          <tr>
            <td colspan="2"><div class="bodytext">Choose Trajectory</div></td>
          </tr>
          <tr>
            <td colspan="2" class="dropdown-cell">
              <select class="dropdown" id="selectTrajectory">
                  <option value="1">Square with 50cm radius</option>
                  <option value="2">Option 2</option>
                  <option value="3">Option 3</option>
              </select>
            </td>
          </tr>
          <tr>
            <td class="button-cell"><button class="btn" id="btnStartTrajectory" onclick="ButtonPressStartTrajectory()">Start</button></td>
            <td class="button-cell"><button class="btn" id="btnStopTrajectory" onclick="ButtonPressStop()">Stop</button></td>
          </tr>
        </table>
        <table style="width:25%">
          <colgroup>
            <col span="1" style="background-color:rgb(230,230,230); width: 50%; color:#000000 ;">
            <col span="1" style="background-color:rgb(200,200,200); width: 50%; color:#000000 ;">
          </colgroup>
          <col span="2"style="background-color:rgb(0,0,0); color:#FFFFFF">
          <col span="2"style="background-color:rgb(0,0,0); color:#FFFFFF">
          <tr>
            <th colspan="2"><div class="heading">GO Search</div></th>
          </tr>
          <tr>
            <td><div class="bodytext"></div></td>
            <td><div class="bodytext"></div></td>
          </tr>
        </table>
      </div>
      <br>
      <br>
      <div style="width:100%;overflow:auto;">
        <div style="width:39%;float:left;">
          <div class="category" >Robot Status</div>
          <br>
          <table style="width:100%">
            <colgroup>
              <col span="1" style="background-color:rgb(230,230,230); width: 50%; color:#000000 ;">
              <col span="1" style="background-color:rgb(200,200,200); width: 50%; color:#000000 ;">
            </colgroup>
            <col span="2"style="background-color:rgb(0,0,0); color:#FFFFFF">
            <col span="2"style="background-color:rgb(0,0,0); color:#FFFFFF">
            <tr>
              <td><div class="bodytext">Total Distance</div></td>
              <td><div class="bodytext" id = "totaldist"></div></td>
            </tr>
            <tr>
              <td><div class="bodytext">Instant Coordenates</div></td>
              <td><div class="bodytext">&nbspX&nbsp=&nbsp<span  id = "instcoorx"></span>&nbsp&nbsp&nbspY&nbsp=&nbsp<span id = "instcoory"></span></div></td>
            </tr>
            <tr>
              <td><div class="bodytext">Rotation Angle</div></td>
              <td><div class="bodytext">&nbsp<span  id = "instrot"></span>&deg</span></div></td>
            </tr>
            <tr>
              <td><div class="bodytext">Instant Velocity</div></td>
              <td><div class="bodytext" id = "instvelo"></div></td>
            </tr>
            <tr>
              <td><div class="bodytext"></div></td>
              <td><div class="bodytext" id = "e0"></div></td>
            </tr>
            <tr>
              <td><div class="bodytext">Victim Situation</div></td>
              <td><div class="bodytext" id = "vicstatus"></div></td>
            </tr>
          </table>
        </div>
        <div style="width:60%;float:left;">
          <div class="category" style="text-align:center;">Map</div>
          <br>
          <br>
        </div>
      </div>
      <br>
      <br>
      <div style="width:100%;overflow:auto;">
        <div class="bodytext" style="width:33%;float:left;">
          <h4>Team:</h4>
          <p>Jo&atilde;o Pratas n&deg;2020217923</p>
          <p>Jo&atilde;o Cunha n&deg;2020252382</p>
          <p>Bruno Silva n&deg;2021248603</p>
        </div>
        <div class="bodytext" style="width:33%;float:left;">
          <h4>Robot:</h4>
          <p>Micro-controlador: STM32F411CEU6</p>
          <p>Mirco-controlador: ESP32-WROOM-32D</p>
        </div>
        <div class="bodytext" style="width:33%;float:left;">
          <h4>Project details:</h4>
          <p>This project has a page dedicated to Documenting the project</p>
          <div class="button-cell"><button class="btn" onclick="ButtonDocPage()">GO TO</button></div>
        </div>
      </div>

    </main>

  <footer div class="foot" id = "temp" >Embedded Systems Project developed by Uvinercity of Coimbra<br>State of Development - Traveling predefined trajectoty</div></footer>
  
  </body>


  <script type="text/javascript">
      
    // global variable visible to all java functions
    var xmlHttp=createXmlHttpObject();
    // var button = document.getElementsByClassName("btn");


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
/*     for (var i = 0; i < button.length; i++) {
      button[i].addEventListener("mousedown", function() { //button is pressed
        button.classList.add("active");
      });
    }
    for (var i = 0; i < button.length; i++) {
      button[i].addEventListener("mouseup", function() { //button is released
        button.classList.remove("active");
      });
    } */
    // function to handle button press from HTML code above
    // and send a processing string back to server
    // this processing string is use in the .on method
    function ButtonPressForward() {
      var xhttp = new XMLHttpRequest(); 
      var value = 
      xhttp.open("PUT", "BUTTON_Forward", false);
      xhttp.send(); 
    }
    function ButtonPressBackward() {
      var xhttp = new XMLHttpRequest(); 
      xhttp.open("PUT", "BUTTON_Backward", false);
      xhttp.send(); 
    }
    function ButtonPressLeft() {
      var xhttp = new XMLHttpRequest(); 
      xhttp.open("PUT", "BUTTON_Left", false);
      xhttp.send(); 
    }
    function ButtonPressRight() {
      var xhttp = new XMLHttpRequest(); 
      xhttp.open("PUT", "BUTTON_Right", false);
      xhttp.send(); 
    }
    function ButtonPressStartMove() {
      var xhttp = new XMLHttpRequest(); 
      xhttp.open("PUT", "XTARGET?VALUE="+document.getElementById("xtarget").value, true);
      xhttp.send(); 
      var xhttp = new XMLHttpRequest(); 
      xhttp.open("PUT", "BUTTON_START?VALUE="+document.getElementById("ytarget").value, true);
      xhttp.send(); 
    }
    function ButtonPressStop() {
      var xhttp = new XMLHttpRequest(); 
      xhttp.open("PUT", "BUTTON_STOP", false);
      xhttp.send(); 
    }
    function ButtonPressStartAngle() {
      var xhttp = new XMLHttpRequest(); 
      xhttp.open("PUT", "BUTTON_TURN?VALUE="+document.getElementById("target_angle").value, true);
      xhttp.send(); 
    }
    function ButtonPressStartTrajectory() {
      var xhttp = new XMLHttpRequest(); 
      xhttp.open("PUT", "BUTTON_TRAJECTORY?VALUE="+document.getElementById("selectTrajectory").value, true);
      xhttp.send(); 
    }






    function ButtonDocPage(){
      var xhttp = new XMLHttpRequest(); 
      xhttp.open("GET", "document", false);
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

      xmldoc = xmlResponse.getElementsByTagName("E0"); //E0 value
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("btnStartMove").innerHTML="message";

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
  </script>

</html>




)=====";

const char PAGE_DOC[] PROGMEM = R"=====(
<!DOCTYPE html>
<!-- Created by pdf2htmlEX (https://github.com/pdf2htmlEX/pdf2htmlEX) -->
<html xmlns="http://www.w3.org/1999/xhtml">
<body>
			<p style="margin-top:0pt; margin-bottom:8pt">
				<img src="1715075369_projeto-se-24-04/1715075369_projeto-se-24-04-1.png" width="567" height="201" alt="" style="margin-right:9pt; margin-left:9pt; float:left; position:relative" >
			</p>
			<p style="margin-top:0pt; margin-bottom:8pt">
				&#xa0;
			</p>
			<p style="margin-top:0pt; margin-bottom:8pt">
				&#xa0;
			</p>
			<p style="margin-top:0pt; margin-bottom:8pt">
				&#xa0;
			</p>
			<p style="margin-top:0pt; margin-bottom:8pt">
				&#xa0;
			</p>
			<p style="margin-top:0pt; margin-bottom:8pt">
				&#xa0;
			</p>
			<p style="margin-top:0pt; margin-bottom:8pt; line-height:108%; font-size:10pt">
				&#xa0;
			</p>
			<p style="margin-top:0pt; margin-bottom:8pt; text-align:center; line-height:108%; font-size:16pt">
				<span style="font-family:Cambria">Universidade de Coimbra</span>
			</p>
			<p style="margin-top:0pt; margin-bottom:8pt; text-align:center; line-height:108%; font-size:16pt">
				<span style="font-family:Cambria">Faculdade de Ciências e Tecnologia</span>
			</p>
			<p style="margin-top:0pt; margin-bottom:8pt; text-align:center; line-height:108%; font-size:16pt">
				<span style="font-family:Cambria">Licenciatura em Engenharia Eletrotécnica e de Computadores</span>
			</p>
			<p style="margin-top:0pt; margin-bottom:8pt; text-align:center; line-height:108%; font-size:16pt">
				&#xa0;
			</p>
			<p style="margin-top:0pt; margin-bottom:8pt; line-height:108%; font-size:16pt">
				&#xa0;
			</p>
			<p style="margin-top:0pt; margin-bottom:8pt; text-align:center; line-height:108%; font-size:26pt">
				<span style="line-height:108%; font-family:Cambria; font-size:22pt; color:#4472c4">&#xa0;</span><span style="font-family:Cambria; color:#4472c4">Robô móvel diferencial para busca e salvamento</span>
			</p>
			<p style="margin-top:0pt; margin-bottom:8pt; line-height:108%; font-size:22pt">
				<span style="font-family:Cambria; color:#4472c4">&#xa0;</span>
			</p>
			<p style="margin-top:0pt; margin-bottom:8pt; text-align:center; line-height:108%; font-size:16pt">
				<span style="font-family:Cambria">&#xa0;</span>
			</p>
			<p style="margin-top:0pt; margin-bottom:8pt; text-align:center; line-height:108%; font-size:16pt">
				<span style="font-family:Cambria">Sistemas Embebidos</span>
			</p>
			<p style="margin-top:0pt; margin-bottom:8pt; text-align:center; line-height:108%; font-size:16pt">
				<span style="font-family:Cambria">&#xa0;</span><span style="font-family:Cambria">Professor </span><span style="line-height:108%; font-family:Cambria; font-size:14pt">Lino José Forte Marques</span>
			</p>
			<p style="margin-top:0pt; margin-bottom:8pt; text-align:center; line-height:108%; font-size:16pt">
				<span style="font-family:Cambria">Bruno Silva – 2021248603</span>
			</p>
			<p style="margin-top:0pt; margin-bottom:8pt; text-align:center; line-height:108%; font-size:16pt">
				<span style="font-family:Cambria">João Cunha – 2020252382</span>
			</p>
			<p style="margin-top:0pt; margin-bottom:8pt; text-align:center; line-height:108%; font-size:16pt">
				<span style="font-family:Cambria">João Pratas – </span><span style="line-height:108%; font-family:Cambria; font-size:14pt">2020217923</span>
			</p>
			<p style="margin-top:0pt; margin-bottom:8pt; line-height:108%; font-size:14pt">
				<span style="font-family:Cambria">&#xa0;</span>
			</p>
			<p style="margin-top:0pt; margin-left:36pt; margin-bottom:8pt">
				&#xa0;
			</p>
			<p style="margin-top:0pt; margin-bottom:8pt; text-align:center; line-height:108%; font-size:14pt">
				<span style="font-family:Cambria">&#xa0;</span>
			</p>
			<p style="margin-top:0pt; margin-bottom:8pt; text-align:center; line-height:108%; font-size:14pt">
				<span style="font-family:Cambria">Ano Letivo 2023/2024</span>
			</p>
			<p style="margin-top:0pt; margin-bottom:8pt; text-align:center; line-height:108%; font-size:14pt">
				<span style="font-family:Cambria">24/04/2024</span>
			</p>
			<p style="margin-top:0pt; margin-bottom:8pt; line-height:108%; font-size:22pt">
				<span style="font-family:Cambria; color:#4472c4">Definição da arquitetura de software</span>
			</p>
			<p style="margin-top:0pt; margin-bottom:8pt; line-height:108%; font-size:22pt">
				<span style="font-family:Cambria; color:#4472c4">&#xa0;</span>
			</p>
			<p style="margin-top:0pt; margin-bottom:8pt; text-indent:35.4pt; line-height:108%; font-size:16pt">
				Através da utilização do STM32F411CEU6 conseguimos alcançar o controlo dos motores envolvidos na movimentação do robô movel, para tal são utilizadas as funções de timers em modo encoder para obtermos os valores de quanto os motores se movimentam de modo a sabermos a sua posição. É também utilizado um timer para gerar pulsos de modo a controlar o duty cycle do PWM.
			</p>
			<p style="margin-top:0pt; margin-bottom:8pt; text-indent:35.4pt; line-height:108%; font-size:16pt">
				O STM32F411CEU6 comunica com o ESP32-WROOM-32D através de dois buffers “TxBuffer” para o envio e “RxBuffer” para receber. A dimensão deste buffer é definida em código podendo tomar valores arbitrários. No final da transmissão utilizamos a callback “HAL_SPI_TxRxCpltCallback” para armazenarmos os valores recebido no buffer e escrever os novos valores no buffer de transmissão para serem enviados na próxima comunicação. Dentro do “TxBuffer” residem os valores dos counters dos encoders de cada um dos motores. Dentro do “RxBuffer” é recebido o comando da próxima ação a realizar.
			</p>
			<p style="margin-top:0pt; margin-bottom:8pt; text-indent:35.4pt; line-height:108%; font-size:16pt">
				O ESP32-WROOM-32D é o master, sendo então este que controla quando as comunicações ocorrem. É também neste que se encontra o clock que sincroniza as comunicações entre o hardware. 
			</p>
			<p style="margin-top:0pt; margin-bottom:8pt; text-indent:35.4pt; line-height:108%; font-size:16pt">
				<span style="color:#313338">O ESP32 usa wifi em modo "station", liga-se à rede LAN para um dispositivo acessar uma página Web onde interagimos com o ESP32. A comunicação entre o ESP32 e a Web é feita através de "XML tables". Nesta página web podemos não só obter controlo manual do robô como também ter acesso aos valores dos sensores incorporados no sistema. Numa fase mais avançada do projeto será também possível visualizar um mapa com o percurso percorrido do robô.</span>
			</p>
			<p style="margin-top:0pt; margin-left:36pt; margin-bottom:0pt">
				&#xa0;
			</p>
			<p style="margin-top:0pt; margin-left:36pt; margin-bottom:8pt">
				&#xa0;
			</p>
			<p style="margin-top:0pt; margin-bottom:8pt; line-height:108%; font-size:22pt">
				<span style="font-family:Cambria; color:#4472c4">Plano de testes</span>
			</p>
			<ol type="I" style="margin:0pt; padding-left:0pt">
				<li style="margin-left:58.45pt; line-height:normal; padding-left:13pt; font-size:16pt">
					Teste de funcionalidade dos sensores de distância;
				</li>
				<li style="margin-left:58.45pt; line-height:normal; padding-left:13pt; font-size:16pt">
					Teste de funcionalidade dos motores DC com encoders;
				</li>
				<li style="margin-left:58.45pt; line-height:normal; padding-left:13pt; font-size:16pt">
					Teste de funcionalidade do modulo de leitura RFID;
				</li>
				<li style="margin-left:58.45pt; line-height:normal; padding-left:13pt; font-size:16pt">
					Teste de comunicação entre ESP32 e STM32 através de SPI; 
				</li>
				<li style="margin-left:58.45pt; line-height:normal; padding-left:13pt; font-size:16pt">
					Teste para a prevenção de colisão com os obstáculos;
				</li>
				<li style="margin-left:58.45pt; line-height:normal; padding-left:13pt; font-size:16pt">
					Teste de reconhecimento e orientação do robô no ambiente; 
				</li>
				<li style="margin-left:58.45pt; line-height:normal; padding-left:13pt; font-size:16pt">
					Teste de navegação do robô pelo ambiente até encontrar a “vítima”;
				</li>
				<li style="margin-left:58.45pt; line-height:normal; padding-left:13pt; font-size:16pt">
					Teste de resgaste da “vítima” até ao ponto de origem;
				</li>
				<li style="margin-left:58.45pt; line-height:normal; padding-left:13pt; font-size:16pt">
					Teste do desempenho geral do sistema, verificando a eficiência de cada tarefa, bem como a integração adequada de todos os componentes do sistema. 
				</li>
			</ol>
	<p style="bottom: 10px; right: 10px; position: absolute;"><a href="https://wordtohtml.net" target="_blank" style="font-size:11px; color: #d0d0d0">Converted to HTML with WordToHTML.net</a></p>
</body>
</html>


)=====";