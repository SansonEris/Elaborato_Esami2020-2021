<?php
  $strJsonFileContents = file_get_contents("../canvasData/canvasData.json");
// Convert to array
  $array = json_decode($strJsonFileContents, true);
  var_dump($array); // print array
  /*
   * array(3) {
   * ["wave"]=> string(6)"square"
   * ["frequency"]=> string(3) "9999"
   * ["dutyCycle"]=> string(2) "50"
   * }
   */

