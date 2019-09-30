<?php
if ($_SESSION[rank] > 1) {
 $name = $_POST[name];
 $desc = $_POST[description];
 if (test($name,5,100) && test($desc,10,200)) {
  $date = formed_time();
  $name = addslashes(htmlspecialchars($name));
  $desc = addslashes(htmlspecialchars($desc));
  $sql = mysql_fetch_array(mysql_query("SELECT * FROM forum_groups ORDER BY id DESC LIMIT 1"));
  $get_new_id = $sql[id] + 1;
  mysql_query("INSERT INTO forum_groups (id, group_name, description, owner) VALUES ('$get_new_id', '$name', '$desc', '$_SESSION[name]')");
  $success = 'success';
 } else {
  $success = 'fail';
 }
 header('Location: ?p=forum/groups&n='.$success);
}
?>