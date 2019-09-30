<?php
$name = $_POST[name];
$password_1st = $_POST[pass_elso];
$password_2nd = $_POST[pass_masodik];
$mail = $_POST[email];
$rules = $_POST[rules];

if (test3($name,3,20) && substr_count($name, '*') == 0 && substr_count($name, '#') == 0) {
 $sql = mysql_fetch_array(mysql_query("SELECT * FROM users WHERE name = '$name' LIMIT 1"));
 if (strtolower($sql[name]) != strtolower($name)) {
  if ($password_1st == $password_2nd) {
   if (test($password_1st,3,20) && test($password_2nd,3,20)) {
    if (test3($mail,8,50)) {
     if ($rules) {
      $ip = $_SERVER[REMOTE_ADDR];
      $pass = md5($password_1st);
      mysql_query("INSERT INTO users (name, pass, email, reg, ip) VALUES ('$name', '$pass', '$mail', NOW(), '$ip')");
      $reg = 'ok';
     } else {
      $reg = 'terms';
     }
    } else {
     $reg = 'badmail';
    }
   } else {
    $reg = 'badpw';
   }
  } else {
   $reg = 'badpw2';
  }
 } else {
  $reg = 'usedname';
 }
} else {
 $reg = 'badname';
}

if ($reg == 'ok') {
 header('Location: ?p=login/index&n=ok');
} else {
 header('Location: ?p=main/reg&n='.$reg);
}
?>