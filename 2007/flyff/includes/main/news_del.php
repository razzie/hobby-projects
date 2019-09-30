<?php
if ($_SESSION[rank] > 0) {
 mysql_query("DELETE FROM news WHERE id = '$_GET[id]' LIMIT 1");
}
header('Location: ?p=main/home');
?>