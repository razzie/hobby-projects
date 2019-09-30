<?php
if ($_SESSION[name]) {
 $message = $_POST[message];
 $thename = $_POST[name];
 $subject = $_POST[subject];
 if (test2($message,5,2000) && test($subject,0,20) && $thename) {
  $date = formed_time();
  $message = addslashes(htmlspecialchars($message));
  $subject = addslashes(htmlspecialchars($subject));
  mysql_query("INSERT INTO messages (`sender`, `receiver`, `subject`, `date`, `message`) VALUES ('$_SESSION[name]', '$thename', '$subject', '$date', '$message')");
  $success = 1;
 } else {
  $success = 0;
 }
 header('Location: ?p=messages/messages&success='.$success);
}
?>