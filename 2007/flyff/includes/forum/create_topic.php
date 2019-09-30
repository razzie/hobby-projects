<?php
if ($_SESSION[rank] > 0) {
 $name = $_POST[name];
 if (test($name,5,100) && $_GET[group]) {
  $date = formed_time();
  $name = addslashes(htmlspecialchars($name));
  $sql = mysql_fetch_array(mysql_query("SELECT * FROM forum_themes ORDER BY id DESC LIMIT 1"));
  $get_new_id = $sql[id] + 1;
  mysql_query("INSERT INTO forum_themes (id, theme_name, group_id, owner) VALUES ('$get_new_id', '$name', '$_GET[group]', '$_SESSION[name]')");
  $success = 'success';
 } else {
  $success = 'fail';
 }
 header('Location: ?p=forum/themes&group='.$_GET[group].'&n='.$success);
}
?>