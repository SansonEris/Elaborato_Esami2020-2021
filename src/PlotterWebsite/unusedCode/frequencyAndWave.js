//reset to default on reload
document.getElementById("default").selected = true;
document.getElementById("Square").value = 420;
document.getElementById("check").style.display = 'none';
document.getElementById("submit").style.display = 'block';
document.getElementById("err").style.display = 'none'

//dutyCycle real time value
function getDataRange(){
  document.getElementById("realTimeV").value = document.getElementById("dutyCycle").value;
}

//show option for selected element
function showDiv() {
    var value = document.getElementById("WaveformSelect").value;

    if(value == 1){
      document.getElementById("SquareWave").style.display = 'block';
      document.getElementById("PWM").style.display = 'none';
      console.log("square: "+value);
    } else if (value == 2) {
      document.getElementById("SquareWave").style.display = 'block';
      document.getElementById("PWM").style.display = 'block';
      console.log("PWM: " + value);
    }
      else {
      document.getElementById("SquareWave").style.display = 'none';
      document.getElementById("PWM").style.display = 'none';
    }

  }

function updateWave(){
  var waveValue = document.getElementById("WaveformSelect").value;
  var tableData = document.getElementById("tableData");
  var toStringValue = ["Square Wave", "PWM Wave"]
  tableData.rows[1].cells[1].innerHTML = toStringValue[waveValue-1];
  //reset to default visibility
  document.getElementById("check").style.display = 'none';
  document.getElementById("submit").style.display = 'block';
  document.getElementById("err").style.display = 'none'
}

function updateFrequency(){
    var waveValue = document.getElementById("WaveformSelect").value;
    var dutyCycle = document.getElementById("dutyCycle").value;
    var tableData = document.getElementById("tableData");
    //reset to default visibility
    document.getElementById("check").style.display = 'none';
    document.getElementById("submit").style.display = 'block';
    document.getElementById("err").style.display = 'none'

    if(waveValue == 1){
      tableData.rows[1].cells[2].innerHTML = Math.abs(document.getElementById("Square").value)+" Hz";
      tableData.rows[1].cells[3].innerHTML = 1/(Math.abs(document.getElementById("Square").value))+" T";
      document.getElementById("tableDuty").style.display = 'block';
      tableData.rows[1].cells[4].innerHTML = "Nan";

    }
    if(waveValue == 2){
      tableData.rows[1].cells[2].innerHTML = Math.abs(document.getElementById("Square").value)+" Hz";
      tableData.rows[1].cells[3].innerHTML = 1/(Math.abs(document.getElementById("Square").value))+" T";
      document.getElementById("tableDuty").style.display = 'block';
      tableData.rows[1].cells[4].innerHTML = Math.abs(dutyCycle)+"%";
      console.log("dutyCycle"+dutyCycle);

    }

}


