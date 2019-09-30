<?php
if ($_SESSION[rank] > 0) {
 $title = $_POST[title];
 $text = $_POST[news];
 if (test2($title,3,200) && test2($text,3,2000)) {
  $date = formed_time();
  $title = addslashes(htmlspecialchars($title));
  // if ($_SESSION[rank] < 3) {
   $text = addslashes(htmlspecialchars($text));
  // }
  mysql_query("INSERT INTO news (owner, title, text, date_text) VALUES ('$_SESSION[name]', '$title', '$text', '$date')");
  $success = 1;
 } else {
  $success = 0;
 }
 header('Location: ?p=main/home&success='.$success);
}
?>