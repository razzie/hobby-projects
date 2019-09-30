<?php
$edit_teszt = mysql_fetch_array(mysql_query("SELECT id,name FROM forum WHERE name = '$_SESSION[name]' AND theme_id = '$_GET[theme]' ORDER BY id DESC LIMIT 1"));
$edit_id = $edit_teszt[id];
$edit_name = $edit_teszt[name];
if ($_SESSION[rank] > 0 or ($_SESSION[name] == $edit_name && $_GET[id] == $edit_id)) {
 $message = $_POST[message];
 if (test2($message,5,2000)) {
  // if ($_SESSION[rank] < 3) {
   $message = addslashes(htmlspecialchars($message));
  // }
  mysql_query("UPDATE forum SET message = '$message' WHERE id = '$_GET[id]' LIMIT 1");
  header('Location: ?p=forum/topic&theme='.$_GET[theme].'&page='.$_GET[page].'&n=edit_ok');
 } else {
  header('Location: ?p=forum/topic&theme='.$_GET[theme].'&page='.$_GET[page].'&n=edit_fail');
 }
} else {
 header('Location: ?p=forum/topic&theme='.$_GET[theme].'&page='.$_GET[page].'&n=no_rights');
}
?>