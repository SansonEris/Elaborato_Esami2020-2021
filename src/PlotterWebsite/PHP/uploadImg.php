<?php
$target_dir = "../imgData/";
$target_file = $target_dir . basename($_FILES["fileToUpload"]["name"]);
$uploadOk = 1;
$imageFileType = strtolower(pathinfo($target_file,PATHINFO_EXTENSION));
$target_new_file_name = $target_dir."draw".".".$imageFileType;
$fileName = "draw".".".$imageFileType;

  if ($_FILES["fileToUpload"]["size"] > 500000) {
    echo "Sorry, your file is too large.";
    $uploadOk = 0;
  };

  if($imageFileType != "svg") {
    echo "Sorry, only svg ";
    $uploadOk = 0;
  }

// Check if $uploadOk is set to 0 by an error
  if ($uploadOk == 0) {
    echo "Sorry, your file was not uploaded.";
// if everything is ok, try to upload file
  } else {
    if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $target_new_file_name)) {
      echo "The file ". htmlspecialchars( basename(basename($_FILES["fileToUpload"]["name"]))). " has been uploaded.";
    } else {
      echo "Sorry, there was an error uploading your file.";
    }
  }


