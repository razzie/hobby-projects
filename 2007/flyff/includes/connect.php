<?php
 if (mysql_connect("localhost", "root", ""))
 {
  mysql_select_db("flyff");
  $mysql = "";
 } else {
  $mysql = "error";
 }
?>