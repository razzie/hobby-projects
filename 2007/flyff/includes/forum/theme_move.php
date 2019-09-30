<?php
if ($_SESSION[rank] > 0) {
?>
<div class="title">Téma áthelyezése</div>
<br />
<br />
Kérlek válaszd ki azt a témacsoportot, amelybe át akarod helyezni a kiválasztott témát:<br />
<?php
echo '
<form action="?a=forum/do_theme_move&amp;id='.$_GET[id].'" method="POST"><br />
<select name="group" style="width: 200px">
';
$query = mysql_query("SELECT * FROM forum_groups");
while ($sql = mysql_fetch_array($query)) {
 echo '<option value="'.$sql[id].'">'.no_format($sql[group_name])."\n";
}
?>
</select>
<br /><br />
<input type="submit" value="Rendben" class="button" />
<input type="button" onClick="history.back(1)" value="Vissza" class="button" />
</form>
<?php
}
?>