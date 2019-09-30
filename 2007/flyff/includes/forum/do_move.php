<?php
if ($_SESSION[rank] > 0 && $_POST[theme] && $_GET[id]) {
 mysql_query("UPDATE forum SET theme_id = '$_POST[theme]' WHERE id = '$_GET[id]'");
 header('Location: ?p=forum/topic&theme='.$_POST[theme].'&page='.$_GET[page].'&n=moved');
}
?>