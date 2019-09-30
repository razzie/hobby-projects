<?php
if ($_SESSION[rank] > 0) {
 $name = $_POST[name];
 $desc = $_POST[description];
 if (test($name,5,100) && test($desc,10,200)) {
  $name = addslashes(htmlspecialchars($name));
  $desc = addslashes(htmlspecialchars($desc));
  mysql_query("UPDATE forum_groups SET group_name = '$name', description = '$desc' WHERE id = '$_GET[group]' LIMIT 1");
 }
}
header('Location: ?p=forum/groups');
?>