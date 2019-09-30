<?php
if ($_SESSION[rank] > 0) {
?>
<div class="title">Áthelyezés</div>
<br />
<br />
Kérlek válaszd ki azt a fórumtémát, amelybe át akarod helyezni a kiválasztott hozzászólást:<br />
<?php
echo '
<form action="?a=forum/do_move&amp;id='.$_GET[id].'&amp;theme='.$_GET[theme].'&amp;page='.$_GET[page].'" method="POST"><br />
<select name="theme" style="width: 250px">
';
$query = mysql_query("SELECT * FROM forum_themes");
while ($sql = mysql_fetch_array($query)) {
 $vmi = mysql_fetch_array(mysql_query("SELECT group_name FROM forum_groups WHERE id = '$sql[group_id]'"));
 echo '<option value="'.$sql[id].'">'.no_format($vmi[group_name]).' \ '.no_format($sql[theme_name])."\n";
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