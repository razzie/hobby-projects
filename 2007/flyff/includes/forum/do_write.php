<?php
if ($_SESSION[name]) {
 $message = $_POST[message];
 $sql = mysql_fetch_array(mysql_query("SELECT * FROM forum_themes WHERE id = '$_GET[theme]'"));
 if (test2($message,5,2000)) {
  if ($sql[closed] <> 1) {
  if ($_GET[answer]) {
   $reply_sql = mysql_fetch_array(mysql_query("SELECT * FROM forum WHERE id = '$_GET[answer]' LIMIT 1"));
   $reply = $reply_sql[message];
  }
  $date = formed_time();
  // if ($_SESSION[rank] < 3) {
   $message = addslashes(htmlspecialchars($message));
  // }
  mysql_query("INSERT INTO forum (`name`, `message`, `reply`, `date_text`, `theme_id`) VALUES ('$_SESSION[name]', '$message', '$reply', '$date', '$_GET[theme]')");
  header('Location: ?p=forum/topic&theme='.$_GET[theme].'&n=success');
  } else {
   header('Location: ?p=forum/topic&theme='.$_GET[theme].'&n=closed');
  }
 } else {
  if (!$_GET[answer]) {
   header('Location: ?p=forum/write&theme='.$_GET[theme].'&n=fail');
  } else {
   header('Location: ?p=forum/write&theme='.$_GET[theme].'&answer='.$_GET[answer].'&n=fail');
  }
 }
} else {
 header('Location: ?p=forum/topic&theme='.$_GET[theme].'&n=log_in');
}
?>