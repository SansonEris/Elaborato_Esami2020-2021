<?php
  $file = 'data.json';
  $data = json_decode($_POST['jsonData']);
  file_put_contents($file, json_encode($data));
 // echo json_encode($canvasData);

  $height = intval(json_decode($_POST['height']));
  $width = intval(json_decode($_POST['width']));

  //parse xml canvasData
  $xml = simplexml_load_string($data);
  //convert xml canvasData in array
  $array = json_decode(json_encode((array)$xml), TRUE);

  $drawMatrix = array(array());
  for($i = 0; $i < $height; $i++){
    for($j = 0; $j < $width; $j++){
      $drawMatrix[$i][$j] = 0;
    }
  }

  $dotsAB = array();
  $allDostsAB = array();

  foreach ($array['line'] as $item) {
    $dotsAB = $item['@attributes'];
    $dotsType = array_pop($dotsAB);
    $dotsAB = array_map('intval', $dotsAB);
    $x1 = $dotsAB['x1'];
    $x2 = $dotsAB['x2'];
    $y1 = $dotsAB['y1'];
    $y2 = $dotsAB['y2'];

    array_push($allDostsAB, array($x1,$y1,$x2,$y2));
  }

  for($i = 0; $i<count($allDostsAB); $i++){
    $Ax = $allDostsAB[$i][0];
    $Ay = $allDostsAB[$i][1];
    $Bx = $allDostsAB[$i][2];
    $By = $allDostsAB[$i][3];
    $drawMatrix = line($Ax,$Bx,$Ay,$By,$drawMatrix);
  }

 /* $Cxy = array();
  $Axy = array();
  $mergedDots = array();
  echo count($allDostsAB)."\n";
  foreach ($allDostsAB as $index => $item) {
    if($index != 0){
      array_push($Axy, array($item[0],$item[1]));
    }
    if($index != count($allDostsAB)-1){
      array_push($Cxy, array($item[2],$item[3]));
    }
  }

  for($i = 0; $i < sizeof($Cxy); $i++){
    $Ax1 = $Axy[$i][0];
    $Ay1 = $Axy[$i][1];
    $Cx1 = $Cxy[$i][0];
    $Cy1 = $Cxy[$i][1];
    $drawMatrix = line($Cx1, $Ax1,$Cy1,$Ay1,$drawMatrix);
  }
*/

/*drawLines*/
  function line($x1,$x2,$y1,$y2,$matrix){
    $matrix[$y1][$x1] = 1;
    $matrix[$y2][$x2] = 1;
    //vertical line
    if(($x1 == $x2) && ($y2 != $y1)){
      if($y1 > $y2){
        for($i = $y1; $i >= $y2; $i--){
            $matrix[$i][$x1] = 1;
        }
      }
      if($y1 < $y2){
        for($i = $y1; $i <= $y2; $i++){
          $matrix[$i][$x1] = 1;
        }
      }
    }
    //horizontal line
    if(($y1 == $y2) && ($x2 != $x1)){
      if($x1 > $x2){
        for($j = $x1; $j >= $x2; $j--){
          $matrix[$y1][$j] = 1;
        }
      }
      if($x1 < $x2){
        for($j = $x1; $j <= $x2; $j++){
          $matrix[$y1][$j] = 1;
        }
      }
    }

    //oblique line
    if(($y1 != $y2) && ($x2 != $x1)) {
      if ($x1 > $x2 && $y1 < $y2) {
        $j = $x1;
        for ($i = $y1; $i <= $y2; $i++) {
          $j--;
          if($j > $x2){
            $matrix[$i][$j] = 1;
          }
        }
      }
      if ($x1 > $x2 && $y1 > $y2) {
        $j = $x1;
        for ($i = $y1; $i >= $y2; $i--) {
          $j--;
          if($j >= $x2){
            $matrix[$i][$j] = 1;
          }
        }
      }
      if ($x1 < $x2 && $y1 > $y2) {
        $j = $x1;
        for ($i = $y1; $i >= $y2; $i--) {
          $j++;
          if($j <= $x2){
            $matrix[$i][$j] = 1;
          }
        }
      }
      if ($x1 < $x2 && $y1 < $y2) {
        $j = $x1;
        for ($i = $y1; $i <= $y2; $i++) {
          $j++;
          if($j <= $x2){
            $matrix[$i][$j] = 1;
          }
        }
      }

    }
    return $matrix;
  }

  //0 => "."
  //1 => "0"

  $fileString = "dataString.txt";
  $dataString = "";
  $coords = "";

  foreach ($drawMatrix as $yKey => $datum) {
    foreach ($datum as $xKey => $item) {
      $dataString .= ($item == 1) ? "0" : ".";
      if ($item == 1){
        $coords .= str_pad($xKey, 3, 0,STR_PAD_LEFT) . "\t" . str_pad($yKey, 3, 0,STR_PAD_LEFT) . "\t". "001"."\n";
      }
    }
    $dataString .= "\n";
  }

  file_put_contents($fileString, $dataString);
  file_put_contents("coords.txt", $coords);
  file_put_contents("dimension.txt", $height."\n".$width);


