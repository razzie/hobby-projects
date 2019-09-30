<?php
if ($_SESSION[rank] > 0 && $_GET[theme]) {
 $task = $_GET[task];
 if ($task == 'close') {
  mysql_query("UPDATE forum_themes SET closed = '1' WHERE id = '$_GET[theme]'");
  header('Location: ?p=forum/topic&theme='.$_GET[theme].'&page='.$_GET[page].'&n=closed');
 } else {
  mysql_query("UPDATE forum_themes SET closed = '0' WHERE id = '$_GET[theme]'");
  header('Location: ?p=forum/topic&theme='.$_GET[theme].'&page='.$_GET[page]);
 }
}
?>