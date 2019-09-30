<?php
// if (!$_GET[p] && !$_GET[a]) {
if (!$_SESSION[session]) {
 mysql_query("UPDATE stats SET guests = guests + 1");
} else {
 mysql_query("UPDATE stats SET downloads = downloads + 1");
}
?>