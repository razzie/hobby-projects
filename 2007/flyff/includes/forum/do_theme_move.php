<?php
if ($_SESSION[rank] > 0 && $_POST[group] && $_GET[id]) {
 mysql_query("UPDATE forum_themes SET group_id = '$_POST[group]' WHERE id = '$_GET[id]'");
 header('Location: ?p=forum/themes&group='.$_POST[group].'&n=moved');
}
?>