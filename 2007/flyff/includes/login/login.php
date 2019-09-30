<?php
$user = "";
$posted_pass = md5($_POST[pass]);
$ip = $_SERVER[REMOTE_ADDR];
$vmi = mysql_fetch_array(mysql_query("SELECT * FROM users WHERE name = '$_POST[name]'"));
if ($posted_pass == $vmi[pass] && $vmi[name] == $_POST[name]) {
 if ($vmi[rank] < 1) {
  $ip_sql = mysql_fetch_array(mysql_query("SELECT * FROM ip_list WHERE ip = '$ip' LIMIT 1"));
  if ($ip_sql[ip] == $ip) { $banned_ip = 1; }
 }
 if ($vmi[banned] == 0 && $banned_ip == 0) {
  $_SESSION[name] = $_POST[name];
  $_SESSION[pass] = $posted_pass;
  $_SESSION[rank] = $vmi[rank];
  if ($_POST[cookie]) {
   setcookie('name', $_POST[name], time()+60*60*24*30);
   setcookie('password', $posted_pass, time()+60*60*24*30);
  } else {
   setcookie('name', '');
   setcookie('password', '');
  }
  mysql_query("UPDATE users SET ip = '$ip', login = NOW(), login2 = '$time' WHERE name = '$_POST[name]'");
  $forumname = $vmi[forum_name];
  if (strlen($forumname) < 3) { $forumname = $vmi[name]; }
  $comment = 'ok';
  $user = "ok";
 } else {
  if ($vmi[banned]) {
   $comment = 'ban';
  } else {
   $comment = 'ip_ban';
  }
 }
} else {
 $comment = 'wrong';
}
if ($user == "ok") { header('Location: ?p=main/home&n='.$comment); } else { header('Location: ?p=login/index&n='.$comment); }
?>