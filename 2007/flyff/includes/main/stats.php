<?php
if ($_SESSION["name"]) {
$vmi = mysql_fetch_array(mysql_query("SELECT * FROM stats LIMIT 1"));
$user_number = mysql_num_rows(mysql_query("SELECT name FROM users"));
$moderator_number = mysql_num_rows(mysql_query("SELECT name FROM users WHERE rank = '1'"));
$admin_number = mysql_num_rows(mysql_query("SELECT name FROM users WHERE rank > 1"));
$groups = mysql_num_rows(mysql_query("SELECT id FROM forum_groups"));
$themes = mysql_num_rows(mysql_query("SELECT id FROM forum_themes"));
$messages = mysql_num_rows(mysql_query("SELECT id FROM forum"));
$news = mysql_num_rows(mysql_query("SELECT id FROM news"));
$votes_query = mysql_query("SELECT * FROM votes");
$votes = mysql_num_rows($votes_query);
$uservotes = 0;
while ($i = mysql_fetch_array($votes_query)) {
 $uservotes += $i[count1] + $i[count2] + $i[count3] + $i[count4] + $i[count5] + $i[count6] + $i[count7] + $i[count8];
}
$kepek = 0;
$kepek = mysql_num_rows(mysql_query("SELECT id FROM images WHERE hidden = 0"));
if ($_SESSION[rank] > 0) {
 $kepek_hidden = 0;
 $kepek_hidden = mysql_num_rows(mysql_query("SELECT id FROM images WHERE hidden = 1"));
 $kepek_hidden = ' (+'.$kepek_hidden.' rejtett)';
}
$kepletolt_sql = mysql_fetch_array(mysql_query("SELECT images FROM stats"));
$kepletolt = 0;
$kepletolt = $kepletolt_sql[images];
?>
<script type="text/javascript">
function stats_del() 
{
  question = window.confirm("Biztosan t�r�lni akarod a statisztik�t?") ;
  if(question)
    window.location.href = "?a=main/stats_del" ;
}
</script>
<div class="title">Statisztika</div>
<br /><br />
<table><tr valign="top">
<td width="200px"><b>L�togat�sok sz�ma: </b></td><td><?php echo $vmi[guests]; ?></td>
</tr><tr valign="top">
<td width="200px"><b>Oldallet�lt�sek sz�ma: </b></td><td><?php echo $vmi[downloads]; ?></td>
</tr><tr valign="top" height="40px">
<td width="200px"><b>Sz�ml�l�k utolj�ra null�zva: </b></td><td><?php echo $vmi[restart]; ?></td>
</tr><tr valign="top">
<td width="200px"><b>Felhaszn�l�k sz�ma: </b></td><td><?php echo ($user_number - $moderator_number - $admin_number); ?></td>
</tr><tr valign="top">
<td width="200px"><b>Moder�torok sz�ma: </b></td><td><?php echo $moderator_number; ?></td>
</tr><tr valign="top" height="40px">
<td width="200px"><b>Adminisztr�torok sz�ma: </b></td><td><?php echo $admin_number; ?></td>
</tr><tr valign="top">
<td width="200px"><b>T�macsoportok sz�ma: </b></td><td><?php echo $groups; ?></td>
</tr><tr valign="top">
<td width="200px"><b>F�rumt�m�k sz�ma: </b></td><td><?php echo $themes; ?></td>
</tr><tr valign="top" height="40px">
<td width="200px"><b>Bejegyz�sek a f�rumban: </b></td><td><?php echo $messages; ?></td>
</tr><tr valign="top" height="40px">
<td width="200px"><b>H�rek sz�ma: </b></td><td><?php echo $news; ?></td>
</tr><tr valign="top">
<td width="200px"><b>Szavaz�sok sz�ma: </b></td><td><?php echo $votes; ?></td>
</tr><tr valign="top" height="40px">
<td width="200px"><b>�sszes szavazat sz�ma: </b></td><td><?php echo $uservotes; ?></td>
</tr><tr valign="top">
<td width="200px"><b>K�pek sz�ma: </b></td><td><?php echo $kepek.$kepek_hidden; ?></td>
</tr><tr valign="top">
<td width="200px"><b>K�plet�lt�sek sz�ma: </b></td><td><?php echo $kepletolt; ?></td>
</tr></table>
<?php if ($_SESSION[rank] > 1) { ?>
<br /><hr class="dashed_line" />
<center>[ <a href="javascript:stats_del()" title="Adatok t�rl�se" class="link">Adatok t�rl�se</a> ]</center>
<br />
<?php
}
}
?>
