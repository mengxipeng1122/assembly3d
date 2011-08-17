<?php
  echo "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
  echo "<!DOCTYPE html>";
?>

<html xmlns="http://www.w3.org/1999/xhtml">
<head>
  <title>ObjShift: Result</title>
</head>
<body>
  <h1>ObjShift</h1>
  <div>A converter for OBJ-files.</div>
  <br/>  
<?php
if ($_FILES["file"]["error"] > 0)
{
  echo "Error while uploading (" . $_FILES["file"]["error"] . ")<br />";
}
else
{
//  echo "Upload: " . $_FILES["file"]["name"] . "<br />";
//  echo "Type: " . $_FILES["file"]["type"] . "<br />";
//  echo "Size: " . ($_FILES["file"]["size"] / 1024) . " Kb<br />";
//  echo "Stored in: " . $_FILES["file"]["tmp_name"] . "<br />";

  $name = $_FILES["file"]["name"];
  {
    $ext = strrchr($name, '.');  
    if($ext !== false)  
     {  
         $name = substr($name, 0, -strlen($ext));  
     }      
  }

  //input
  $inputDir = "/tmp/" . date_timestamp_get(date_create()) . DIRECTORY_SEPARATOR;      
  mkdir ( $inputDir);  
  $inputFile = $inputDir . $_FILES["file"]["name"];  
  move_uploaded_file($_FILES["file"]["tmp_name"], $inputFile);  
  
  //output
  $outputDir = $inputDir . "shift" . DIRECTORY_SEPARATOR;
  mkdir ( $outputDir);
  $outputFile = $outputDir . $name . ".zip";
  
  // concersion
  $cmd = "ObjShift.jar ";
  if(isset($_POST["multiple"]) && $_POST["multiple"] == "true")
  {
    $cmd = $cmd . "-m  ";
  }
  $cmd = $cmd . $inputFile . " " . $outputDir; 
  exec($cmd, $ok);
  
  // zip the result
  $zip = new ZipArchive();
  $ok = $zip->open($outputFile, ZIPARCHIVE::CREATE);
  $dirHandle = opendir($outputDir);
  
  // add files in the output directory
  while (false !== ($file = readdir($dirHandle))) 
  {
    $absFile = $outputDir . $file;
    if($file == "." || $file == "..")
    {
      continue;
    }
    
    if(file_exists($absFile)) 
    {
      echo "&nbsp;&nbsp;&nbsp; $file <br/>";
      $zip->addFile($absFile,$file);
    }
  }
 
  closedir($dirHandle);    
  
  $numFiles = $zip->numFiles;
  $zip->close();
  
  if($numFiles > 1 && file_exists($outputFile))
  {
    echo "<br/>";
    $downloadfile = $outputFile;
  }
}
?>
  <div>
    <?php if(isset($downloadfile)) { ?>    
    <form action="download.php" method="post" enctype="multipart/form-data">
      <input type="hidden" name="downloadfile" value="<?php echo $downloadfile; ?>" />
      <input type="submit" value="download"/>
    </form>
    <br/>
    <?php } else { ?>
    Error during conversion!<br/><br/>
    <?php } ?>
    <a href="index.xhtml"><input type="button" value="back"/></a>
  </div>
</body>
</html>
