<?php
 include '../connect.php';
 header('Content-type: image/jpeg');

 $result = mysql_query("SELECT avatar FROM users WHERE name = '$_GET[name]'");
 echo mysql_result($result, 0);
?>