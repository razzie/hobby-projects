<?php
 // include '../connect.php';
 mysql_query("UPDATE stats SET images = images + 1");
 header('Location: images/gallery/'.$_GET[id].'.jpg');
?>