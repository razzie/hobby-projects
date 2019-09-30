<?php
if ($_SESSION[rank] > 2 && $_GET[id]) {
 $query = mysql_query("SELECT * FROM forum_themes WHERE group_id = '$_GET[id]'");
 while ($sql = mysql_fetch_array($query)) {
  mysql_query("DELETE FROM forum WHERE theme_id = '$sql[id]'");
  mysql_query("DELETE FROM forum_themes WHERE id = '$sql[id]'");
 }
 mysql_query("DELETE FROM forum_groups WHERE id = '$_GET[id]' LIMIT 1");
}
header('Location: ?p=forum/groups');
?>