<?php
  $file = "coords.txt";
  $data = $_POST;
  file_put_contents($file, $data);

