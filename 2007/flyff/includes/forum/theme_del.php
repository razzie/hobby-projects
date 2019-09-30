<?php
if ($_SESSION[rank] > 1 && $_GET[id] && $_GET[group]) {
 mysql_query("DELETE FROM forum_themes WHERE id = '$_GET[id]' LIMIT 1");
 mysql_query("DELETE FROM forum WHERE theme_id = '$_GET[id]'");
}
header('Location: ?p=forum/themes&group='.$_GET[group]);
?>