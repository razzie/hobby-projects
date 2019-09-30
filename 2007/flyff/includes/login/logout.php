<?php
if ($_SESSION[name]) {
 $time = '0';
 mysql_query("UPDATE users SET login2 = '$time' WHERE name = '$_SESSION[name]' LIMIT 1");
}
$_SESSION[name] = "";
$_SESSION[pass] = "";
$_SESSION[rank] = "";
setcookie('name', '');
setcookie('password', '');
header('Location: ?p=main/home');
?>