<?php
if ($_SESSION[rank] > 1) {
 $date = formed_time();
 mysql_query("UPDATE stats SET guests = '0', downloads = '0', images = '0', restart = '$date'");
}
header('Location: ?p=main/stats');
?>