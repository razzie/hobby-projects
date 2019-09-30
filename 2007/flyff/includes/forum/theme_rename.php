<?php
if ($_SESSION[rank] > 0) {
 $name = $_POST[name];
 if (test($name,5,100)) {
  $name = addslashes(htmlspecialchars($name));
  mysql_query("UPDATE forum_themes SET theme_name = '$name' WHERE id = '$_GET[theme]' LIMIT 1");
 }
}
header('Location: ?p=forum/themes&group='.$_GET[group]);
?>