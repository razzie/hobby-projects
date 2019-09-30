<?php
if ($_SESSION[rank] > 0) {
 mysql_query("DELETE FROM forum WHERE id = '$_GET[id]' LIMIT 1");
}
header('Location: ?p=forum/topic&theme='.$_GET[theme].'&page='.$_GET[page]);
?>