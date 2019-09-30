<?php
if ($_SESSION[name] && $_GET[id]) {
$sql = mysql_fetch_array(mysql_query("SELECT * FROM messages WHERE id = '$_GET[id]' AND (sender = '$_SESSION[name]' OR receiver = '$_SESSION[name]')"));
if ($sql[id]) {
 if ($sql[sender] == $_SESSION[name]) {
  mysql_query("UPDATE messages SET del1 = '1' WHERE id = '$_GET[id]'");
  $comment = '<div class="red_line">Az üzenet törölve!</div><br />';
 }
 if ($sql[receiver] == $_SESSION[name]) {
  mysql_query("UPDATE messages SET del2 = '1' WHERE id = '$_GET[id]'");
  $comment = '<div class="red_line">Az üzenet törölve!</div><br />';
 }
}
header('Location: ?p=messages/messages');
}
?>