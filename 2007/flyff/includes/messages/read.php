<?php
if ($_SESSION[name] && $_GET[id]) {
$sql = mysql_fetch_array(mysql_query("SELECT * FROM messages WHERE id = '$_GET[id]' AND (sender = '$_SESSION[name]' OR receiver = '$_SESSION[name]')"));
if ($sql[id]) {
 if ($sql[sender] == $_SESSION[name]) {
  $tipus = 'Címzett:';
  $thename = $sql[receiver];
 }
 if ($sql[receiver] == $_SESSION[name]) {
  $tipus = 'Küldõ:';
  $thename = $sql[sender];
  $reply = '<input type="button" onClick="location.href='."'".'?p=messages/write&amp;reply='.$_GET[id]."'".'" value="Válasz" class="button" />';
  mysql_query("UPDATE messages SET seen = '1' WHERE id = '$_GET[id]'");
 }
$adatok = mysql_fetch_array(mysql_query("SELECT name,forum_name FROM users WHERE name = '$thename'"));
$owner = $adatok[forum_name];
if (strlen($owner) < 3) { $owner = $thename; }
$message = $sql[message];
$message = str_replace('  ', ' &nbsp;', $message);
?>
<script language="Javascript" type="text/javascript">
function msg_del(id)
{
  question = window.confirm("Biztosan törölni akarod ezt az üzenetet?") ;
  if(question)
    window.location.href = "?a=messages/del&id=" + id ;
}
</script>
<div class="title">Üzenetek</div>
<br />
<br />
<table width="100%" cellspacing="0" cellpadding="0">
<tr valign="top" height="30">
 <td width="30%"><b><?php echo $tipus; ?></b></td>
 <td width="70%"><?php echo '<a href="?p=main/userdata&amp;user='.$thename.'" class="link">'.text($owner).'</a>'; ?></td>
</tr><tr valign="top" height="30">
 <td><b>Tárgy:</b></td>
 <td><?php echo text($sql[subject]); ?></td>
</tr><tr valign="top" height="30">
 <td><b>Dátum:</b></td>
 <td><?php echo $sql[date]; ?></td>
</tr><tr valign="top" height="30">
 <td colspan="2"><hr class="dashed_line" /></td>
</tr><tr valign="top" height="30">
 <td><b>Üzenet:</b></td>
 <td><?php echo smile(text(images($message))); ?></td>
</tr><tr valign="bottom" height="30">
 <td colspan="2"><hr class="dashed_line" /></td>
</tr>
</table>
<?php
echo '<br /><br />
<input type="button" class="button" onClick="history.back(1)" value="Vissza" />
<input type="button" class="button" onClick="msg_del('.$_GET[id].')" value="Töröl" />
'.$reply;
}
}
?>