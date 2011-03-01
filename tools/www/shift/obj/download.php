<?php
  if(isset($_POST["downloadfile"]) && file_exists($file = $_POST["downloadfile"]))
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
    echo "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    echo "<!DOCTYPE html>";      
?>
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
  <title>ObjShift: Download</title>
</head>
<body>
  error
</body>
</html>  
<?php  
  }
?>

  

