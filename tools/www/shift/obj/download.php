<?php
  echo "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
  echo "<!DOCTYPE html>";
?>

<html xmlns="http://www.w3.org/1999/xhtml">
<head>
  <title>ObjShift: Download</title>
</head>
<body>

<?php
  if(isset($_POST["downloadfile"]))
  {
    $file=$_POST["downloadfile"];
    echo $file;
    
    if (file_exists($file)) 
    {
      header('Content-Description: File Transfer');
      header('Content-Type: application/octet-stream');
      header('Content-Disposition: attachment; filename='.basename($file));
      header('Content-Transfer-Encoding: binary');
      header('Expires: 0');
      header('Cache-Control: must-revalidate, post-check=0, pre-check=0');
      header('Pragma: public');
      header('Content-Length: ' . filesize($file));
      ob_clean();
      flush();
      readfile($file);
      exit;
    }
    else
    {
      echo " does not exist";
    }
  }
  else
  {
    echo "error";
  }

?>

  
  </body>
</html>
