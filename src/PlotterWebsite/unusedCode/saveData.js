$(document).ready(function() {
  document.getElementById("check").style.display = 'none';
  document.getElementById("submit").style.display = 'block';
  document.getElementById("err").style.display = 'none';
});
$("#submit").on('click', function(){
    let selectionWave = document.getElementById("WaveformSelect").value; //  1 = square; 2 = pwm
    let frequencyVal = document.getElementById("Square").value; //Fosc/4...
    let dutyCycleVal = document.getElementById("dutyCycle").value; //PWM option

    function waveToString(){
      let stringValue = ["Square", "Pwm"];
      return stringValue[selectionWave-1];
    }

    let waveStringVal = waveToString();

    class SaveData{
      wave = waveStringVal;
      frequency = frequencyVal;
      dutyCycle = (this.wave == "Square")?0:dutyCycleVal;
    }

    let jsonClassData = new SaveData();
    if(jsonClassData.wave == null){
      jsonClassData.wave = jsonClassData.dutyCycle = jsonClassData.frequency = 0;
    }

    var arrayToString = JSON.stringify(Object.assign({}, jsonClassData));
    var stringToJsonObject = JSON.parse(arrayToString);
    console.log(stringToJsonObject);

    $.ajax({
      url: 'data\\getData.php',
      type : "POST",
      dataType : 'text',
      data : stringToJsonObject,
      success : function(result) {
        console.log("result-> "+result);
        document.getElementById("check").style.display = 'block';
        document.getElementById("submit").style.display = 'none';
      },
      error: function(xhr, resp, text) {
        console.log(xhr, resp, text);
        document.getElementById("check").style.display = 'none';
        document.getElementById("submit").style.display = 'none';
        document.getElementById("err").style.display = 'block'
      }
    })
  });
