<?php
$vmi = mysql_fetch_array(mysql_query("SELECT * FROM users WHERE CONVERT (name USING utf8) = '".$_GET[name]."' LIMIT 1"));
if ($_GET[old_pw] == $vmi[pass] && strlen($_GET[pw]) >= 3 && strlen($_GET[pw]) <= 20) {
 $password = md5($_GET[pw]);
 mysql_query("UPDATE users SET pass = '".$password."' WHERE CONVERT(name USING utf8) = '".$vmi[name]."' LIMIT 1");
 $comment = '<div class="green_line">A jelszavad sikeresen le lett cserélve, '.$vmi[name].'!</div><br />';
}
?>