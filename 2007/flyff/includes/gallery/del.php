<?php
if ($_GET[id] && $_SESSION[name]) {
 $kepek_sql = mysql_fetch_array(mysql_query("SELECT id,name FROM images WHERE id = '$_GET[id]'"));
 $sql = mysql_fetch_array(mysql_query("SELECT rank FROM users WHERE name = '$kepek_sql[name]'"));
 if ($_SESSION[rank] > $sql[rank] && $kepek_sql[id]) {
  mysql_query("DELETE FROM images WHERE id = '$_GET[id]'");
  unlink('images/gallery/'.$_GET[id].'.jpg');
  unlink('images/gallery/'.$_GET[id].'_th.jpg');
  $comment = '<div class="red_line">A kiválasztott kép törlése megtörtént!</div><br /><br />';
 } else {
  if ($kepek_sql[name] == $_SESSION[name]) {
   mysql_query("DELETE FROM images WHERE id = '$_GET[id]'");
   unlink('images/gallery/'.$_GET[id].'.jpg');
   unlink('images/gallery/'.$_GET[id].'_th.jpg');
   $comment = '<div class="red_line">A kiválasztott kép törlése megtörtént!</div><br /><br />';
  }
 }
 header('Location: ?p=gallery/gallery&n=del&page='.$_GET[page]);
}
?>