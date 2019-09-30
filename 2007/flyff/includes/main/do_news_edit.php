<?php
if ($_SESSION[rank] > 0) {
 $title = $_POST[title];
 $text = $_POST[news];
 if (test2($title,3,200) && test2($text,3,2000)) {
  $title = addslashes(htmlspecialchars($title));
  // if ($_SESSION[rank] < 3) {
   $text = addslashes(htmlspecialchars($text));
  // }
  $date = formed_time();
  mysql_query("UPDATE news SET title = '$title', text = '$text', date_text = '$date' WHERE id = '$_GET[id]' LIMIT 1");
 }
}
header('Location: ?p=main/home&news_id='.$_GET[id].'#jump');
?>